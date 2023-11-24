#ifndef STIMMT_JAVASCRIPTDEBUGGINGTOOLSWINDOW_H
#define STIMMT_JAVASCRIPTDEBUGGINGTOOLSWINDOW_H

#include <QMainWindow>

#include <stimmt/StimmtGlobal.h>

namespace stimmt {
    class STIMMT_EXPORT JavaScriptDebuggingToolsWindow : public QMainWindow {
        Q_OBJECT
    public:
        explicit JavaScriptDebuggingToolsWindow(QWidget *parent = nullptr);
    };
}

#endif // STIMMT_JAVASCRIPTDEBUGGINGTOOLSWINDOW_H
