#include "simple_app.h"
#include "main_wnd.h"
#include "g_path.h"

#include "commgr2.h"
#include "souistd.h"

#include <memory>

using namespace SOUI;

// 1��ʾ��skia��Ⱦ�� ����GDI��Ⱦ
#define RANDER_TYPE 1

#ifdef _DEBUG
#define RES_DDTYPE 0      //���ļ��м�����Դ������ʧ���ٴ�PE����
#define SYS_NAMED_RESOURCE _DDT("soui-sys-resourced.dll")
#else
#define RES_DDTYPE 1        //��PE��Դ�м���UI��Դ
#define SYS_NAMED_RESOURCE _DDT("soui-sys-resource.dll")
#endif

BEG_NSP_DDM

using sp_app = std::shared_ptr<SApplication>;

sp_app create_app(SComMgr2& comMgr, HINSTANCE hInstance)
{
    SAutoRefPtr<IImgDecoderFactory> pImgDecoderFactory;
    if (!comMgr.CreateImgDecoder((IObjRef**)&pImgDecoderFactory)) {
        return nullptr;
    }

    SAutoRefPtr<IRenderFactory> pRenderFactory;
#if (RANDER_TYPE == 1)
    if (!comMgr.CreateRender_Skia((IObjRef**)&pRenderFactory))
#else
    if (!comMgr.CreateRender_GDI((IObjRef**)&pRenderFactory))
#endif 
    {
        return nullptr;
    } else {
        // Ϊ��Ⱦģ����������Ҫ���õ�ͼƬ����ģ��
        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
    }

    // ����һ��Ψһ��SApplication����SApplication��������Ӧ�ó������Դ
    return sp_app(new (std::nothrow) SApplication(pRenderFactory, hInstance));
}

bool load_resource(sp_app spApp, HINSTANCE hInstance)
{
#ifdef _DEBUG
    // �����������·���޸ĵ�demo���ڵ�Ŀ¼
    // SStringT strResDir = spApp->GetAppDir();
    // strResDir += _DDT("\\..\\demo");
    // ::SetCurrentDirectory(strResDir);
    // //SOUIϵͳ���Ǵ�appdirȥ������Դ
    // spApp->SetAppDir(strResDir);
#endif
    ddstr resBasePath = g_path::get_res_base_path();

    //����ϵͳ��Դ
    HMODULE hSysResource = ::LoadLibrary((resBasePath + SYS_NAMED_RESOURCE).c_str());
    if (hSysResource != NULL)
    {
        SAutoRefPtr<IResProvider> sysResProvider;
        CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
        sysResProvider->Init((WPARAM)hSysResource, 0);
        spApp->LoadSystemNamedResource(sysResProvider);
        ::FreeLibrary(hSysResource);
    }

    //����һ�˸���Դ�ṩ����,SOUIϵͳ��ʵ����3����Դ���ط�ʽ���ֱ��Ǵ��ļ����أ���EXE����Դ���ؼ���ZIPѹ��������
    SAutoRefPtr<IResProvider> pResProvider;
#if (RES_DDTYPE == 0)//���ļ�����
    CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
    if (!pResProvider->Init((LPARAM)(resBasePath.c_str()), 0))
    {
        CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
        if (!pResProvider->Init((WPARAM)hInstance, 0))
        {
            return false;
        }
    }
#elif (RES_DDTYPE==1)//��EXE��Դ����
    CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
    pResProvider->Init((WPARAM)hInstance, 0);
#elif (RES_DDTYPE==2)//��ZIP������
    result = comMgr.CreateResProvider_ZIP((IObjRef**)&pResProvider);
    DD_ASSERT_FMT(result, _DDT("load interface [%s] failed!"), _DDT("resprovider_zip"));

    ZIPRES_PARAM param;
    param.ZipFile(pRenderFactory, _DDT("uires.zip"), "souizip");
    result = pResProvider->Init((WPARAM)&param, 0);
    DD_ASSERT(result);
#endif

    //��������IResProvider����SApplication����
    spApp->AddResProvider(pResProvider);
    return true;
}

int simple_app::run_wnd(HINSTANCE hInstance)
{
    SComMgr2 comMgr(_DDT("imgdecoder-png"));
    sp_app spApp = create_app(comMgr, hInstance);

    if (spApp == nullptr) {
        return -1;
    }

    if (!load_resource(spApp, hInstance)) {
        return -1;
    }

    {
        CMainWnd wndMain;
        wndMain.Create(GetActiveWindow(), 0, 0, 800, 600);
        wndMain.SendMessage(WM_INITDIALOG);
        wndMain.CenterWindow(wndMain.m_hWnd);
        wndMain.ShowWindow(SW_SHOWNORMAL);
        return spApp->Run(wndMain.m_hWnd);
    }
}

app_run_cmd_res simple_app::run_cmd(const std::vector<ddstr>& cmds)
{
    m_run_cmd_result = 1;
    if (cmds.size() <= 1) {
        return app_run_cmd_res::CONTINUE_WND;
    }

    return app_run_cmd_res::CONTINUE_WND;
}

END_NSP_DDM

