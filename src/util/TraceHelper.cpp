#include "TraceHelper.h"

#include <QJSEngine>

namespace stimmt {

    QString TraceHelper::stackTrace(QJSEngine *engine, int depth) {
        return engine->evaluate("throw new Error()")
                .property("stack")
                .toString()
                .split("\n")
                .mid(1 + depth)
                .join("\n");
    }

    QString TraceHelper::fileTrace(QJSEngine *engine, int depth) {
        auto stack = stackTrace(engine).split("\n")[depth];
        return stack.mid(stack.indexOf('@') + 1);
    }

} // stimmt