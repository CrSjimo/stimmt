#include "Storage.h"
#include "Storage_p.h"

#include <QJSEngine>

namespace stimmt::modul {
    Storage::Storage(QObject *parent) : Storage(parent, *new StoragePrivate) {

    }

    Storage::~Storage() = default;

    int Storage::length() const {
        Q_D(const Storage);
        QReadLocker locker(const_cast<QReadWriteLock *>(&d->lock));
        return d->data.size();
    }

    void Storage::setData(const QVariantMap &data) {
        Q_D(Storage);
        QWriteLocker locker(&d->lock);
        d->data = data;
        emit dataChanged(d->data);
    }

    QVariantMap Storage::data() const {
        Q_D(const Storage);
        QReadLocker locker(const_cast<QReadWriteLock *>(&d->lock));
        return d->data;
    }

    QJSValue Storage::getItem(const QString &keyName) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        Q_D(const Storage);
        QReadLocker locker(const_cast<QReadWriteLock *>(&d->lock));
        auto it = d->data.find(keyName);
        if (it == d->data.end())
            return QJSValue::NullValue;
        return engine->toScriptValue(it.value());
    }

    void Storage::setItem(const QString &keyName, const QJSValue &keyValue) {
        Q_D(Storage);
        QWriteLocker locker(&d->lock);
        auto v = qjsvalue_cast<QVariant>(keyValue);
        auto it = d->data.find(keyName);
        if (it == d->data.end()) {
            d->data.insert(keyName, v);
            emit dataChanged(d->data);
        } else if (it.value() != v) {
            it.value() = v;
            emit dataChanged(d->data);
        }
    }

    void Storage::removeItem(const QString &keyName) {
        Q_D(Storage);
        QWriteLocker locker(&d->lock);
        if (d->data.remove(keyName)) {
            emit dataChanged(d->data);
        }
    }

    void Storage::clear() {
        Q_D(Storage);
        QWriteLocker locker(&d->lock);
        if (!d->data.isEmpty()) {
            d->data.clear();
            emit dataChanged(d->data);
        }
    }

    QJSValue Storage::key(int index) const {
        Q_D(const Storage);
        QReadLocker locker(const_cast<QReadWriteLock *>(&d->lock));
        return d->data.keys()[index];
    }

    Storage::Storage(QObject *parent, StoragePrivate &d) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
    }
} // stimmt::modul