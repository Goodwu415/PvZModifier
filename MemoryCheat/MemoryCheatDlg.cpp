
// MemoryCheatDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MemoryCheat.h"
#include "MemoryCheatDlg.h"
#include "afxdialogex.h"
#include "CDialogProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//回调函数 指示 是否继续搜索
static bool* g_pGoon = nullptr;
//进度列表的范围（0 ~rage)
static int range = 100;
//首次扫描回调函数
bool __stdcall FirstSearchRoutine(void* pArgs, size_t nPageCount, size_t index)
{
	//CProgressCtrl *p = (CProgressCtrl*)pArgs;
	CProgressCtrl *p = static_cast<CProgressCtrl*>(pArgs);
	if(nPageCount == 0)
		return *g_pGoon;
	p->SetPos(static_cast<int>(index / (nPageCount / float(range))));
	return *g_pGoon;
}
//下次扫描回调函数
bool __stdcall NextSearchRoutine(void* pArgs, size_t addrCount, size_t index)
{
	CProgressCtrl* p = (CProgressCtrl*)pArgs;
	if(addrCount == 0)
		return *g_pGoon;
	p->SetPos(static_cast<int>(index / (addrCount / float(range))));
	return *g_pGoon;
}
// CMemoryCheatDlg 对话框



CMemoryCheatDlg::CMemoryCheatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEMORYCHEAT_DIALOG, pParent)
	, m_strSearchValue(_T(""))
	, m_strValueType(_T(""))
	, m_strLimitBegin(_T(""))
	, m_strLimitEnd(_T(""))
	, m_strDesEdit(_T(""))
	, m_strValueTypeEdit(_T(""))
	, m_strValueEdit(_T(""))
	, m_strAddressEdit(_T(""))
	
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pGoon = &m_bGoon;
}

void CMemoryCheatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADDRESS_TEMP, m_lstAddressTemp);
	DDX_Control(pDX, IDC_LIST_ADDRESS_TARGET, m_lstAddressTarget);
	DDX_CBString(pDX, IDC_COMBO_VALUE_TYPE, m_strValueType);
	DDX_Control(pDX, IDC_COMBO_VALUE_TYPE, m_cbbValueType);
	DDX_Text(pDX, IDC_EDIT_LIMIT_START, m_strLimitBegin);
	DDX_Text(pDX, IDC_EDIT_LIMIT_END, m_strLimitEnd);
	DDX_Text(pDX, IDC_EDIT_DES, m_strDesEdit);
	DDX_Control(pDX, IDC_PROGRESS_SEARCH, m_pProcess);
	DDX_CBString(pDX, IDC_COMBO_VALUE_TYPE2, m_strValueTypeEdit);
	DDX_Control(pDX, IDC_COMBO_VALUE_TYPE2, m_cbbValueTypeEdit);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_strValueEdit);
	DDX_Text(pDX, IDC_EDIT_SEARCH_VALUE, m_strSearchValue);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_strAddressEdit);
}

