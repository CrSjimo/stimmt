#include "JavaScriptOutputWidget.h"

#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFontDialog>
#include <QMenuBar>
#include <QPainter>
#include <QTextBrowser>
#include <QTextDocument>
#include <QTextTable>
#include <QTime>
#include <QVBoxLayout>
#include <QtSvg/QSvgRenderer>
#include <QFormLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>

#include "Global/Console.h"
#include "Global/GlobalObject.h"

namespace stimmt {
    JavaScriptOutputWidget::JavaScriptOutputWidget(QWidget * parent) : QWidget(parent), m_textEdit(new QTextBrowser) {
        auto mainLayout = new QVBoxLayout;
        setLayout(mainLayout);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        auto menuBar = new QMenuBar;

        auto fileMenu = menuBar->addMenu(tr("&File"));

        auto exportAction = fileMenu->addAction(tr("E&xport"));
        connect(exportAction, &QAction::triggered, this, &JavaScriptOutputWidget::exportCachedMessages);

        auto editMenu = menuBar->addMenu(tr("&Edit"));

        auto copyAction = editMenu->addAction(tr("&Copy"));
        copyAction->setShortcut(QKeySequence::Copy);
        copyAction->setEnabled(false);
        connect(m_textEdit, &QTextEdit::copyAvailable, copyAction, &QAction::setEnabled);
        connect(copyAction, &QAction::triggered, m_textEdit, &QTextEdit::copy);

        editMenu->addSeparator();

        auto findAction = editMenu->addAction(tr("&Find"));
        findAction->setShortcut(QKeySequence::Find);
        connect(findAction, &QAction::triggered, this, &JavaScriptOutputWidget::openFindDialog);

        m_findNextAction = editMenu->addAction(tr("Find &Next"));
        m_findNextAction->setShortcut(QKeySequence::FindNext);
        m_findNextAction->setEnabled(false);
        connect(m_findNextAction, &QAction::triggered, this, [=] { findNext(false); });

        m_findPreviousAction = editMenu->addAction(tr("Find &Previous"));
        m_findPreviousAction->setShortcut(QKeySequence::FindPrevious);
        m_findPreviousAction->setEnabled(false);
        connect(m_findPreviousAction, &QAction::triggered, this, [=] { findNext(true); });

        editMenu->addSeparator();

        auto selectAllAction = editMenu->addAction(tr("Select &All"));
        selectAllAction->setShortcut(QKeySequence::SelectAll);
        connect(selectAllAction, &QAction::triggered, this, [=]() { m_textEdit->selectAll(); });

        editMenu->addSeparator();

        auto clearAction = editMenu->addAction(tr("C&lear"));
        connect(clearAction, &QAction::triggered, this, &JavaScriptOutputWidget::clear);

        auto viewMenu = menuBar->addMenu(tr("&View"));
        auto errorAction = viewMenu->addAction(tr("&Error"));
        errorAction->setCheckable(true);
        errorAction->setChecked(true);
        connect(errorAction, &QAction::triggered, this, [=](bool checked) { setLevelFlag(Console::Error, checked); });

        auto warningAction = viewMenu->addAction(tr("&Warning"));
        warningAction->setCheckable(true);
        warningAction->setChecked(true);
        connect(warningAction, &QAction::triggered, this,
                [=](bool checked) { setLevelFlag(Console::Warning, checked); });

        auto logAction = viewMenu->addAction(tr("&Log"));
        logAction->setCheckable(true);
        logAction->setChecked(true);
        connect(logAction, &QAction::triggered, this, [=](bool checked) { setLevelFlag(Console::Log, checked); });

        auto infoAction = viewMenu->addAction(tr("&Info"));
        infoAction->setCheckable(true);
        infoAction->setChecked(true);
        connect(infoAction, &QAction::triggered, this, [=](bool checked) { setLevelFlag(Console::Info, checked); });

        auto debugAction = viewMenu->addAction(tr("&Debug"));
        debugAction->setCheckable(true);
        debugAction->setChecked(true);
        connect(debugAction, &QAction::triggered, this, [=](bool checked) { setLevelFlag(Console::Debug, checked); });

        viewMenu->addSeparator();

        auto showFileTraceAction = viewMenu->addAction(tr("Show &File Trace"));
        showFileTraceAction->setCheckable(true);
        showFileTraceAction->setChecked(true);
        connect(showFileTraceAction, &QAction::triggered, this, [=](bool checked) { m_showFileTrace = checked; });

        auto showTimeAction = viewMenu->addAction(tr("Show &Time"));
        showTimeAction->setCheckable(true);
        showTimeAction->setChecked(false);
        connect(showTimeAction, &QAction::triggered, this, [=](bool checked) { m_showTime = checked; });

        viewMenu->addSeparator();

        auto fontAction = viewMenu->addAction(tr("Fo&nt"));
        connect(fontAction, &QAction::triggered, this, [=]() {
            bool ok;
            QFont font = QFontDialog::getFont(&ok, m_textEdit->font(), this);
            if (ok) {
                m_textEdit->setFont(font);
                // TODO save font
            }
        });

        auto wordWrapAction = viewMenu->addAction(tr("W&ord Wrap"));
        wordWrapAction->setCheckable(true);
        wordWrapAction->setChecked(true);
        connect(wordWrapAction, &QAction::triggered, this, [=](bool checked) {
            if (checked)
                m_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
            else
                m_textEdit->setLineWrapMode(QTextEdit::NoWrap);
        });

        mainLayout->addWidget(menuBar);

        m_textEdit->setOpenLinks(false);
        QFont font("Monospace", 10);
        font.setStyleHint(QFont::TypeWriter);
        m_textEdit->setFont(font);

        connect(m_textEdit, &QTextBrowser::anchorClicked, this,
                [=](const QUrl &url) { QDesktopServices::openUrl(url); });
        mainLayout->addWidget(m_textEdit);

        setMinimumSize(600, 400);

        connect(jsGlobal->console(), &Console::messagePrinted, this, &JavaScriptOutputWidget::receiveMessage);
        connect(jsGlobal->console(), &Console::clearRequested, this, &JavaScriptOutputWidget::clear);
    }

