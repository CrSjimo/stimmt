#include <QApplication>
#include <QMainWindow>
#include <QJSEngine>
#include <QDebug>
#include <QLayout>
#include <QDir>
#include <QUrl>
#include <QComboBox>

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
    auto layoutJSObj = renderXmlFunc.call({R"xml(
        <VBoxLayout>
            <PushButton>aaa</PushButton>
            <CheckBox text="bbb"/>
            <ComboBox id="test">
                <ComboBox-Item>1</ComboBox-Item>
                <ComboBox-Item>2</ComboBox-Item>
            </ComboBox>
        </VBoxLayout>)xml"});
    auto widgetWrapper = qobject_cast<WidgetWrapper *>(layoutJSObj.toQObject());
    auto comboBox = qobject_cast<QComboBox *>(WidgetWrapper::unwrapFromJSObject(widgetWrapper->findChildById("test")));
    comboBox->addItem("newly added");
    QMainWindow win;
    auto mainWidget = new QWidget;
    mainWidget->setLayout(qobject_cast<QLayout *>(WidgetWrapper::unwrapFromJSObject(layoutJSObj)));
    win.setCentralWidget(mainWidget);
    win.show();
    return a.exec();
}

#include "main.moc"