#ifndef STIMMT_TIMER_H
#define STIMMT_TIMER_H

#include <QObject>
#include <QJSValue>

#include <stimmt/stimmtGlobal.h>

class QJSEngine;

namespace stimmt::extension {

    class STIMMT_EXPORT Timer : public QObject {
        Q_OBJECT
    public:
        explicit Timer(QJSEngine *engine, QObject *parent = nullptr);
        ~Timer() override;

    public slots:
        QObject *createTimer(int delay, bool isSingleShot);
        void clearTimer(int id);
        void clearTimer(QObject *timer);
        int timerId(QObject *timer);
    };

} // stimmt::extension

#endif //STIMMT_TIMER_H
