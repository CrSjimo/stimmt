#include "Quick.h"
#include "Quick_p.h"

#include <QQmlApplicationEngine>
#include <QQuickWidget>
#include <QCoreApplication>

#include <stimmt/ModuleSystem.h>
#include <stimmt/Console.h>
#include <stimmt/Codec.h>
#include <stimmt/Timer.h>

namespace stimmt::modul {
    Quick::Quick() {
        Q_D(Quick);
        d->q_ptr = this;
        d->widget = new QQuickWidget(new QQmlApplicationEngine, nullptr);
        d->messageHandler = new QmlMessageHandler(d, this);
        d->qmlEngine.reset(d->widget->engine());
        d->qmlEngine->installExtensions(QJSEngine::TranslationExtension);
        new extension::Console(d->qmlEngine.get());
        new extension::Codec(d->qmlEngine.get());
        new extension::Timer(d->qmlEngine.get());
        d->qmlEngine->globalObject().setProperty("Stimmt", d->qmlEngine->newQObject(d->messageHandler));
        disconnect(d->qmlEngine.get(), nullptr, QCoreApplication::instance(), nullptr);
    }

    Quick::~Quick() {
        Q_D(Quick);
        delete d->widget.data();
    }

    QWidget *Quick::wrappedWidget() const {
        Q_D(const Quick);
        return d->widget;
    }

    void Quick::setResizeMode(Quick::ResizeMode mode) {
        Q_D(Quick);
        d->widget->setResizeMode(QQuickWidget::ResizeMode(mode));
    }

    Quick::ResizeMode Quick::resizeMode() const {
        Q_D(const Quick);
        return ResizeMode(d->widget->resizeMode());
    }

    static Quick::ModuleSystemResolver m_resolver = [](QJSEngine *) {
        return nullptr;
    };

    void Quick::setModuleSystemResolver(const Quick::ModuleSystemResolver &resolver) {
        m_resolver = resolver;
    }

    void Quick::load(const QString &filePath) {
        Q_D(Quick);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        d->widget->setSource(QUrl::fromLocalFile(filePath));
        d->moduleSystem = m_resolver(engine);
    }

    void Quick::sendMessage(const QString &name, const QJSValue &message) {
        Q_D(Quick);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        auto packedMessage = engine->fromScriptValue<QVariant>(message);
        auto qmlEngineJSValue = d->qmlEngine->toScriptValue(packedMessage);
        emit d->messageHandler->messageReceived(name, qmlEngineJSValue);
    }

    QmlMessageHandler::QmlMessageHandler(QuickPrivate *d, QObject *parent) : QObject(parent), d(d) {
    }

    QmlMessageHandler::~QmlMessageHandler() = default;

    QJSValue QmlMessageHandler::import(const QString &moduleName) {
        if (d->moduleSystem) {
            return d->moduleSystem->loadModule(moduleName);
        } else {
            auto engine = qmlEngine(this);
            engine->throwError("Module is not available");
            return {};
        }
    }

    void QmlMessageHandler::sendMessage(const QString &name, const QJSValue &message) {
        auto packedMessage = d->qmlEngine->fromScriptValue<QVariant>(message);
        auto engine = qjsEngine(d->q_ptr);
        Q_ASSERT(engine);
        auto jsEngineJSValue = engine->toScriptValue(packedMessage);
        emit d->q_ptr->messageReceived(name, jsEngineJSValue);
    }


}