    void JavaScriptOutputWidget::receiveMessage(int level, const QString &message, const QString &fileTrace) {
        QString levelText = QStringList{"[Debug]", "[Info]", "[Log]", "[Warning]", "[Error]"}[level];
        auto timeText = QString("[%1]").arg(QTime::currentTime().toString());
        for (const auto &s : message.split('\n'))
            m_cachedMessages += (levelText + "[" + fileTrace + "]" + timeText + " " + s + "\n");

        if (!(m_levelFlags & (1 << level)))
            return;
        QColor levelColor = QList<QColor>{
            {0x16, 0xa0, 0x85},
            {0x40, 0x73, 0x9e},
            {},
            {0xe6, 0x7e, 0x22},
            {0xc0, 0x39, 0x2b}
        }[level];
        QColor levelBackground = QList<QColor>{
            {},
            {0xa5, 0xd8, 0xff},
            {},
            {0xff, 0xec, 0x99},
            {0xff, 0xc9, 0xc9}
        }[level];
        QString levelImage = QStringList{{}, ":/stimmt/icons/info.svg", {}, ":/stimmt/icons/warning.svg", ":/stimmt/icons/error.svg"}[level];
        QTextCursor cur(m_textEdit->document());
        cur.movePosition(QTextCursor::End);
        cur.movePosition(QTextCursor::PreviousBlock);
        QTextTable *table = cur.currentTable();
        if (!table) {
            QTextTableFormat tableFmt;
            tableFmt.setWidth(QTextLength(QTextLength::PercentageLength, 100));
            tableFmt.setCellSpacing(0);
            tableFmt.setCellPadding(4);
            QBrush bs(Qt::BrushStyle::SolidPattern);
            bs.setColor({0xbf, 0xbf, 0xbf});
            tableFmt.setBorderBrush(bs);
            table = cur.insertTable(1, 1, tableFmt);
        } else {
            table->appendRows(1);
        }
        QTextCharFormat cellFmt;
        if (levelBackground.isValid())
            cellFmt.setBackground(QBrush{levelBackground});
        table->cellAt(table->rows() - 1, 0).setFormat(cellFmt);
        cur = table->cellAt(table->rows() - 1, 0).firstCursorPosition();

        QTextCharFormat fmt;
        fmt.setForeground(QBrush(levelColor));

        if (!levelImage.isEmpty()) {
            QTextTableFormat iconTableFmt;
            iconTableFmt.setBorder(0);
            iconTableFmt.setMargin(0);
            auto iconTable = cur.insertTable(1, 2, iconTableFmt);
            cur = iconTable->cellAt(0, 0).firstCursorPosition();
            double ratio = qApp->primaryScreen()->logicalDotsPerInch() / 96; // TODO use QtMediate QMOS::unitDpi in the future
            QImage img(12 * ratio, 12 * ratio, QImage::Format_ARGB32);
            img.fill(QColor(0, 0, 0, 0));
            QSvgRenderer renderer(levelImage);
            QPainter painter(&img);
            renderer.render(&painter);
            cur.insertImage(img);
            cur = iconTable->cellAt(0, 1).firstCursorPosition();
        }
        QRegExp rx(R"(file://[A-Za-z0-9\$\-_\.\+!\*'\(\)\/&\?=:%]*)");
        int pos = 0;
        int offsetPos = 0;
        while ((pos = rx.indexIn(message, offsetPos)) != -1) {
            cur.insertText(message.mid(offsetPos, pos - offsetPos), fmt);
            fmt.setAnchor(true);
            auto href = rx.cap();
            href.replace(QRegExp("(:[0-9]+)+$"), "");
            fmt.setAnchorHref(href);
            fmt.setUnderlineStyle(QTextCharFormat::SingleUnderline);
            cur.insertText(rx.cap(), fmt);
            fmt.setAnchor(false);
            fmt.setAnchorHref({});
            fmt.setUnderlineStyle(QTextCharFormat::NoUnderline);
            offsetPos = pos + rx.matchedLength();
        }
        cur.insertText(message.mid(offsetPos), fmt);
        fmt.setForeground(QBrush(QColor{0x7f, 0x7f, 0x7f}));
        cur.insertText(" ", fmt);


        // File Trace
        if (m_showFileTrace && !fileTrace.isEmpty()) {
            cur.insertText("[", fmt);
            QUrl fileUrl(fileTrace);
            if (fileUrl.isLocalFile()) {
                fmt.setAnchor(true);
                auto href = fileTrace;
                href.replace(QRegExp("(:[0-9]+)+$"), "");
                fmt.setAnchorHref(href);
                fmt.setUnderlineStyle(QTextCharFormat::SingleUnderline);
            }
            cur.insertText(fileUrl.fileName(), fmt);
            fmt.setAnchor(false);
            fmt.setAnchorHref({});
            fmt.setUnderlineStyle(QTextCharFormat::NoUnderline);
            cur.insertText("]", fmt);
        }

        // Time
        if (m_showTime) {
            cur.insertText(timeText, fmt);
        }
    }

