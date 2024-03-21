#include <QApplication>
#include <QMainWindow>
#include <QJSEngine>
#include <QDebug>
#include <QLayout>
#include <QDir>
#include <QUrl>
#include <QComboBox>
#include <QQuickWidget>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QPushButton>

#include <stimmt/ModuleSystem.h>
#include <stimmt/Console.h>
#include <stimmt/ClosureHelper.h>
#include <stimmt/FileSystem.h>
#include <stimmt/Widgets.h>
#include <stimmt/WidgetWrapper.h>

using namespace stimmt;


int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QJSEngine engine;
    new extension::Console(&engine);
    ModuleSystem ms(&engine);
    modul::Widgets::registerModule(&ms, new modul::Widgets(&engine));
    auto renderXmlFunc = ms.loadModule("stimmt:widgets").property("renderXml");
    auto quickWidgetJSObj = renderXmlFunc.call({R"xml(<QuickWidget/>)xml"});
    auto widgetWrapper = qobject_cast<WidgetWrapper *>(quickWidgetJSObj.toQObject());
    qDebug() << widgetWrapper;
    auto quickWidget = qobject_cast<QQuickWidget *>(widgetWrapper->wrappedObject());
    QMainWindow win;
    win.setCentralWidget(quickWidget);
    engine.evaluate(R"js(w => w.load('C:/Qt/Examples/Qt-5.15.2/tutorials/alarms/main.qml'))js").call({quickWidgetJSObj});
    engine.evaluate(R"js(w => w.messageReceived.connect((name, message) => console.log(name, message)))js").call({quickWidgetJSObj});
    win.show();
    return a.exec();
}

#include "main.moc"