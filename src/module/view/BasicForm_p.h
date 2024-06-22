#ifndef STIMMT_BASICFORM_P_H
#define STIMMT_BASICFORM_P_H

#include <stimmt/BasicForm.h>

#include <QPointer>

class QFormLayout;

namespace stimmt::modul {
    class BasicFormPrivate {
        Q_DECLARE_PUBLIC(BasicForm)
    public:
        BasicForm *q_ptr;
        QPointer<QWidget> widget;
        QFormLayout *formLayout;
        QJSValue aboutToCompleteCallback;
        QHash<QString, BasicFormRenderedWidget *> rows;
    };
}

#endif //STIMMT_BASICFORM_P_H
