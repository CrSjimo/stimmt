#ifndef STIMMT_FILESYSTEM_H
#define STIMMT_FILESYSTEM_H

#include <QJSValue>
#include <QObject>

#include <stimmt/stimmtGlobal.h>

class QJSEngine;

namespace stimmt {
    class ModuleSystem;
}

namespace stimmt::modul {

    class STIMMT_EXPORT FileSystem : public QObject {
        Q_OBJECT
    public:
        explicit FileSystem(QObject *parent = nullptr);
        ~FileSystem() override;

        static void registerModule(ModuleSystem *moduleSystem, FileSystem *moduleObject);

    public slots:
        QStringList readDir(const QString &dir) const;
        bool exists(const QString &path) const;
        bool isDir(const QString &path) const;

        QJSValue readFile(const QString &path) const;
        void writeFile(const QString &path, const QJSValue &data) const;
        void appendFile(const QString &path, const QJSValue &data) const;

        void remove(const QString &path) const;
        void removeRecursively(const QString &dir) const;
        void rename(const QString &oldPath, const QString &newPath) const;
        void copyFile(const QString &src, const QString &dest) const;

        void mkdir(const QString &dir) const;
        void mkpath(const QString &dir) const;

        void openFileOrUrl(const QString &path) const;
    };

} // stimmt::modul

#endif //STIMMT_FILESYSTEM_H
