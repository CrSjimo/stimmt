#ifndef STIMMT_WATCHDOG_P_H
#define STIMMT_WATCHDOG_P_H

#include <stimmt/Watchdog.h>

#include <QProcess>
#include <QThread>
#include <QSharedMemory>

class QTimer;

namespace stimmt {

    class WatchdogPrivate {
        Q_DECLARE_PUBLIC(Watchdog)
    public:
        enum WatchDogOperation {
            CloseNotification,
            OpenNotification,
            Interrupt,
        };
        struct Data {
            char operation;
            char flag;
        };
        Watchdog *q_ptr;
        QProcess watchdogProcess;
        QJSEngine *engine;
        QThread workerThread;
        QTimer *timer = nullptr;
        QSharedMemory sharedMemory;
        Data *data = nullptr;
        int counter = 0;
    };
}

#endif //STIMMT_WATCHDOG_P_H
