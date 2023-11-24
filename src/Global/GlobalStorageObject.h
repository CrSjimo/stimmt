#ifndef STIMMT_GLOBALSTORAGEOBJECT_H
#define STIMMT_GLOBALSTORAGEOBJECT_H

#include <QFile>
#include <QJSValue>
#include <QJsonObject>
#include <QObject>

#include <stimmt/StimmtGlobal.h>

namespace stimmt {
    class GlobalObject;

    class STIMMT_EXPORT GlobalStorageObject : public QObject {
        Q_OBJECT
    public:
        explicit GlobalStorageObject(GlobalObject *global, const QString &filename);
        ~GlobalStorageObject() override;

        void update();

    public slots:
        void setItem(const QString &key, const QString &value);
        QJSValue getItem(const QString &key);
        void removeItem(const QString &key);

    private:
        QFile m_storageFile;
        QJsonObject m_jsonStorage;
        bool m_isModified = false;
    };
}

#endif // STIMMT_GLOBALSTORAGEOBJECT_H
