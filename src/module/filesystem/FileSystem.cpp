#include "FileSystem.h"

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QJSEngine>
#include <QUrl>

#include <stimmt/ModuleSystem.h>
#include <stimmt/TypedArrayHelper.h>
#include <stimmt/ClosureHelper.h>

namespace stimmt::modul {

    FileSystem::FileSystem(QObject *parent) : QObject(parent) {
    }

    FileSystem::~FileSystem() = default;

    void FileSystem::registerModule(ModuleSystem *moduleSystem, FileSystem *moduleObject) {
        moduleSystem->registerModule("stimmt:filesystem", ClosureHelper::makeClosure(moduleSystem->engine(), moduleObject));
    }

    QStringList FileSystem::readDir(const QString &dir) const {
        return QDir().entryList();
    }

    bool FileSystem::exists(const QString &path) const {
        return QDir().exists(path);
    }

    bool FileSystem::isDir(const QString &path) const {
        return QFileInfo(path).isDir();
    }

    QJSValue FileSystem::readFile(const QString &path) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly)) {
            engine->throwError("Cannot open file: " + path);
            return {};
        }
        return TypedArrayHelper::toUint8Array(engine, f.readAll());
    }

    void FileSystem::writeFile(const QString &path, const QJSValue &data) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        QFile f(path);
        if (!f.open(QIODevice::WriteOnly)) {
            engine->throwError("Cannot open file: " + path);
            return;
        }
        auto byteData = data.isString() ? data.toString().toUtf8() : TypedArrayHelper::fromGenericValue(engine, data);
        f.write(byteData);
    }

    void FileSystem::appendFile(const QString &path, const QJSValue &data) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        QFile f(path);
        if (!f.open(QIODevice::Append)) {
            engine->throwError("Cannot open file: " + path);
            return;
        }
        auto byteData = data.isString() ? data.toString().toUtf8() : TypedArrayHelper::fromGenericValue(engine, data);
        f.write(byteData);
    }

    void FileSystem::remove(const QString &path) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!QFile::remove(path)) {
            engine->throwError("Cannot remove: " + path);
        }
    }

    void FileSystem::removeRecursively(const QString &dir) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!QDir(dir).removeRecursively()) {
            engine->throwError("Cannot remove recursively: " + dir);
        }
    }

    void FileSystem::rename(const QString &oldPath, const QString &newPath) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!QFile::rename(oldPath, newPath)) {
            engine->throwError(QString("Cannot rename %1 to %2").arg(oldPath, newPath));
        }
    }

    void FileSystem::copyFile(const QString &src, const QString &dest) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!QFile::copy(src, dest)) {
            engine->throwError(QString("Cannot copy %1 to %2").arg(src, dest));
        }
    }

    void FileSystem::mkdir(const QString &dir) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!QDir().mkdir(dir)) {
            engine->throwError("Cannot mkdir: " + dir);
        }
    }

    void FileSystem::mkpath(const QString &dir) const {
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!QDir().mkpath(dir)) {
            engine->throwError("Cannot mkpath: " + dir);
        }
    }

    void FileSystem::openFileOrUrl(const QString &path) const {
        QDesktopServices::openUrl(QUrl::fromUserInput(path));
    }

} // stimmt::modul