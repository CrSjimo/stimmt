#ifndef STIMMT_GLOBALREGISTRYOBJECT_H
#define STIMMT_GLOBALREGISTRYOBJECT_H

#include <QHash>
#include <QJSValue>
#include <QObject>

#include <stimmt/StimmtGlobal.h>

namespace stimmt {
    class GlobalObject;

    class STIMMT_EXPORT GlobalRegistryObject : public QObject {
        Q_OBJECT
    public:
        explicit GlobalRegistryObject(GlobalObject *global);
        ~GlobalRegistryObject() override;

        QStringList scripts() const;
        struct ScriptSpec {
            bool isBuiltIn = false;
            bool isVisibleToUser = true;
            QJSValue scriptConstructor;
        };
        ScriptSpec scriptSpec(const QString &id) const;
        QJSValue scriptConstructor(const QString &id) const;
        void loadScript(const QString &userScriptDir);
        void clearRegistry();

        template<class T>
        void addInternalScriptImplementation(bool isVisibleToUser = true) {
            addInternalScriptImplementationImpl(&T::staticMetaObject, T::manifest());
        }

    public slots:
        void registerScript(const QJSValue &scriptConstructor);

    private:
        friend class GlobalObject;
        ScriptSpec m_currentScriptSpec;
        QHash<QString, ScriptSpec> m_scriptDict;

        QJSValue m_builtInScriptHelper;
        void registerScriptImplementation(const QMetaObject *impl, const QJSValue &manifest, bool isVisibleToUser);
        void registerBuiltInScripts();

        void addInternalScriptImplementationImpl(const QMetaObject *impl, const QJSValue &manifest, bool isVisibleToUser);
        QList<std::tuple<const QMetaObject *, QJSValue, bool>> m_internalScriptImplementations;
    };
}

#endif // STIMMT_GLOBALREGISTRYOBJECT_H
