#ifndef STIMMT_BUTTON_H
#define STIMMT_BUTTON_H

#include <QPushButton>
#include <stimmt/ScriptObject.h>

namespace stimmt {
    class Button : public QPushButton, public ScriptDescriptiveObject {
        Q_OBJECT
    public:
        explicit Button(QWidget *parent = nullptr);

        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;
    };
}

#endif // STIMMT_BUTTON_H
