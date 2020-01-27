// CDialogProgress.cpp: 实现文件
//

#include "stdafx.h"
#include "MemoryCheat.h"
#include "CDialogProgress.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(CDialogProgress, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDialogProgress::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDialogProgress 消息处理程序


void CDialogProgress::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
