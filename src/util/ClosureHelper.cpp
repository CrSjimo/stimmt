#include "ClosureHelper.h"

#include <QMetaMethod>
#include <QJSEngine>

namespace stimmt {
    QJSValue ClosureHelper::makeClosure(QJSEngine *engine, QObject *o) {
        QStringList methodList;
        for (int i = o->metaObject()->methodOffset(); i < o->metaObject()->methodCount(); i++)
            methodList.append(o->metaObject()->method(i).name());
        QString sourceCode = "(a)=>{let b={};";
        for (const auto &methodName : methodList) {
            sourceCode += QString("b.%1=(...c)=>a.%1(...c);").arg(methodName);
        }
        sourceCode += "return b;}";
        return engine->evaluate(sourceCode).call({engine->newQObject(o)});
    }
} // stimmt