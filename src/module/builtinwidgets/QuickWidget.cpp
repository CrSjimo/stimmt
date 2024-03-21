#include "QuickWidget_p.h"

#include <QCoreApplication>
#include <QQmlApplicationEngine>

namespace stimmt::widgets {
    QuickWidget::QuickWidget() : WidgetWrapper("QuickWidget", new QQuickWidget(new QQmlApplicationEngine, nullptr)), m_messageHandler(new QuickWidgetQmlMessageHandler(this)) {
        m_qmlEngine.reset(quickWidget()->engine());
        m_qmlEngine->installExtensions(QJSEngine::TranslationExtension);
        m_qmlEngine->globalObject().setProperty("Stimmt", m_qmlEngine->newQObject(m_messageHandler));
        disconnect(m_qmlEngine.get(), nullptr, QCoreApplication::instance(), nullptr);
    }

    QuickWidget::~QuickWidget() = default;

    void QuickWidget::setResizeMode(QuickWidget::ResizeMode mode) {
        quickWidget()->setResizeMode(QQuickWidget::ResizeMode(mode));
    }

    QuickWidget::ResizeMode QuickWidget::resizeMode() const {
        return QuickWidget::ResizeMode(quickWidget()->resizeMode());
    }

    void QuickWidget::load(const QString &filePath) {
        m_jsEngine = qjsEngine(this);
        Q_ASSERT(m_jsEngine);
        quickWidget()->setSource(QUrl::fromLocalFile(filePath));
    }

    void QuickWidget::sendMessage(const QString &name, const QJSValue &message) {
        Q_ASSERT(m_jsEngine);
        auto packedMessage = m_jsEngine->fromScriptValue<QVariant>(message);
        auto qmlEngineJSValue = m_qmlEngine->toScriptValue(packedMessage);
        emit m_messageHandler->messageReceived(name, qmlEngineJSValue);
    }

    QuickWidgetQmlMessageHandler::QuickWidgetQmlMessageHandler(QuickWidget *quickWidgetWrapper) : m_quickWidgetWrapper(quickWidgetWrapper) {
    }

    QuickWidgetQmlMessageHandler::~QuickWidgetQmlMessageHandler() = default;

    void QuickWidgetQmlMessageHandler::sendMessage(const QString &name, const QJSValue &message) {
        auto packedMessage = m_quickWidgetWrapper->m_qmlEngine->fromScriptValue<QVariant>(message);
        if (!m_quickWidgetWrapper->m_jsEngine)
            return;
        auto jsEngineJSValue = m_quickWidgetWrapper->m_jsEngine->toScriptValue(packedMessage);
        emit m_quickWidgetWrapper->messageReceived(name, jsEngineJSValue);
    }
} // stimmt