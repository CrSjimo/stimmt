#ifndef STIMMT_WIDGETWRAPPER_P_H
#define STIMMT_WIDGETWRAPPER_P_H

#include <QPointer>

#include <stimmt/WidgetWrapper.h>

namespace stimmt {
    class WidgetWrapperPrivate {
        Q_DECLARE_PUBLIC(WidgetWrapper)
    public:
        WidgetWrapper *q_ptr;
        QPointer<QObject> wrappedObject;
        QString id;
        QString tagName;

        QJSValue findChildByIdJSFunction;
        QJSValue findChildrenByTagNameJSFunction;
        QJSValue renderJSFunction;
    };
}

#endif //STIMMT_WIDGETWRAPPER_P_H
