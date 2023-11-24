#ifndef STIMMT_PROJECTSCOPEDSTORAGEOBJECT_H
#define STIMMT_PROJECTSCOPEDSTORAGEOBJECT_H

#include <QObject>
#include <QMap>
#include <QJSValue>

#include <stimmt/StimmtGlobal.h>

namespace stimmt {
    class ProjectObject;

    class STIMMT_EXPORT ProjectScopedStorageObject : public QObject {
        Q_OBJECT
    public:
        explicit ProjectScopedStorageObject(ProjectObject *project);

    public slots:
        void setItem(const QString &key, const QJSValue &value);
        QJSValue getItem(const QString &key) const;
        void removeItem(const QString &key);

    private:
        QMap<QString, QJSValue> m_itemDict;
    };
}

#endif // STIMMT_PROJECTSCOPEDSTORAGEOBJECT_H
