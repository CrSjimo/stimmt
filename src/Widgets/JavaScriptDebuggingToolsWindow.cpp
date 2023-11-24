#include "JavaScriptDebuggingToolsWindow.h"

#include <QTabWidget>

#include "JavaScriptOutputWidget.h"

namespace stimmt {
    JavaScriptDebuggingToolsWindow::JavaScriptDebuggingToolsWindow(QWidget * parent) : QMainWindow(parent) {
        setWindowTitle(tr("JavaScript Debugging Tools"));
        auto tabWidget = new QTabWidget;
        setCentralWidget(tabWidget);
        tabWidget->addTab(new JavaScriptOutputWidget, tr("Output"));
        tabWidget->addTab(new QWidget, tr("Script Inspector"));
        tabWidget->addTab(new QWidget, tr("Storage Inspector"));
    }
}
