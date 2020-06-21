
#ifndef g_path_h_
#define g_path_h_ 1

#include "base\g_def.h"
#include <windows.h>
BEG_NSP_DDM

// 获取一些全局的路径，比如某个dll路径，临时目录，注册表路径等等
class g_path
{
public:
    // 获得模块的运行路径，包括名称后缀
    // @param [in，out] hMoudle 模块句柄，如果为空则自动获取
    static ddstr get_moudle_run_path(HMODULE hMoudle = NULL);

    // 获得 host dll 名称包括后缀
    static ddstr get_host_dll_name();

    // 获得 host dll 的全路径，包括名称后缀
    static ddstr get_host_dll_path();

    // 获得 资源的父目录
    static ddstr get_res_base_path();
};

END_NSP_DDM
#endif // g_path_h_

