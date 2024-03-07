#include <QCoreApplication>
#include <QJSEngine>
#include <QDebug>

//#include <stimmt/ModuleInjector.h>
//
//using namespace stimmt;

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    QJSEngine engine;
    engine.installExtensions(QJSEngine::ConsoleExtension);

    return a.exec();
}