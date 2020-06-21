
#include "export\host_dll.h"

#include "simple_app.h"

#include <iostream>
#include <windows.h>

BEG_NSP_DDM

extern HMODULE g_hMoudle = NULL;

class host_dll_impl : public host_dll {
public:
    virtual int host_dll_main(const std::vector<ddstr>& cmds) override
    {
        simple_app app;
        app_run_cmd_res cmdRes = app.run_cmd(cmds);
        if (cmdRes != app_run_cmd_res::CONTINUE_WND) {
            return app.get_run_cmd_result();
        } else {
            return app.run_wnd(g_hMoudle);
        }
    }

    virtual HMODULE get_moudle()
    {
        return g_hMoudle;
    }
};

extern "C" {
    Dll_EXPORT host_dll* new_host_dll()
    {
        return new (std::nothrow) host_dll_impl();
    }
    Dll_EXPORT void del_host_dll(host_dll* pHostDll)
    {
        if (pHostDll != NULL) {
            delete pHostDll;
        }
    }
}

END_NSP_DDM

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  reasonForCall,
                      LPVOID lpReserved)
{
    (hModule);
    (reasonForCall);
    (lpReserved);

    switch (reasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        NSP_DDM::g_hMoudle = hModule;
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    default:
        break;
    }

    return TRUE;
}


