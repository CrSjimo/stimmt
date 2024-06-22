#ifndef STIMMT_QUICK_P_H
#define STIMMT_QUICK_P_H

#include <stimmt/Quick.h>

#include <QPointer>

class QQmlEngine;
class QQuickWidget;

namespace stimmt::modul {

    class QmlMessageHandler : public QObject {
        Q_OBJECT
    public:
        explicit QmlMessageHandler(QuickPrivate *d, QObject *parent = nullptr);
        ~QmlMessageHandler() override;

    public slots:
        QJSValue import(const QString &moduleName);
        void sendMessage(const QString &name, const QJSValue &message);

    signals:
        void messageReceived(const QString &name, const QJSValue &message);

    private:
        QuickPrivate *d;
    };

    class QuickPrivate {
        Q_DECLARE_PUBLIC(Quick)
    public:
        Quick *q_ptr;
        QPointer<QQuickWidget> widget;
        QScopedPointer<QQmlEngine> qmlEngine;
        QmlMessageHandler *messageHandler;
        ModuleSystem *moduleSystem = nullptr;

    };

}

#endif //STIMMT_QUICK_P_H
