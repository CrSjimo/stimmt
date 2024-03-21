#ifndef STIMMT_QUICKWIDGET_P_H
#define STIMMT_QUICKWIDGET_P_H

#include <QQuickWidget>

#include <stimmt/WidgetWrapper.h>

namespace stimmt::widgets {

    class QuickWidgetQmlMessageHandler;

    class QuickWidget final : public WidgetWrapper {
        Q_OBJECT
        Q_PROPERTY(ResizeMode resizeMode READ resizeMode WRITE setResizeMode)
    public:
        Q_INVOKABLE QuickWidget();
        ~QuickWidget() override;

        enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView };
        Q_ENUM(ResizeMode)
        void setResizeMode(ResizeMode mode);
        ResizeMode resizeMode() const;

    public slots:
        void load(const QString &filePath);
        void sendMessage(const QString &name, const QJSValue &message);

    signals:
        void messageReceived(const QString &name, const QJSValue &message);

    private:
        friend class QuickWidgetQmlMessageHandler;
        inline QQuickWidget *quickWidget() const {
            return qobject_cast<QQuickWidget *>(wrappedObject());
        }

        QScopedPointer<QQmlEngine> m_qmlEngine;
        QuickWidgetQmlMessageHandler *m_messageHandler;
        QJSEngine *m_jsEngine = nullptr;
    };

    class QuickWidgetQmlMessageHandler : public QObject {
        Q_OBJECT
    public:
        explicit QuickWidgetQmlMessageHandler(QuickWidget *quickWidgetWrapper);
        ~QuickWidgetQmlMessageHandler() override;

    public slots:
        void sendMessage(const QString &name, const QJSValue &message);

    signals:
        void messageReceived(const QString &name, const QJSValue &message);

    private:
        QuickWidget *m_quickWidgetWrapper;
    };

}

#endif //STIMMT_QUICKWIDGET_P_H
