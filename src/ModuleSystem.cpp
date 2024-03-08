#include "ModuleSystem.h"
#include "ModuleSystem_p.h"

#include <QDir>
#include <QJSEngine>
#include <QQmlEngine>

namespace stimmt {

    QJSValue ModuleSystemPrivate::require(const QString &moduleName) {
        Q_Q(ModuleSystem);
        return q->loadModule(moduleName);
    }

    ModuleSystem::ModuleSystem(QJSEngine *engine, QObject *parent) : ModuleSystem(parent, *new ModuleSystemPrivate) {
        Q_D(ModuleSystem);
        d->engine = engine;
        auto dObj = engine->newQObject(d);
        d->requireFunction = engine->evaluate("(t) => ((s) => t.require(s))").call({dObj});
        QQmlEngine::setObjectOwnership(d, QQmlEngine::CppOwnership);
    }

    ModuleSystem::ModuleSystem(QObject *parent, ModuleSystemPrivate &d) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
    }

    ModuleSystem::~ModuleSystem() = default;

    QJSValue ModuleSystem::loadModule(const QString &moduleName) {
        Q_D(ModuleSystem);
        if (QDir(moduleName).isAbsolute() || moduleName.startsWith('.')) {
            auto resolvingFileInfo = QFileInfo(d->currentDirectory.isEmpty() ? moduleName : QDir(d->currentDirectory).filePath(moduleName));
            if (resolvingFileInfo.isDir()) {
                resolvingFileInfo = QFileInfo(QDir(resolvingFileInfo.absoluteFilePath()).filePath("index.js"));
            }
            if (!resolvingFileInfo.exists()) {
                resolvingFileInfo = QFileInfo(resolvingFileInfo.absoluteFilePath() + ".js");
            }
            if (!resolvingFileInfo.isFile()) {
                d->engine->throwError(QString("Cannot load module '%1'").arg(resolvingFileInfo.absoluteFilePath()));
                return {};
            }
            auto filePath = resolvingFileInfo.canonicalFilePath();
            if (d->cachedFileModules.contains(filePath)) {
                return d->cachedFileModules.value(filePath);
            }
            QFile f(filePath);
            if (!f.open(QIODevice::ReadOnly)) {
                d->engine->throwError(QString("Cannot load module '%1'").arg(filePath));
                return {};
            }
            auto sourceCode = QString::fromUtf8(f.readAll());
            auto previousDirectory = d->currentDirectory;
            auto guardFunc = [=](ModuleSystemPrivate *d) {
                d->currentDirectory = previousDirectory;
            };
            std::unique_ptr<ModuleSystemPrivate, decltype(guardFunc)> guard(d, guardFunc);
            d->currentDirectory = resolvingFileInfo.canonicalPath();
            auto capsuledFunction = d->engine->evaluate(QString("(require, module, __filename, __dirname) => {%1}").arg(sourceCode), filePath);
            if (capsuledFunction.isError()) {
                d->engine->globalObject().property("console").property("error").call({capsuledFunction});
                d->engine->throwError(QString("Cannot load module '%1'").arg(filePath));
                return {};
            }
            QJSValue moduleObj = d->engine->newObject();
            moduleObj.setProperty("exports", d->engine->newObject());
            d->cachedFileModules.insert(filePath, QJSValue::UndefinedValue);
            auto ret = capsuledFunction.call({d->requireFunction, moduleObj, filePath, d->currentDirectory});
            if (ret.isError()) {
                d->engine->evaluate("(s) => console.error(s)").call({ret});
                d->engine->throwError(QString("Cannot load module '%1'").arg(filePath));
                d->cachedFileModules.remove(filePath);
                return {};
            }
            d->cachedFileModules.insert(filePath, moduleObj.property("exports"));
            return moduleObj.property("exports");
        } else {
            auto it = d->nativeModules.find(moduleName);
            if (it == d->nativeModules.end()) {
                d->engine->throwError(QString("Cannot load module '%1'").arg(moduleName));
                return {};
            }
            return *it;
        }
    }

    bool ModuleSystem::registerModule(const QString &moduleName, const QJSValue &value) {
        Q_D(ModuleSystem);
        if (d->nativeModules.contains(moduleName))
            return false;
        d->nativeModules.insert(moduleName, value);
        return true;
    }
} // stimmt