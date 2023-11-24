#ifndef STIMMT_CHECKBOX_H
#define STIMMT_CHECKBOX_H

#include <QCheckBox>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class CheckBox : public QCheckBox, public ScriptDescriptiveObject {
        Q_OBJECT
        Q_PROPERTY(int checkState READ checkState WRITE setCheckState)
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;

        explicit CheckBox(QWidget *parent = nullptr);
        void setCheckState(int state);
        int checkState() const;
    };
}

#endif // STIMMT_CHECKBOX_H
