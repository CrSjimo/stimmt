#include <QCoreApplication>
#include <QJSEngine>
#include <QDebug>

#include <QDir>

#include <stimmt/ModuleSystem.h>
#include <stimmt/Console.h>
#include <stimmt/ClosureHelper.h>
#include <stimmt/FileSystem.h>

using namespace stimmt;

int main(int argc, char **argv) {
    QCoreApplication a(argc, argv);
    QJSEngine engine;
    new extension::Console(&engine);
    QJSValue value(114514);
    QVariant v = engine.fromScriptValue<QVariant>(value);
    qDebug() << v;
    value = engine.toScriptValue(v);
    qDebug() << value.toString();
    return a.exec();
}