#pragma once
#include "afxcmn.h"


// CAddLayerDlg 对话框

class CAddLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddLayerDlg)

public:
	CAddLayerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddLayerDlg();

// 对话框数据
	enum { IDD = IDD_ADDLAYERDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CListCtrl m_lists;
public:
	CString m_selectedinfo;
	int m_selindex;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString server;
	CString name;
	CString user;
	CString pin;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	int m_selid;
};
