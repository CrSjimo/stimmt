#include <QApplication>
#include <QJSEngine>
#include <QDialog>
#include <QVBoxLayout>
#include <QDebug>

#include <stimmt/ModuleSystem.h>
#include <stimmt/Console.h>
#include <stimmt/View.h>
#include <stimmt/BasicForm.h>
#include <stimmt/Quick.h>

using namespace stimmt;


int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QJSEngine engine;
    new extension::Console(&engine);
    ModuleSystem ms(&engine);
    modul::View::registerModule(&ms);
    auto obj = ms.loadModule(":/index.js");
    auto content = qobject_cast<stimmt::modul::BasicForm *>(obj.property("form").toQObject());
    auto quickContent = qobject_cast<stimmt::modul::Quick *>(obj.property("quick").toQObject());

    QDialog dlg;
    auto layout = new QVBoxLayout;
    layout->addWidget(content->wrappedWidget());
    layout->addWidget(quickContent->wrappedWidget());
    dlg.setLayout(layout);
    dlg.show();

    QObject::connect(content, &modul::BasicForm::rowValueChanged, [](const QString &id, const QJSValue &value) {
        qDebug() << id << value.toString();
    });

    QObject::connect(content, &modul::ViewContent::finished, &dlg, &QDialog::done);



    return a.exec();
}

#include "main.moc"