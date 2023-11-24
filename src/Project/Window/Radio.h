#ifndef STIMMT_RADIO_H
#define STIMMT_RADIO_H

#include <QRadioButton>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class Radio : public QRadioButton, public ScriptDescriptiveObject {
        Q_OBJECT
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;
        explicit Radio(QWidget *parent = nullptr);
    };
}

#endif // STIMMT_RADIO_H
