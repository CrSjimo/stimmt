#ifndef STIMMT_STORAGE_H
#define STIMMT_STORAGE_H

#include <QJSValue>
#include <QVariant>
#include <QObject>

#include <stimmt/stimmtGlobal.h>

namespace stimmt {
    class ModuleSystem;
}

namespace stimmt::modul {

    class StoragePrivate;

    class STIMMT_EXPORT Storage : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Storage)
        Q_PROPERTY(int length READ length)
    public:
        explicit Storage(QObject *parent = nullptr);
        ~Storage() override;

        static void registerModule(ModuleSystem *moduleSystem, Storage *localStorage, Storage *sessionStorage, Storage *projectStorage, Storage *scopedStorage);

        int length() const;

        void setData(const QVariantMap &data);
        QVariantMap data() const;

    public slots:
        QJSValue getItem(const QString &keyName) const;
        void setItem(const QString &keyName, const QJSValue &keyValue);
        void removeItem(const QString &keyName);

        void clear();

        QJSValue key(int index) const;

    signals:
        void dataChanged(const QVariantMap &data);

    protected:
        Storage(QObject *parent, StoragePrivate &d);

    private:
        QScopedPointer<StoragePrivate> d_ptr;
    };

} // stimmt::modul

#endif //STIMMT_STORAGE_H
