#include "simple_app.h"
#include "main_wnd.h"
#include "g_path.h"

#include "commgr2.h"
#include "souistd.h"

#include <memory>

using namespace SOUI;

// 1表示用skia渲染， 否者GDI渲染
#define RANDER_TYPE 1

#ifdef _DEBUG
#define RES_DDTYPE 0      //从文件中加载资源，加载失败再从PE加载
#define SYS_NAMED_RESOURCE _DDT("soui-sys-resourced.dll")
#else
#define RES_DDTYPE 1        //从PE资源中加载UI资源
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
        // 为渲染模块设置它需要引用的图片解码模块
        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
    }

    // 定义一个唯一的SApplication对象，SApplication管理整个应用程序的资源
    return sp_app(new (std::nothrow) SApplication(pRenderFactory, hInstance));
}

bool load_resource(sp_app spApp, HINSTANCE hInstance)
{
#ifdef _DEBUG
    // 将程序的运行路径修改到demo所在的目录
    // SStringT strResDir = spApp->GetAppDir();
    // strResDir += _DDT("\\..\\demo");
    // ::SetCurrentDirectory(strResDir);
    // //SOUI系统总是从appdir去查找资源
    // spApp->SetAppDir(strResDir);
#endif
    ddstr resBasePath = g_path::get_res_base_path();

    //加载系统资源
    HMODULE hSysResource = ::LoadLibrary((resBasePath + SYS_NAMED_RESOURCE).c_str());
    if (hSysResource != NULL)
    {
        SAutoRefPtr<IResProvider> sysResProvider;
        CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
        sysResProvider->Init((WPARAM)hSysResource, 0);
        spApp->LoadSystemNamedResource(sysResProvider);
        ::FreeLibrary(hSysResource);
    }

    //定义一人个资源提供对象,SOUI系统中实现了3种资源加载方式，分别是从文件加载，从EXE的资源加载及从ZIP压缩包加载
    SAutoRefPtr<IResProvider> pResProvider;
#if (RES_DDTYPE == 0)//从文件加载
    CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
    if (!pResProvider->Init((LPARAM)(resBasePath.c_str()), 0))
    {
        CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
        if (!pResProvider->Init((WPARAM)hInstance, 0))
        {
            return false;
        }
    }
#elif (RES_DDTYPE==1)//从EXE资源加载
    CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
    pResProvider->Init((WPARAM)hInstance, 0);
#elif (RES_DDTYPE==2)//从ZIP包加载
    result = comMgr.CreateResProvider_ZIP((IObjRef**)&pResProvider);
    DD_ASSERT_FMT(result, _DDT("load interface [%s] failed!"), _DDT("resprovider_zip"));

    ZIPRES_PARAM param;
    param.ZipFile(pRenderFactory, _DDT("uires.zip"), "souizip");
    result = pResProvider->Init((WPARAM)&param, 0);
    DD_ASSERT(result);
#endif

    //将创建的IResProvider交给SApplication对象
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

