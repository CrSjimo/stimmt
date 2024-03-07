#include "Timer.h"

#include <QJSEngine>
#include <QTimer>

namespace stimmt::extension {
    Timer::Timer(QJSEngine *engine, QObject *parent) : QObject(parent) {
        engine->evaluate(R"js(
            (t, global) => {
                global.setTimeout = function setTimeout(f, delay = 0, ...args) {
                    let timer = t.createTimer(delay, true);
                    timer.timeout.connect(() => {
                        if (typeof(f) === 'string')
                            eval(f);
                        else
                            f(...args);
                        t.clearTimer(timer);
                    });
                    return t.timerId(timer);

                };
                global.setInterval = function setInterval(f, delay = 0, ...args) {
                    let timer = t.createTimer(delay, false);
                    timer.timeout.connect(() => {
                        if (typeof(f) === 'string')
                            eval(f);
                        else
                            f(...args);
                    });
                    return t.timerId(timer);
                };
                global.clearTimeout = function clearTimeout(id) {
                    t.clearTimer(id);
                };
                global.clearInterval = function clearInterval(id) {
                    t.clearTimer(id);
                };
            })js").call({engine->newQObject(this), engine->globalObject()});
    }

    Timer::~Timer() = default;

    QHash<int, QTimer *> m_timers;

    QObject *Timer::createTimer(int delay, bool isSingleShot) {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        auto timer = new QTimer(this);
        timer->setInterval(delay);
        timer->setSingleShot(isSingleShot);
        timer->start();
        m_timers.insert(timer->timerId(), timer);
        return timer;
    }

    void Timer::clearTimer(int id) {
        if (m_timers.contains(id)) {
            auto timer = m_timers.value(id);
            m_timers.remove(id);
            timer->stop();
            timer->deleteLater();
        }
    }

    void Timer::clearTimer(QObject *object) {
        auto timer = qobject_cast<QTimer *>(object);
        clearTimer(timer->timerId());
    }

    int Timer::timerId(QObject *object) {
        auto timer = qobject_cast<QTimer *>(object);
        return timer->timerId();
    }
} // stimmt::extension