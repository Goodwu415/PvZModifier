
// MemoryCheatDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MemoryCheat.h"
#include "MemoryCheatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMemoryCheatDlg 对话框



CMemoryCheatDlg::CMemoryCheatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEMORYCHEAT_DIALOG, pParent)
	, m_strDesEdit(_T(""))
	, m_strValueTypeEdit(_T(""))
	, m_strValueEdit(_T(""))
	, m_strAddressEdit(_T(""))
	, m_strSearchValue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemoryCheatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DES, m_strDesEdit);
	DDX_Control(pDX, IDC_PROGRESS_SEARCH, m_pProcess);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_strAddressEdit);
	DDX_CBString(pDX, IDC_COMBO_VALUE_TYPE2, m_strValueTypeEdit);
	DDX_Control(pDX, IDC_COMBO_VALUE_TYPE2, m_cbbValueTypeEdit);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_strValueEdit);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_strAddressEdit);
	DDX_Text(pDX, IDC_EDIT_SEARCH_VALUE, m_strSearchValue);
}

BEGIN_MESSAGE_MAP(CMemoryCheatDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MFCSHELLLIST1, &CMemoryCheatDlg::OnLvnItemchangedMfcshelllist1)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMemoryCheatDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMemoryCheatDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CMemoryCheatDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_PLANT_INJECT, &CMemoryCheatDlg::OnBnClickedButtonPlantInject)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CMemoryCheatDlg::OnBnClickedButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CMemoryCheatDlg::OnBnClickedButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CMemoryCheatDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMemoryCheatDlg::OnBnClickedButtonStop)
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




{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
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
	// TODO: 在此添加控件通知处理程序代码
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
