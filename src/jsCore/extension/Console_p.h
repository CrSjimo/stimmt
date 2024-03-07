#ifndef STIMMT_CONSOLE_P_H
#define STIMMT_CONSOLE_P_H

#include <stimmt/Console.h>

namespace stimmt::extension {

    class ConsolePrivate {
    public:
        Q_DECLARE_PUBLIC(Console);
        Console *q_ptr;
        QMap<QString, int> counters;
        QMap<QString, quint64> timers;
        QJSEngine *engine;

        void install();
    };

}

#endif //STIMMT_CONSOLE_P_H
