#ifndef STIMMT_DOUBLESPINBOX_H
#define STIMMT_DOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class DoubleSpinBox : public QDoubleSpinBox, public ScriptDescriptiveObject {
        Q_OBJECT
    public:
        QJSValue createScriptObject() override;

        explicit DoubleSpinBox(QWidget *parent = nullptr);
    };
}

#endif // STIMMT_DOUBLESPINBOX_H
