#include <QCoreApplication>
#include <QJSEngine>
#include <QDebug>

#include <QDir>
#include <QUrl>

#include <stimmt/ModuleSystem.h>
#include <stimmt/Console.h>
#include <stimmt/ClosureHelper.h>
#include <stimmt/FileSystem.h>

using namespace stimmt;

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    QJSEngine engine;
    new extension::Console(&engine);
    return a.exec();
}