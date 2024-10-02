#ifndef STIMMT_TRACEHELPER_H
#define STIMMT_TRACEHELPER_H

#include <QString>

#include <stimmt/stimmtGlobal.h>

class QJSEngine;

namespace stimmt {

    class STIMMT_EXPORT TraceHelper {
    public:
        static QString stackTrace(QJSEngine *engine, int depth = 0);
        static QString fileTrace(QJSEngine *engine, int depth = 0);
    };

} // stimmt

#endif //STIMMT_TRACEHELPER_H
