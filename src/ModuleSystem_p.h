#ifndef STIMMT_MODULESYSTEM_P_H
#define STIMMT_MODULESYSTEM_P_H

#include <QMap>

#include <stimmt/ModuleSystem.h>

namespace stimmt {

    class ModuleSystemPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ModuleSystem)
    public:
        ModuleSystem *q_ptr;
        QJSEngine *engine;
        QJSValue requireFunction;
        QString currentDirectory;
        QMap<QString, QJSValue> nativeModules;
        QMap<QString, QJSValue> cachedFileModules;

    public slots:
        QJSValue require(const QString &moduleName);
    };

}

#endif //STIMMT_MODULESYSTEM_P_H
