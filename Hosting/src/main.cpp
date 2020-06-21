
#include "host_dll.h"

#include "g_path.h"

#include "misc\cmd_line_util.h"

#include <iostream>
#include <sstream>

BEG_NSP_DDM

int ddm_main()
{
#ifdef _UNICODE
    std::wstringstream errstr;
#else
    std::stringstream errstr;
#endif // _UNICODE

    HMODULE hDll = NULL;
    int mainRtn = -1;

    do {
        ddstr dllPath = g_path::get_host_dll_path();

        if (dllPath.empty()) {
            errstr << _DDT("get_host_dll_path failure");
            break;
        }

        hDll = ::LoadLibrary(dllPath.c_str());

        if (hDll == NULL) {
            errstr << _DDT("load dll:") << dllPath << _DDT(" error, GetLasrError:") << ::GetLastError();
            break;
        }

        typedef host_dll* (*new_host_dll_api)();
        typedef void (*del_host_dll_api)(host_dll*);
        new_host_dll_api new_host_dll_fun = (new_host_dll_api)::GetProcAddress(hDll, "new_host_dll");
        del_host_dll_api dle_host_dll_fun = (del_host_dll_api)::GetProcAddress(hDll, "del_host_dll");

        if (new_host_dll_fun == NULL || dle_host_dll_fun == NULL) {
            errstr << _DDT("GetProcAddress:new_host_dll/del_host_dll failure, GetLasrError:")<< ::GetLastError();
            break;
        }

        host_dll* pHostDll = (*new_host_dll_fun)();

        if (pHostDll == nullptr) {
            errstr << _DDT("new_host_dll()return nullptr");
            break;
        }

        std::vector<ddstr> cmds;
        cmd_line_util::get_cmds(cmds);
        mainRtn = pHostDll->host_dll_main(cmds);
        (*dle_host_dll_fun)(pHostDll);
        pHostDll = NULL;
    } while (0);


    if (hDll != NULL) {
        ::FreeLibrary(hDll);
    }

    ddstr tmp = errstr.str();
    if (!tmp.empty()) {
#ifdef _UNICODE
        std::wcout << tmp << std::endl;
#else
        std::cout << tmp << std::endl;
#endif // _UNICODE
    }

    return mainRtn;
}

END_NSP_DDM

int main()
{
    return NSP_DDM::ddm_main();
}
