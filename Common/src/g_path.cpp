#include "export/g_path.h"

#include <windows.h>
BEG_NSP_DDM

ddstr g_path::get_moudle_run_path(HMODULE hMoudle/* = NULL*/)
{
    static ddstr moudlePath;
    if (moudlePath.size() < 1) {
        if (hMoudle == NULL) {
            BOOL rtn = ::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)(&moudlePath), &hMoudle);

            if (!rtn) {
                return _DDT("");
            }
        }

        ddchar chArr[1024];
        DWORD size = ::GetModuleFileName(hMoudle, (LPTSTR)chArr, sizeof(chArr) / sizeof(ddchar));

        if (size >= sizeof(chArr) / sizeof(ddchar) || size == 0) {
            return _DDT("");
        }

        moudlePath = chArr;
    }

    return moudlePath;
}

ddstr g_path::get_host_dll_name()
{
    return _DDT("HostDll.dll");
}

ddstr g_path::get_host_dll_path()
{
    ddstr exePath = get_moudle_run_path();

    if (exePath.empty()) {
        return _DDT("");
    }

    size_t pos = exePath.find_last_of(_DDT("\\"));

    if (pos == ddstr::npos) {
        return _DDT("");
    }

    exePath = exePath.substr(0, pos + 1);
    exePath += get_host_dll_name();
    return exePath;
}

ddstr g_path::get_res_base_path()
{
#ifdef _DEBUG
    return _DDT("../../HostDll/res/uires/");
#endif // _DEBUG

    return ddstr();
}

END_NSP_DDM

