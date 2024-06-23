#ifndef STIMMT_WATCHDOG_H
#define STIMMT_WATCHDOG_H

#include <QObject>

#include <stimmt/stimmtGlobal.h>

class QJSEngine;

namespace stimmt {

    class WatchdogPrivate;

    class STIMMT_EXPORT Watchdog final : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Watchdog)
    public:
        explicit Watchdog(const QString &programPath, QJSEngine *engine, QObject *parent = nullptr);
        ~Watchdog() override;

        bool start(int timeoutMsec);
        void stop();

    private:
        QScopedPointer<WatchdogPrivate> d_ptr;
    };

} // stimmt

#endif //STIMMT_WATCHDOG_H
