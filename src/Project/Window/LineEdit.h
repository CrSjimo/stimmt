#ifndef STIMMT_LINEEDIT_H
#define STIMMT_LINEEDIT_H

#include <QLineEdit>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class LineEdit : public QLineEdit, public ScriptDescriptiveObject {
        Q_OBJECT
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;
        explicit LineEdit(QWidget *parent = nullptr);
    };
}

#endif // STIMMT_LINEEDIT_H
