#ifndef STIMMT_JAVASCRIPTOUTPUTWIDGET_H
#define STIMMT_JAVASCRIPTOUTPUTWIDGET_H

#include <QWidget>

#include <stimmt/StimmtGlobal.h>

class QTextBrowser;

namespace stimmt {
    class STIMMT_EXPORT JavaScriptOutputWidget : public QWidget {
        Q_OBJECT;

    public:
        explicit JavaScriptOutputWidget(QWidget *parent = nullptr);

    public slots:
        void receiveMessage(int level, const QString &message, const QString &fileTrace);
        void clear();

    private:
        QTextBrowser *m_textEdit;
        int m_levelFlags = -1;
        void setLevelFlag(int flag, bool enabled);

        bool m_showFileTrace = true;
        bool m_showTime = false;
        QString m_cachedMessages;

        void exportCachedMessages();

        struct {
            QString text;
            bool matchCase;
            bool isRegEx;
            bool matchWholeWords;
        } m_findContext;
        QAction *m_findNextAction;
        QAction *m_findPreviousAction;
        void openFindDialog();
        bool findNext(bool backward);
    };
}

#endif // STIMMT_JAVASCRIPTOUTPUTWIDGET_H
