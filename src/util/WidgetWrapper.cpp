#include "WidgetWrapper.h"
#include "WidgetWrapper_p.h"

#include <QJSEngine>

namespace stimmt {
    WidgetWrapper::WidgetWrapper(const QString &tagName, QObject *wrappedObject, QObject *parent) : WidgetWrapper(parent, *new WidgetWrapperPrivate) {
        Q_D(WidgetWrapper);
        d->tagName = tagName;
        d->wrappedObject = wrappedObject;
    }

    WidgetWrapper::~WidgetWrapper() {
        Q_D(WidgetWrapper);
        if (!d->wrappedObject.isNull())
            d->wrappedObject->deleteLater();
    }

    QObject *WidgetWrapper::wrappedObject() const {
        Q_D(const WidgetWrapper);
        return d->wrappedObject;
    }

    void WidgetWrapper::render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule) {
        Q_D(WidgetWrapper);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (d->renderJSFunction.isUndefined()) {
            d->renderJSFunction = engine->evaluate(R"js((obj, attributes) => {
                for (let key of Object.keys(attributes)) {
                    obj[key] = attributes[key];
                }
            })js");
        }
        d->renderJSFunction.call({wrappedObject, attributes});
    }

    QString WidgetWrapper::id() const {
        Q_D(const WidgetWrapper);
        return d->id;
    }

    void WidgetWrapper::setId(const QString &id) {
        Q_D(WidgetWrapper);
        d->id = id;
    }

    QString WidgetWrapper::tagName() const {
        Q_D(const WidgetWrapper);
        return d->tagName;
    }

    QJSValue WidgetWrapper::children() const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        return engine->newArray();
    }

    QJSValue WidgetWrapper::findChildById(const QString &id) {
        Q_D(WidgetWrapper);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (d->findChildByIdJSFunction.isUndefined()) {
            d->findChildByIdJSFunction = engine->evaluate(R"js((children, id) => {
                for (let child of children) {
                    if (child.id == id)
                        return child;
                    let o = child.findChildById(id);
                    if (o)
                        return o;
                }
                return undefined;
            })js");
        }
        return d->findChildByIdJSFunction.call({children(), id});
    }

    QJSValue WidgetWrapper::findChildrenByTagName(const QString &tagName) {
        Q_D(WidgetWrapper);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (d->findChildrenByTagNameJSFunction.isUndefined()) {
            d->findChildrenByTagNameJSFunction = engine->evaluate(R"js((children, tagName) => {
                let objFound = [];
                for (let child of children) {
                    if (child.id == id)
                        objFound.push(child);
                    let o = child.findChildrenByTagName(id);
                    objFound.push(...o);
                }
                return objFound;
            })js");
        }
        return d->findChildrenByTagNameJSFunction.call({children(), tagName});
    }

    WidgetWrapper::WidgetWrapper(QObject *parent, WidgetWrapperPrivate &d) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
    }
} // stimmt