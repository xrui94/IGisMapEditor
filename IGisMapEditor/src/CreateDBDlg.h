#pragma once


// CCreateDBDlg 对话框

class CCreateDBDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateDBDlg)

public:
	CCreateDBDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateDBDlg();

// 对话框数据
	enum { IDD = IDD_CREATEDBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_server;
	CString m_user;
	CString m_pin;
	CString m_name;
	CString m_path;
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedOk();
};