    void JavaScriptOutputWidget::clear() {
        m_textEdit->clear();
        m_cachedMessages.clear();
    }

    void JavaScriptOutputWidget::setLevelFlag(int flag, bool enabled) {
        if (enabled)
            m_levelFlags |= (1 << flag);
        else
            m_levelFlags &= ~(1 << flag);
    }

    void JavaScriptOutputWidget::exportCachedMessages() {
        auto initialFileName =
                "javascript-output-" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + ".log";
        auto filePath = QFileDialog::getSaveFileName(this, {}, initialFileName,
                                                     QString("%1 (*.txt *.log)").arg(tr("Text files")));
        if (filePath.isEmpty())
            return;
        QFile f(filePath);
        f.open(QIODevice::WriteOnly);
        f.write(m_cachedMessages.toUtf8());
    }

    void JavaScriptOutputWidget::openFindDialog() {
        QDialog dlg(this);
        dlg.setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        auto dlgLayout = new QHBoxLayout;
        auto findSectionLayout = new QVBoxLayout;
        auto findEditLayout = new QFormLayout;
        auto findEdit = new QLineEdit;
        findEditLayout->addRow(tr("&Find"), findEdit);
        auto optionsDirectionLayout = new QHBoxLayout;
        auto optionsGroupBox = new QGroupBox(tr("Options"));
        auto optionsLayout = new QVBoxLayout;
        auto matchCaseCheckBox = new QCheckBox(tr("Match &Case"));
        auto regExCheckBox = new QCheckBox(tr("&Regular Expression"));
        auto matchWholeWordsCheckBox = new QCheckBox(tr("Match &Whole Words"));
        optionsLayout->addWidget(matchCaseCheckBox);
        optionsLayout->addWidget(regExCheckBox);
        optionsLayout->addWidget(matchWholeWordsCheckBox);
        optionsGroupBox->setLayout(optionsLayout);
        auto directionGroupBox = new QGroupBox(tr("Direction"));
        auto directionLayout = new QVBoxLayout;
        auto upRadio = new QRadioButton(tr("&Up"));
        auto downRadio = new QRadioButton(tr("&Down"));
        directionLayout->addWidget(upRadio);
        directionLayout->addWidget(downRadio);
        directionGroupBox->setLayout(directionLayout);
        optionsDirectionLayout->addWidget(optionsGroupBox);
        optionsDirectionLayout->addWidget(directionGroupBox);
        findSectionLayout->addLayout(findEditLayout);
        findSectionLayout->addLayout(optionsDirectionLayout, 1);
        auto buttonLayout = new QVBoxLayout;
        auto findButton = new QPushButton(tr("Find"));
        auto cancelButton = new QPushButton(tr("Cancel"));
        buttonLayout->addWidget(findButton);
        buttonLayout->addWidget(cancelButton);
        buttonLayout->addStretch();
        dlgLayout->addLayout(findSectionLayout);
        dlgLayout->addLayout(buttonLayout);
        dlg.setLayout(dlgLayout);

        connect(findEdit, &QLineEdit::textChanged, findButton, [=] { findButton->setDisabled(findEdit->text().isEmpty()); });
        findButton->setDefault(true);
        connect(findButton, &QPushButton::clicked, &dlg, [=, &dlg] {
            m_findNextAction->setEnabled(true);
            m_findPreviousAction->setEnabled(true);
            m_findContext.text = findEdit->text();
            m_findContext.matchCase = matchCaseCheckBox->isChecked();
            m_findContext.isRegEx = regExCheckBox->isChecked();
            m_findContext.matchWholeWords = matchWholeWordsCheckBox->isChecked();
            if (upRadio->isChecked()) {
                if (findNext(true))
                    dlg.accept();
            } else {
                if (findNext(false))
                    dlg.accept();
            }
        });
        connect(cancelButton, &QPushButton::clicked, &dlg, &QDialog::reject);
        if (!m_findContext.text.isEmpty()) {
            findEdit->setText(m_findContext.text);
            matchCaseCheckBox->setChecked(m_findContext.matchCase);
            regExCheckBox->setChecked(m_findContext.isRegEx);
            matchWholeWordsCheckBox->setChecked(m_findContext.matchWholeWords);
        }
        downRadio->setChecked(true);
        dlg.exec();

    }

    bool JavaScriptOutputWidget::findNext(bool backward) {
        QTextDocument::FindFlags flags;
        if (m_findContext.matchCase)
            flags |= QTextDocument::FindCaseSensitively;
        if (m_findContext.matchWholeWords)
            flags |= QTextDocument::FindWholeWords;
        if (backward)
            flags |= QTextDocument::FindBackward;
        bool ret;
        for (int i = 0; i < 2; i++) {
            if (m_findContext.isRegEx)
                ret = m_textEdit->find(QRegExp(m_findContext.text), flags);
            else
                ret = m_textEdit->find(m_findContext.text, flags);
            if (ret)
                break;
            else if (i == 0)
                m_textEdit->moveCursor(backward ? QTextCursor::End : QTextCursor::Start);
        }
        if (!ret) {
            QMessageBox::warning(this, {}, tr(R"(Cannot find "%1".)").arg(m_findContext.text));
        }
        return ret;
    }
}
