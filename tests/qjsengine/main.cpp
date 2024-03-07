#include <QCoreApplication>
#include <QJSEngine>
#include <QDebug>

#include <stimmt/ModuleSystem.h>
#include <stimmt/Console.h>

using namespace stimmt;

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    QJSEngine engine;
    new extension::Console(&engine);
    auto ms = ModuleSystem(&engine);
    qDebug() << ms.loadModule("D:/a.js").toString();
    qDebug() << ms.loadModule("D:/b.js").property("add").call({1, 2}).toInt();
    return a.exec();
}