BEGIN_MESSAGE_MAP(CMemoryCheatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMemoryCheatDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMemoryCheatDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CMemoryCheatDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_PLANT_INJECT, &CMemoryCheatDlg::OnBnClickedButtonPlantInject)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CMemoryCheatDlg::OnBnClickedButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CMemoryCheatDlg::OnBnClickedButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CMemoryCheatDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMemoryCheatDlg::OnBnClickedButtonStop)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ADDRESS_TEMP, &CMemoryCheatDlg::OnNMDblclkListAddressTemp)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ADDRESS_TARGET, &CMemoryCheatDlg::OnNMClickListAddressTarget)
	ON_CBN_SELCHANGE(IDC_COMBO_VALUE_TYPE2, &CMemoryCheatDlg::OnCbnSelchangeComboValueTypeEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMemoryCheatDlg 消息处理程序

BOOL CMemoryCheatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//界面初始化
	{
		{//临时数据 列表
			CListCtrl& m_lst = m_lstAddressTemp;
			LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
			lStyle &= ~LVS_TYPEMASK;
			lStyle |= LVS_REPORT;
			SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
			DWORD dwStyle = m_lst.GetExtendedStyle();
			dwStyle |= LVS_EX_FULLROWSELECT;//选中行， 整行高亮
			dwStyle |= LVS_EX_GRIDLINES;	//网格线
			{//设置列 大小
				CRect rc;
				m_lst.GetClientRect(rc);
				m_lst.InsertColumn(EListTempIndexAddress, _T("地址"), LVCFMT_LEFT, 80);
			}
		}
		{//保留数据列表
			CListCtrl& m_lst = m_lstAddressTarget;
			LONG lStyle = GetWindowLong(m_lst.m_hWnd, GWL_STYLE);
			lStyle &= ~LVS_TYPEMASK;
			lStyle |= LVS_REPORT;
			SetWindowLong(m_lst.GetSafeHwnd(), GWL_STYLE, lStyle);
			DWORD dwStyle = m_lst.GetExtendedStyle();
			dwStyle |= LVS_EX_FULLROWSELECT;
			dwStyle |= LVS_EX_GRIDLINES;
			m_lst.SetExtendedStyle(dwStyle);
			{
				CRect rc;
				m_lst.GetClientRect(rc);
				m_lst.InsertColumn(EListTargetIndexDescription, _T("说明"), LVCFMT_LEFT, 80);
				m_lst.InsertColumn(EListTargetIndexAddress, _T("地址"), LVCFMT_LEFT, 80);
				m_lst.InsertColumn(EListTargetIndexType, _T("类型"), LVCFMT_LEFT, 80);
				m_lst.InsertColumn(EListTargetIndexValue, _T("值"), LVCFMT_LEFT, 80);
			}
		}
		{//值类型
			auto& ss = m_indexValueTypeArray;
			ss[0].index = 0, ss[0].strValueType.LoadString(IDS_STRING_1BYTE);
			ss[1].index = 1, ss[1].strValueType.LoadString(IDS_STRING_2BYTE);
			ss[2].index = 2, ss[2].strValueType.LoadString(IDS_STRING_4BYTE);
			ss[3].index = 3, ss[3].strValueType.LoadString(IDS_STRING_FLOAT);
			ss[4].index = 4, ss[4].strValueType.LoadString(IDS_STRING_DOUBLE);
		}
		{//搜索 值类型
			for(auto km : m_indexValueTypeArray)
			{
				m_cbbValueType.AddString(km.strValueType);
			}
			m_cbbValueType.SetCurSel(2);
		}
		{//编辑值类型
			for(auto km : m_indexValueTypeArray)
				m_cbbValueTypeEdit.AddString(km.strValueType);
			m_cbbValueTypeEdit.SetCurSel(2);
		}
		// 设置搜索回调函数
		// 传送的指针为 进度条
		m_pFinder->SetCallbackFirst(FirstSearchRoutine, &m_pProcess);
		m_pFinder->SetCallbackNext(NextSearchRoutine, &m_pProcess);
	}
	// 启动定时器 : 每秒更新一次 目标地址框中的数据
	SetTimer(1, 1000, nullptr);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMemoryCheatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMemoryCheatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMemoryCheatDlg::PreTranslateMessage(MSG* pMsg)
{
	// 防止 ESC/回车键 自动退出
	if(pMsg->message == WM_KEYFIRST)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;// 返回非0,代表此消息不继续进行分发
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMemoryCheatDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMemoryCheatDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMemoryCheatDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMemoryCheatDlg::OnBnClickedButtonPlantInject()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMemoryCheatDlg::OnBnClickedButtonProcess()
{
	//显示进程列表对话框
	CDialogProgress dlg(this);
	if(IDOK != dlg.DoModal())
	{
		m_dwProcessId = 0;
		return;
	}
	m_dwProcessId = CDialogProgress::m_dwProcessId;
	// 把本窗口设置为 目标进程名
	SetWindowText(CDialogProgress::m_strProcessName);
	// 打开进程
	m_pFinder->OpenProcess(m_dwProcessId);
}


void CMemoryCheatDlg::OnBnClickedButtonFirst()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMemoryCheatDlg::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMemoryCheatDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
}
