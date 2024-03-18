#ifndef STIMMT_WIDGETS_P_H
#define STIMMT_WIDGETS_P_H

#include <QDomNode>
#include <QMap>

#include <stimmt/Widgets.h>

namespace stimmt::modul {
    class WidgetsPrivate {
        Q_DECLARE_PUBLIC(Widgets)
    public:
        Widgets *q_ptr;
        QJSEngine *engine;
        QMap<QString, QJSValue> moduleWidgetClasses;
        QMap<QString, QJSValue> renderableWidgetClasses;

        QJSValue convertDomToDesc(const QDomNode &node) const;
    };
}

#endif //STIMMT_WIDGETS_P_H
