#include <QApplication>
#include <QJSEngine>
#include <QMainWindow>
#include <QPushButton>
#include <QProcess>

#include <stimmt/Watchdog.h>

using namespace stimmt;

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QMainWindow win;
    QJSEngine engine;
    auto button = new QPushButton("Test");
    win.setCentralWidget(button);
    win.show();

    QObject::connect(button, &QAbstractButton::clicked, [&] {
        Watchdog watchdog("stimmt_watchdog.exe", &engine);
        watchdog.start(5000);
        engine.evaluate("for (;;);");
        watchdog.stop();
        engine.setInterrupted(false);
    });

    return a.exec();
}
