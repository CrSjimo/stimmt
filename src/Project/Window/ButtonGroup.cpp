#include "ButtonGroup.h"

#include "Global/GlobalObject.h"
#include "Project/ProjectWindowObject.h"
#include "ObjectWrapper.h"
#include <QAbstractButton>
#include <QJSEngine>

namespace stimmt {
    ButtonGroup::ButtonGroup(QObject * parent) : QButtonGroup(parent) {
    }

    void ButtonGroup::addButton(const QJSValue &jsButton, int id) {
        auto btn = ObjectWrapper::getOfWrappedObject<QAbstractButton>(jsButton);
        if (btn) {
            QButtonGroup::addButton(btn, id);
            return;
        }
        JS_THROW(QJSValue::TypeError, "Invalid type of button");
    }

    int ButtonGroup::checkedId() const {
        return QButtonGroup::checkedId();
    }

    int ButtonGroup::id(const QJSValue &jsButton) const {
        return 0;
    }

    void ButtonGroup::removeButton(const QJSValue &jsButton) {
    }

    void ButtonGroup::setId(const QJSValue &jsButton, int id) {
    }
}