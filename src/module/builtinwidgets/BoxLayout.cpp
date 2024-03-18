#include "BoxLayout_p.h"

#include <QJSEngine>
#include <QWidget>
#include <QMetaEnum>

#include <stimmt/Widgets.h>

namespace stimmt::widgets {
    void BoxLayout::render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule) {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        WidgetWrapper::render(wrappedObject, attributes, children, widgetsModule);
        for (int i = 0; i < children.property("length").toInt(); i++) {
            auto child = children.property(i);
            if (child.isString()) {
                auto text = child.toString();
                child = engine->newObject();
                child.setProperty("tagName", "Label");
                child.setProperty("children", engine->toScriptValue(QStringList{text}));
            }
            auto tagName = child.property("tagName").toString();
            if (tagName == "BoxLayout-Stretch") {
                auto stretch = child.property("attributes").property("stretch");
                addStretch(stretch.toInt());
            } else {
                int stretch = 0;
                if (child.property("attributes").hasProperty("BoxLayout-stretch")) {
                    stretch = child.property("attributes").property("BoxLayout-stretch").toInt();
                }
                int alignment = Qt::Alignment();
                if (child.property("attributes").hasProperty("BoxLayout-alignment")) {
                    auto alignmentText = child.property("attributes").property("BoxLayout-alignment").toString();
                    alignment = QMetaEnum::fromType<AlignmentFlag>().keysToValue(alignmentText.toUtf8());
                }
                addWidget(widgetsModule->render(child), stretch, alignment);
            }
        }
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

    void BoxLayout::addStretch(int stretch) {
        boxLayout()->addStretch(stretch);
    }

    void BoxLayout::insertWidget(int index, const QJSValue &jsWidget, int stretch, int alignment) {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (m_children.isUndefined())
            m_children = engine->newArray();
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