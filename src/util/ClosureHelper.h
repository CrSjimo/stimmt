#ifndef STIMMT_CLOSUREHELPER_H
#define STIMMT_CLOSUREHELPER_H

#include <QJSValue>

#include <stimmt/stimmtGlobal.h>

class QObject;
class QJSEngine;

namespace stimmt {

    class STIMMT_EXPORT ClosureHelper {
    public:
        static QJSValue makeClosure(QJSEngine *engine, QObject *o);
    };

} // stimmt

#endif //STIMMT_CLOSUREHELPER_H
