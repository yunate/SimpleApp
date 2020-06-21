
#ifndef g_path_h_
#define g_path_h_ 1

#include "base\g_def.h"
#include <windows.h>
BEG_NSP_DDM

// ��ȡһЩȫ�ֵ�·��������ĳ��dll·������ʱĿ¼��ע���·���ȵ�
class g_path
{
public:
    // ���ģ�������·�����������ƺ�׺
    // @param [in��out] hMoudle ģ���������Ϊ�����Զ���ȡ
    static ddstr get_moudle_run_path(HMODULE hMoudle = NULL);

    // ��� host dll ���ư�����׺
    static ddstr get_host_dll_name();

    // ��� host dll ��ȫ·�����������ƺ�׺
    static ddstr get_host_dll_path();

    // ��� ��Դ�ĸ�Ŀ¼
    static ddstr get_res_base_path();
};

END_NSP_DDM
#endif // g_path_h_

