#include "GlobalRegistryObject.h"

#include <QJSEngine>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include "GlobalObject.h"
#include "Console.h"

namespace stimmt {
    GlobalRegistryObject::GlobalRegistryObject(GlobalObject * global) : QObject(global) {
    }

    GlobalRegistryObject::~GlobalRegistryObject() = default;

    void GlobalRegistryObject::registerScriptImplementation(const QMetaObject *impl, const QJSValue &manifest,
                                                                bool isVisibleToUser) {
        m_currentScriptSpec = {true, isVisibleToUser};
        auto ret = m_builtInScriptHelper.call({jsGlobal->engine()->newQMetaObject(impl), manifest});
        if (ret.isError())
            jsGlobal->console()->printUncaughtError(ret);
        m_currentScriptSpec = ScriptSpec();
    }

    void GlobalRegistryObject::addInternalScriptImplementationImpl(const QMetaObject *impl, const QJSValue &manifest,
                                                                   bool isVisibleToUser) {
        m_internalScriptImplementations.append({impl, manifest, isVisibleToUser});
    }

    void GlobalRegistryObject::registerBuiltInScripts() {
        m_builtInScriptHelper = jsGlobal->load(":/stimmt/scripts/builtInScriptHelper.js");
        for (const auto &[impl, manifest, isVisibleToUser]: m_internalScriptImplementations) {
            registerScriptImplementation(impl, manifest, isVisibleToUser);
        }
    }

    QStringList GlobalRegistryObject::scripts() const {
        return m_scriptDict.keys();
    }

    GlobalRegistryObject::ScriptSpec GlobalRegistryObject::scriptSpec(const QString &id) const {
        return m_scriptDict.value(id);
    }

    QJSValue GlobalRegistryObject::scriptConstructor(const QString &id) const {
        return m_scriptDict.value(id).scriptConstructor;
    }

    void GlobalRegistryObject::loadScript(const QString &userScriptDir) {
        registerBuiltInScripts();
        QList<QString> failedFiles;
        QDir dir(userScriptDir);
        if (!dir.exists()) {
            QMessageBox::warning(nullptr, tr("Script Loading Error"),
                                 tr("User script directory not exists:\n\n%1").arg(userScriptDir));
            return;
        }
        for (const auto &file : dir.entryList(QDir::Files)) {
            auto ret = jsGlobal->load(dir.filePath(file));
            if (ret.isError())
                failedFiles.append(file);
        }
        if (!failedFiles.isEmpty()) {
            QMessageBox::warning(
                nullptr, tr("Script Loading Error"),
                tr("JavaScript errors occurred when loading %n file(s):\n\n%1", nullptr, failedFiles.size())
                    .arg(failedFiles.join('\n')));
        }
    }

    void GlobalRegistryObject::clearRegistry() {
        m_scriptDict.clear();
    }

    void GlobalRegistryObject::registerScript(const QJSValue &scriptConstructor) {
        if (!scriptConstructor.property("manifest").isCallable())
            return JS_THROW(QJSValue::TypeError, "'manifest' is not a function");
        auto manifest = scriptConstructor.property("manifest").call();
        if (!manifest.property("id").isString())
            return JS_THROW(QJSValue::TypeError, "Invalid return value of manifest()");
        auto id = manifest.property("id").toString();
        if (m_scriptDict.contains(id))
            return JS_THROW(QJSValue::RangeError, "Script already exists: " + id);
        auto spec = m_currentScriptSpec;
        spec.scriptConstructor = scriptConstructor;
        m_scriptDict.insert(id, spec);
    }
}
