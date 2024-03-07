#ifndef STIMMT_MODULESYSTEM_H
#define STIMMT_MODULESYSTEM_H

#include <QJSValue>
#include <QObject>

#include <stimmt/stimmtGlobal.h>

class QJSEngine;

namespace stimmt {

    class ModuleSystemPrivate;

    class STIMMT_EXPORT ModuleSystem : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ModuleSystem)
    public:
        explicit ModuleSystem(QJSEngine *engine, QObject *parent = nullptr);
        ~ModuleSystem() override;

        QJSValue loadModule(const QString &moduleName);
        bool registerModule(const QString &moduleName, const QJSValue &value);

    protected:
        ModuleSystem(QObject *parent, ModuleSystemPrivate &d);

    private:
        QScopedPointer<ModuleSystemPrivate> d_ptr;
    };

} // stimmt

#endif //STIMMT_MODULESYSTEM_H
