#include "Watchdog.h"
#include "Watchdog_p.h"

#include <QTimer>
#include <QUuid>
#include <QJSEngine>

namespace stimmt {
    Watchdog::Watchdog(const QString &programPath, QJSEngine *engine, QObject *parent) : QObject(parent), d_ptr(new WatchdogPrivate) {
        Q_D(Watchdog);
        d->q_ptr = this;
        d->engine = engine;
        d->sharedMemory.setKey(QUuid::createUuid().toString(QUuid::WithoutBraces));
        if (d->sharedMemory.create(sizeof(WatchdogPrivate::Data)))
            d->data = reinterpret_cast<WatchdogPrivate::Data *>(d->sharedMemory.data());
        d->watchdogProcess.setProcessChannelMode(QProcess::ForwardedChannels);
        d->watchdogProcess.setProgram(programPath);
        d->watchdogProcess.setArguments({d->sharedMemory.key()});
    }

    Watchdog::~Watchdog() {
        Q_D(Watchdog);
        stop();
    }

    bool Watchdog::start(int timeoutMsec) {
        Q_D(Watchdog);
        if (!d->sharedMemory.isAttached())
            return false;
        d->watchdogProcess.start();
        if (!d->watchdogProcess.waitForStarted())
            return false;
        d->timer = new QTimer;
        d->timer->setInterval(1000);
        d->timer->moveToThread(&d->workerThread);
        connect(d->timer, &QTimer::timeout, this, [=] {
            if (d->data->operation == WatchdogPrivate::Interrupt) {
                d->engine->setInterrupted(true);
                return;
            }
            d->counter++;
            d->data->flag++;
            if (1000 * d->counter >= timeoutMsec) {
                d->data->operation = WatchdogPrivate::OpenNotification;
                d->counter = 0;
            }
        }, Qt::DirectConnection);
        connect(&d->workerThread, &QThread::started, d->timer, QOverload<>::of(&QTimer::start));
        d->workerThread.start();
        return true;
    }

    void Watchdog::stop() {
        Q_D(Watchdog);
        if (d->data)
            d->data->operation = WatchdogPrivate::CloseNotification;
        d->workerThread.quit();
        d->watchdogProcess.kill();
        d->workerThread.wait();
        d->watchdogProcess.waitForFinished();
        if (d->timer) {
            d->timer->deleteLater();
        }
        d->counter = 0;
        d->engine->setInterrupted(false);
    }
} // stimmt