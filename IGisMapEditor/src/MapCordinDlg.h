#pragma once
#include "afxwin.h"


// CMapCordinDlg 对话框

class CMapCordinDlg : public CDialog
{
	DECLARE_DYNAMIC(CMapCordinDlg)

public:
	CMapCordinDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMapCordinDlg();

// 对话框数据
	enum { IDD = IDD_SETCORDINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_ltx;
	double m_lty;
	double m_rbx;
	double m_rby;
	CString pathname;
	bool shown;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CStatic m_map;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
};
