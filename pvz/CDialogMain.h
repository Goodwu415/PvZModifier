#pragma once
#include "afxwin.h"

// CDialogMain 对话框

class CDialogMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogMain)

public:
	CDialogMain(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogMain();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 设置 种植物没有CD
	afx_msg void OnBnClickedButtonNocd();
	// 设置 可以重叠种植
	afx_msg void OnBnClickedButtonAnywhere();
	// 设置 可以自动收集阳光，金钱
	afx_msg void OnBnClickedButtonAutoCollect();
	// 设置 僵尸全部出动
	afx_msg void OnBnClickedButtonAllZombies();
	DWORD m_dwSunlight;
	afx_msg void OnBnClickedButtonSunlight();
	DWORD m_dwGold;
	afx_msg void OnBnClickedButtonGold();
	// 要种植的植物
	CComboBox m_comboPlants;
	// 坐标 x
	CComboBox m_comboCropX;
	// 坐标 y
	CComboBox m_comboCropY;
	afx_msg void OnBnClickedButtonCrop();
	afx_msg void OnBnClickedButtonCropRow();
	afx_msg void OnBnClickedButtonCol();
	afx_msg void OnBnClickedButtonRemoveplant();
	CComboBox m_comboPlants2;
	afx_msg void OnCbnSelchangeComboPlants();
	DWORD GetPlants2Id();
};
