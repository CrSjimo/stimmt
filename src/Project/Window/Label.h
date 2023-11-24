#ifndef STIMMT_LABEL_H
#define STIMMT_LABEL_H

#include <QLabel>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class Label : public QLabel, public ScriptDescriptiveObject {
        Q_OBJECT
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;
        explicit Label(QWidget *parent = nullptr);
    };
}

#endif // STIMMT_LABEL_H
