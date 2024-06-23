#include <QApplication>
#include <QMainWindow>
#include <QJSEngine>
#include <QVBoxLayout>
#include <QPushButton>

#include <stimmt/JavaScriptOutputWidget.h>
#include <stimmt/Console.h>

using namespace stimmt;

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    QMainWindow win;
    QJSEngine engine;

    auto console = new extension::Console(&engine);
    auto outputWidget = new JavaScriptOutputWidget(console);

    auto mainWidget = new QWidget;
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(outputWidget);
    auto button = new QPushButton("Test");
    mainWidget->setLayout(mainLayout);
    mainLayout->addWidget(button);

    win.setCentralWidget(mainWidget);

    QObject::connect(button, &QPushButton::clicked, [&] {
        engine.evaluate(R"js(
            console.debug("test debug");
            console.log("test log");
            console.info("test info");
            console.warn("test warning");
            console.error("test error");
            console.trace("test trace");
        )js");
    });

    win.show();

    return a.exec();
}
