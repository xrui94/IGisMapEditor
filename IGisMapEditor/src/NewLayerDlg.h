#pragma once
#include "afxwin.h"


// CNewLayerDlg 对话框

class CNewLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewLayerDlg)

public:
	CNewLayerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewLayerDlg();

// 对话框数据
	enum { IDD = IDD_NEWLAYERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_name;
	CComboBox m_combo;
	int m_nindex;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
