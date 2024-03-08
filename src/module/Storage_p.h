#ifndef STIMMT_STORAGE_P_H
#define STIMMT_STORAGE_P_H

#include <stimmt/Storage.h>
#include <QReadWriteLock>

namespace stimmt::modul {
    class StoragePrivate {
        Q_DECLARE_PUBLIC(Storage);
        Storage *q_ptr;
        QReadWriteLock lock;
        QVariantMap data;
    };
}

#endif //STIMMT_STORAGE_P_H
