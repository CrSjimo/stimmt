#include "TypedArrayHelper.h"

#include <QJSEngine>

namespace stimmt {

    QByteArray TypedArrayHelper::fromGenericValue(QJSEngine *engine, const QJSValue &value) {
        QJSValue data = value;
        if (engine->globalObject().property("ArrayBuffer").property("isView").call({value}).toBool()) {
            data = value.property("buffer");
        }
        return engine->fromScriptValue<QByteArray>(data);
    }

    QJSValue TypedArrayHelper::toUint8Array(QJSEngine *engine, const QByteArray &byteArray) {
        return engine->globalObject().property("Uint8Array").callAsConstructor({engine->toScriptValue(byteArray)});
    }

} // stimmt