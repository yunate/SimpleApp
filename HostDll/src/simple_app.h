
#ifndef simple_app_h_
#define simple_app_h_ 1

#include "base\g_def.h"
#include <windows.h>

BEG_NSP_DDM

// ��������Ľ��
enum class app_run_cmd_res
{
    // �������д���
    CONTINUE_WND = 0,

    // �˳�����
    EXIT_PROCESS
};

class simple_app {
public:
    // ���д���
    int run_wnd(HINSTANCE hInstance);

    // ���������У�����
    app_run_cmd_res run_cmd(const std::vector<ddstr>& cmds);

    // ������������еĽ����ע��˽����run_cmd���ؽ����ͬ
    inline int get_run_cmd_result()
    {
        return m_run_cmd_result;
    }

private:
    int m_run_cmd_result = 1;
};

END_NSP_DDM
#endif // simple_app_h_
