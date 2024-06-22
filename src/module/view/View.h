#ifndef STIMMT_VIEW_H
#define STIMMT_VIEW_H

#include <stimmt/stimmtGlobal.h>

namespace stimmt {
    class ModuleSystem;
}

namespace stimmt::modul {

    class STIMMT_EXPORT View {
    public:
        static void registerModule(ModuleSystem *moduleSystem);
    };

}

#endif //STIMMT_VIEW_H
