#pragma once
#include "DataBase.h"

// CConnectDbDlg 对话框

class CConnectDbDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDbDlg)

public:
	CConnectDbDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConnectDbDlg();

// 对话框数据
	enum { IDD = IDD_CONNDBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_server;
	CString m_name;
	CString m_user;
	CString m_pin;
	BOOL m_remember;
	BOOL m_rememberPin;
	DBConnection *pdb;
	virtual BOOL OnInitDialog();
};
