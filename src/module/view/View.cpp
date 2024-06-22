#include "View.h"

#include <QJSEngine>

#include <stimmt/ModuleSystem.h>
#include <stimmt/BasicForm.h>
#include <stimmt/Quick.h>

namespace stimmt::modul {
    void View::registerModule(ModuleSystem *moduleSystem) {
        auto moduleObject = moduleSystem->engine()->newObject();
        moduleObject.setProperty("BasicForm", moduleSystem->engine()->newQMetaObject<BasicForm>());
        moduleObject.setProperty("Quick", moduleSystem->engine()->newQMetaObject<Quick>());
        moduleSystem->registerModule("stimmt:view", moduleObject);
    }
} // stimmt