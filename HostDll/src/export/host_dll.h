
#ifndef host_dll_h_
#define host_dll_h_
#include "base\g_def.h"
#include <memory>

#include <windows.h>

BEG_NSP_DDM
#define Dll_EXPORT __declspec( dllexport )

class host_dll {
public:
    virtual ~host_dll(){}

    // 入口点
    virtual int host_dll_main(const std::vector<ddstr>& cmds) = 0;

    // 获得模块
    virtual HMODULE get_moudle() = 0;
};

extern "C" {
    Dll_EXPORT host_dll* new_host_dll();
    Dll_EXPORT void del_host_dll(host_dll* pHostDll);
}

END_NSP_DDM
#endif
