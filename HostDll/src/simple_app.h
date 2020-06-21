
#ifndef simple_app_h_
#define simple_app_h_ 1

#include "base\g_def.h"
#include <windows.h>

BEG_NSP_DDM

// 运行命令的结果
enum class app_run_cmd_res
{
    // 继续运行窗口
    CONTINUE_WND = 0,

    // 退出程序
    EXIT_PROCESS
};

class simple_app {
public:
    // 运行窗口
    int run_wnd(HINSTANCE hInstance);

    // 运行命令行，返回
    app_run_cmd_res run_cmd(const std::vector<ddstr>& cmds);

    // 获得运行命令行的结果，注意此结果和run_cmd返回结果不同
    inline int get_run_cmd_result()
    {
        return m_run_cmd_result;
    }

private:
    int m_run_cmd_result = 1;
};

END_NSP_DDM
#endif // simple_app_h_
