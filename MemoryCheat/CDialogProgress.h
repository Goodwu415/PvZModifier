#pragma once


// CDialogProgress 对话框

class CDialogProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogProgress)

public:
	CDialogProgress(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogProgress();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROGRESS_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//32位进程列表
	CListCtrl m_lst;
	//进程列表的图标
	CImageList m_imgList;
	//获取32位进程，并更新到进程列中
	BOOL GetProcessList();
public:
	static DWORD m_dwProcessId;//当前选中的进程ID
	static CString m_strProcessName;//选中的进程名
	

	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
