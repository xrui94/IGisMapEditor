#pragma once
#include "afxcmn.h"
#include "DataBase.h"
#include "FileView.h"

// CDeleteDlg 对话框

class CDeleteDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeleteDlg)

public:
	CDeleteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeleteDlg();

// 对话框数据
	enum { IDD = IDD_DELETEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_viewtable;
	int nindex;
	CString lname;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CFileView *pView;
	DBConnection* db;
};
