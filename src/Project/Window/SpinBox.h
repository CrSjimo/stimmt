#ifndef STIMMT_SPINBOX_H
#define STIMMT_SPINBOX_H

#include <QSpinBox>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class SpinBox : public QSpinBox, public ScriptDescriptiveObject {
        Q_OBJECT
    public:
        QJSValue createScriptObject() override;
        explicit SpinBox(QWidget *parent = nullptr);
    };
}

#endif // STIMMT_SPINBOX_H
