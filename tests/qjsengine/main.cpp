#include <stimmt/JavaScriptOutputWidget.h>
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QJSEngine>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

#include <stimmt/GlobalObject.h>
#include <stimmt/GlobalRegistryObject.h>
#include <stimmt/ProjectObject.h>

#include <stimmt/File.h>
#include <stimmt/JavaScriptDebuggingToolsWindow.h>

using namespace stimmt;

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    auto win = new QMainWindow;
    auto executeButton = new QPushButton("Execute Script");
    auto label = new QLabel;
    auto progressBar = new QProgressBar;
    progressBar->hide();
    auto cancelButton = new QPushButton("Cancel");
    cancelButton->setEnabled(false);
    auto mainWidget = new QWidget;
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(executeButton);
    mainLayout->addWidget(label);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(cancelButton);
    mainWidget->setLayout(mainLayout);
    win->setCentralWidget(mainWidget);
    win->show();
    GlobalObject global;
    auto ret = global.load(":/scripts/test.js");
    if (ret.isError())
        qDebug() << ret.toString();
    qDebug() << global.registry()->scripts();
    ProjectObject project(win);
    QObject::connect(&project, &ProjectObject::progressStarted, win, [=](const QString &title, int maximum) {
        label->setText(title);
        progressBar->show();
        progressBar->setMaximum(maximum);
        progressBar->setValue(0);
        cancelButton->setEnabled(true);
    });
    QObject::connect(&project, &ProjectObject::progressValueChanged, progressBar, &QProgressBar::setValue);
    QObject::connect(&project, &ProjectObject::progressFinished, win, [=]() {
        label->clear();
        progressBar->hide();
        cancelButton->setEnabled(false);
    });
    QObject::connect(cancelButton, &QPushButton::clicked, []() {
        jsGlobal->engine()->setInterrupted(true);
        QTimer::singleShot(0, []() {
            jsGlobal->engine()->setInterrupted(false);
        });
    });
    QObject::connect(executeButton, &QPushButton::clicked, [&]() {
        executeButton->setDisabled(true);
        ret = project.invoke("test");
        if (ret.isError())
            qDebug() << ret.toString();
        executeButton->setDisabled(false);
    });
    project.invoke("transpose", 4);
    auto outputWin = new JavaScriptDebuggingToolsWindow(win);
    auto outputButton = new QPushButton("Debugging Tools");
    mainLayout->addWidget(outputButton);
    outputWin->hide();
    QObject::connect(outputButton, &QPushButton::clicked, outputWin, &QWidget::show);
    return a.exec();
}