#ifndef STIMMT_TYPEDARRAYHELPER_H
#define STIMMT_TYPEDARRAYHELPER_H

#include <QByteArray>
#include <QJSValue>

#include <stimmt/stimmtGlobal.h>

class QJSEngine;

namespace stimmt {

    class STIMMT_EXPORT TypedArrayHelper {
    public:
        static QByteArray fromGenericValue(QJSEngine *engine, const QJSValue &value);
        static QJSValue toUint8Array(QJSEngine *engine, const QByteArray &byteArray);
    };

} // stimmt

#endif //STIMMT_TYPEDARRAYHELPER_H
