// CDialogProgress.cpp: 实现文件
//

#include "stdafx.h"
#include "MemoryCheat.h"
#include "CDialogProgress.h"
#include "afxdialogex.h"

#include <TlHelp32.h>

namespace hh
{//判断os是否是64位的
	BOOL IsOs64()
	{
		SYSTEM_INFO inf;
		GetNativeSystemInfo(&inf);
		if(inf.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64
			|| inf.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64){
			return TRUE;
		} else
			return FALSE;
	}
	//判断是否是32位进程
	BOOL Is32BitProcess(DWORD dwProcessId)
	{
		if(!hh::IsOs64())//如果不是64位系统
			return TRUE;
		if(dwProcessId == 0 || dwProcessId == 4)
			return FALSE;
		//判断是否是32位系统
		typedef BOOL(WINAPI* LPEN_ISWOW64PROCESS)(HANDLE, PBOOL);
		static LPEN_ISWOW64PROCESS fnIsWow64Process = (LPEN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(_T("kernel32")), "IsWow64Process");
		if(!fnIsWow64Process)//系统是32位的，进程也就是32位
			return TRUE;
		//判断是否是64位系统下面的32位进程
		HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
		if(!h || h == INVALID_HANDLE_VALUE)
			return FALSE;
		BOOL b32 = 0;
		BOOL b = fnIsWow64Process(h, &b32);
		CloseHandle(h);
		return b32;
	}
	//获得进程完整的路径
	BOOL GetFullPathProcess(DWORD dwProcessId, TCHAR szBuffer[], DWORD dwMaxLen)
	{
		HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
		if(h && h != INVALID_HANDLE_VALUE)
		{
			DWORD dwLen = dwMaxLen;
			BOOL b = ::QueryFullProcessImageName(h, 0, szBuffer, (PDWORD)&dwLen);
			if(b)
			{
				szBuffer[dwLen] = _T('\0');
			}
			CloseHandle(h);
			return b;
		}
		return FALSE;
	}
	BOOL GetProcessIco(DWORD dwProcessId, HICON & hico)
	{
		TCHAR szPath[MAX_PATH] = { 0 };
		if(hh::GetFullPathProcess(dwProcessId, szPath, sizeof(szPath)))
		{
			SHFILEINFO info = {0};
			DWORD_PTR dwRet = ::SHGetFileInfo(szPath, 0, &info, sizeof(info), SHGFI_ICON | SHGFI_LARGEICON);
			if(dwRet == 0)
				hico = 0;
			else
				hico = info.hIcon;
		}
	}
}

DWORD CDialogProgress::m_dwProcessId = 0;
CString CDialogProgress::m_strProcessName = _T("");

// CDialogProgress 对话框

IMPLEMENT_DYNAMIC(CDialogProgress, CDialogEx)

CDialogProgress::CDialogProgress(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROGRESS_LIST, pParent)
{

}

CDialogProgress::~CDialogProgress()
{
}

void CDialogProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lst);
}


BEGIN_MESSAGE_MAP(CDialogProgress, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDialogProgress::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDialogProgress 消息处理程序

//获取32位进程，并更新到进程列中
BOOL CDialogProgress::GetProcessList()
{
	// 通过进程快照,遍历进程
	// 并判断进程是否是32位进程
	// 如果是32位进程,则加入进程列表m_lst中
	HANDLE hProcessSnap;//快照
	PROCESSENTRY32 pe32;//判断是否是32位进程
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
		return FALSE; //创建快照失败
	//获得第一个进程
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(hProcessSnap, &pe32))
	{//失败
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	do{
		//判断是否是32位进程
		if(hh::Is32BitProcess(pe32.th32ProcessID))
		{
			int indexIco = -1;
			{//获取进程图标，并放入到m_imgList中
				HICON hIco = 0;
				if(hh::GetProcessIco(pe32.th32ProcessID, hIco))
				{
					indexIco = m_imgList.Add(hIco);
				}
			}
			{//插入新行
				int index = m_lst.InsertItem(m_lst.GetItemCount(), pe32.szExeFile, indexIco);
				CString id;
				id.Format(_T("%d"), pe32.th32ProcessID);
				m_lst.SetItemText(index, 1, id);//进程ID
				m_lst.SetItemData(index, (DWORD_PTR)pe32.th32ProcessID);//设置该行数据
			}
		}else
		{ }
	} while(Process32Next(hProcessSnap, &pe32));//获取下一个进程
	//关闭快照句柄
	CloseHandle(hProcessSnap);
	return TRUE;
}

BOOL CDialogProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//控件
	{
		LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
		lStyle &= ~LVS_TYPEMASK;
		lStyle |= LVS_REPORT;
		SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
		DWORD dwStyle = m_lst.GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;//选中行，整行高亮
		dwStyle |= LVS_EX_GRIDLINES; //网格线
		m_lst.SetExtendedStyle(dwStyle);
		//设置列，并设置大小
		{
			CRect rc;
			m_lst.GetClientRect(rc);
			m_lst.InsertColumn(0, _T("进程名"), LVCFMT_LEFT, rc.Width() / 2);
			m_lst.InsertColumn(1, _T("进程ID"), LVCFMT_LEFT, rc.Width() / 2);
		}
		// 设置控件关联的图标列表,这样才可以在每行的开头显示图标
		m_imgList.Create(16, 16, ILC_COLOR32, 1, 1);
		m_lst.SetImageList(&m_imgList, LVSIL_SMALL);
	}
	//列出进程
	GetProcessList();
	return TRUE;
}

void CDialogProgress::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMItemActivate->iItem;
	// 如果有选中的行,设置回传给父窗口的变量,使父窗口可以知道当前选择的进程信息
	if(nItem >= 0)
	{
		m_dwProcessId = (DWORD)m_lst.GetItemData(nItem);
		m_strProcessName = m_lst.GetItemText(nItem, 0);
		OnOK();
	}
	*pResult = 0;
}
