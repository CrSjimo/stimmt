#ifndef STIMMT_SCRIPTOBJECT_H
#define STIMMT_SCRIPTOBJECT_H

#include <QJSValue>

#include <stimmt/StimmtGlobal.h>

namespace stimmt {
    class STIMMT_EXPORT ScriptObject {
    public:
        virtual QJSValue createScriptObject() = 0;
    };

    class STIMMT_EXPORT ScriptDescriptiveObject : public ScriptObject {
    public:
        virtual void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                            const QJSValue &children,
                                                            const std::function<QJSValue(const QJSValue &)> &renderer);
    };
}

#endif // STIMMT_SCRIPTOBJECT_H
