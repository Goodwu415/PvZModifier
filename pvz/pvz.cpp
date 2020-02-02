// pvz.cpp: 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "pvz.h"
#include "CDialogMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CpvzApp

BEGIN_MESSAGE_MAP(CpvzApp, CWinApp)
END_MESSAGE_MAP()


// CpvzApp 构造

CpvzApp::CpvzApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CpvzApp 对象

CpvzApp theApp;

#define GAME_WIN_NAME _T("植物大战僵尸中文版")

HHOOK g_hHook;
//辅助对话框
CDialogMain* g_pDlg{ nullptr };
// CpvzApp 初始化

BOOL CpvzApp::InitInstance()
{
	CWinApp::InitInstance();

	if(!AfxSocketInit()){
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	return TRUE;
}

static bool IsFullScreenWindow(HWND hWnd)
{
	bool bFullScreen = false;
	RECT rcApp, rcDesk;
	GetWindowRect(GetDesktopWindow(), &rcDesk);
	if(hWnd != GetDesktopWindow() && hWnd != GetShellWindow())
	{
		GetWindowRect(hWnd, &rcApp);
		if(rcApp.left <= rcDesk.left
			&& rcApp.top <= rcDesk.top
			&& rcApp.right >= rcDesk.right
			&& rcApp.bottom >= rcDesk.bottom)
		{
			bFullScreen = true;
		}
	}
	return bFullScreen;
}

LRESULT WINAPI KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if(code == HC_ACTION && wParam == VK_HOME && GetKeyState(VK_HOME) < 0)
	{
		if(!g_pDlg)
		{
			//创建窗口
			g_pDlg = new CDialogMain;
			g_pDlg->Create(IDD_DIALOG1);
			
			HWND hWndGame = ::FindWindow(NULL, GAME_WIN_NAME);
			if(!IsFullScreenWindow(hWndGame))
			{
				SetWindowPos(hWndGame, HWND_TOP, 0 + 10, 0 + 10, -1, -1, SWP_NOSIZE);
				RECT rcGame;
				::GetWindowRect(hWndGame, &rcGame);
				::SetWindowPos(g_pDlg->GetSafeHwnd(), HWND_TOP, rcGame.right + 0, 10,
					-1, -1, SWP_NOSIZE);
			} else
			{
				AfxMessageBox(_T("去掉游戏 “全屏”设置"));
			}
		}
		g_pDlg->ShowWindow(SW_SHOW);
		return 0;
	} else
		return CallNextHookEx(NULL, code, wParam, lParam);
}

void __stdcall SetHook()
{
	HWND hWndGame;
	hWndGame = ::FindWindow(NULL, GAME_WIN_NAME);

	DWORD dwProcessId, dwThreadId;
	dwThreadId = ::GetWindowThreadProcessId(hWndGame, &dwProcessId);

	g_hHook = ::SetWindowsHookEx(WH_KEYBOARD,	//键盘钩子
		KeyboardProc,	//钩子过程
		GetModuleHandle(_T("pvz.dll")),//钩子函数所在的模块
		dwThreadId);//游戏窗口线程ID

}