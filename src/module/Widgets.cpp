#include "Widgets.h"
#include "Widgets_p.h"

#include <QDomDocument>
#include <QJSEngine>

#include <stimmt/ClosureHelper.h>
#include <stimmt/ModuleSystem.h>
#include <stimmt/WidgetWrapper.h>

#include <stimmt/private/BoxLayout_p.h>
#include <stimmt/private/Button_p.h>
#include <stimmt/private/ComboBox_p.h>

namespace stimmt::modul {

    QJSValue WidgetsPrivate::convertDomToDesc(const QDomNode &node) const {
        Q_Q(const Widgets);
        auto engine = qjsEngine(q);
        Q_ASSERT(engine);

        QList<QJSValue> children;

        QDomNode childNode = node.firstChild();
        while (!childNode.isNull()) {
            if (childNode.isElement() || childNode.isText()) {
                auto child = convertDomToDesc(childNode);
                children.append(child);
            }
            childNode = childNode.nextSibling();
        }

        if (node.isElement()) {
            auto elementJson = engine->newObject();
            elementJson.setProperty("tagName", node.toElement().tagName());

            auto attributes = node.attributes();
            auto attributesJson = engine->newObject();
            for (int i = 0; i < attributes.count(); ++i) {
                QDomAttr attribute = attributes.item(i).toAttr();
                attributesJson.setProperty(attribute.name(), attribute.value());
            }
            elementJson.setProperty("attributes", attributesJson);
            elementJson.setProperty("children", engine->toScriptValue(children));
            return elementJson;
        } else {
            return node.nodeValue();
        }
    }

    Widgets::Widgets(QJSEngine *engine, QObject *parent) : Widgets(parent, *new WidgetsPrivate) {
        Q_D(Widgets);
        d->engine = engine;
        registerWidgetClass<widgets::VBoxLayout>();
        registerWidgetClass<widgets::HBoxLayout>();
        registerWidgetClass<widgets::PushButton>();
        registerWidgetClass<widgets::CheckBox>();
        registerWidgetClass<widgets::RadioButton>();
        registerWidgetClass<widgets::ComboBox>();
    }

    Widgets::~Widgets() = default;

    void Widgets::registerModule(ModuleSystem *moduleSystem, Widgets *moduleObject) {
        auto moduleObj = ClosureHelper::makeClosure(moduleSystem->engine(), moduleObject);
        for (const auto &widgetClassName : moduleObject->d_func()->moduleWidgetClasses.keys()) {
            moduleObj.setProperty(widgetClassName, moduleObject->d_func()->moduleWidgetClasses.value(widgetClassName));
        }
        moduleSystem->registerModule("stimmt:widgets", moduleObj);
    }

    QJSValue Widgets::render(const QJSValue &description) const {
        Q_D(const Widgets);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        auto tag = description.property("tagName").toString();
        auto obj = d->renderableWidgetClasses.value(tag);
        if (obj.isUndefined()) {
            engine->throwError(QJSValue::TypeError, QString("Invalid tag '%1'").arg(tag));
            return {};
        }
        obj = obj.callAsConstructor();
        auto widgetWrapper = qobject_cast<WidgetWrapper *>(obj.toQObject());
        Q_ASSERT(widgetWrapper);
        widgetWrapper->render(obj, description.property("attributes"), description.property("children"), this);
        return obj;
    }

    QJSValue Widgets::renderXml(const QString &xmlText) const {
        Q_D(const Widgets);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        QDomDocument doc;
        doc.setContent(xmlText);
        if (doc.documentElement().isNull()) {
            engine->throwError(QJSValue::TypeError, "Invalid XML document");
            return {};
        }
        return render(d->convertDomToDesc(doc.documentElement()));
    }

    Widgets::Widgets(QObject *parent, WidgetsPrivate &d) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
    }

    void Widgets::addWidgetClassImpl(const QMetaObject *metaObject, const QString &name) {
        Q_D(Widgets);
        auto constructor = d->engine->newQMetaObject(metaObject);
        d->renderableWidgetClasses.insert(name, constructor);
        d->moduleWidgetClasses.insert(name, constructor);
    }

    void Widgets::addExternalWidgetClass(const QJSValue &constructor, QString &name) {
        Q_D(Widgets);
        d->renderableWidgetClasses.insert(name, constructor);
    }
} // stimmt