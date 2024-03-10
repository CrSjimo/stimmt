#include "BoxLayout_p.h"

#include <QJSEngine>
#include <QWidget>

namespace stimmt::widgets {
    void BoxLayout::render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule) {
        WidgetWrapper::render(wrappedObject, attributes, children, widgetsModule);
    }

    QJSValue BoxLayout::children() const {
        return m_children;
    }

    BoxLayout::BoxLayout(const QString &tagName, QBoxLayout *layout) : WidgetWrapper(tagName, layout) {
    }

    BoxLayout::~BoxLayout() = default;

    int BoxLayout::count() {
        return boxLayout()->count();
    }

    void BoxLayout::addWidget(const QJSValue &jsWidget, int stretch, int alignment) {
        insertWidget(-1, jsWidget, stretch, alignment);
    }

    void BoxLayout::addSpacing(int size) {
        boxLayout()->addSpacing(size);
    }

    void BoxLayout::addStretch(int stretch) {
        boxLayout()->addStretch(stretch);
    }

    void BoxLayout::addStrut(int size) {
        boxLayout()->addStrut(size);
    }

    void BoxLayout::insertWidget(int index, const QJSValue &jsWidget, int stretch, int alignment) {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (m_childrenInsertFunction.isUndefined())
            m_childrenInsertFunction = engine->evaluate("(children, index, object) => (index < 0 ? children.push(object) : children.splice(index, 0, object))");
        auto wrappedObj = WidgetWrapper::unwrapFromJSObject(jsWidget);
        if (auto widget = qobject_cast<QWidget *>(wrappedObj))
            boxLayout()->insertWidget(index, widget, stretch, Qt::Alignment(alignment));
        else if (auto layout = qobject_cast<QLayout *>(wrappedObj))
            boxLayout()->insertLayout(index, layout, stretch);
        else
            return engine->throwError(QJSValue::TypeError, "Invalid argument type");
        m_childrenInsertFunction.call({m_children, index, jsWidget});

    }

    void BoxLayout::insertSpacing(int index, int size) {
        boxLayout()->insertSpacing(index, size);
    }

    void BoxLayout::insertStretch(int index, int stretch) {
        boxLayout()->insertStretch(index, stretch);
    }

    HBoxLayout::HBoxLayout() : BoxLayout("HBoxLayout", new QHBoxLayout) {
    }

    HBoxLayout::~HBoxLayout() = default;

    VBoxLayout::VBoxLayout() : BoxLayout("VBoxLayout", new QVBoxLayout) {
    }

    VBoxLayout::~VBoxLayout() = default;
} // stimmt::widgets