#include <QApplication>
#include <QMainWindow>
#include <QJSEngine>
#include <QDebug>

#include <QDir>
#include <QUrl>

#include <stimmt/ModuleSystem.h>
#include <stimmt/Console.h>
#include <stimmt/ClosureHelper.h>
#include <stimmt/FileSystem.h>

#include <stimmt/private/BoxLayout_p.h>
#include <stimmt/private/Button_p.h>

using namespace stimmt;

class MyObject : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE MyObject() {
        qDebug() << qjsEngine(this);
    }
    ~MyObject() override = default;

    Q_INVOKABLE void f() {
        qDebug() << qjsEngine(this);
    }
};

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QJSEngine engine;
    new extension::Console(&engine);
    engine.globalObject().setProperty("VBoxLayout", engine.newQMetaObject<widgets::VBoxLayout>());
    engine.globalObject().setProperty("PushButton", engine.newQMetaObject<widgets::PushButton>());
    auto ret = engine.evaluate("() => {let layout = new VBoxLayout(); let button = new PushButton(); button.text = 'aaa'; layout.addWidget(button); return layout}");
    qDebug() << ret.toString();
    ret = ret.call();
    qDebug() << ret.toString();
    QMainWindow win;
    auto mainWidget = new QWidget;
    mainWidget->setLayout(qobject_cast<QLayout *>(WidgetWrapper::unwrapFromJSObject(ret)));
    win.setCentralWidget(mainWidget);
    win.show();
    return a.exec();
}

#include "main.moc"