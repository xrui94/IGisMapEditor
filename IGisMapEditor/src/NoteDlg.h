#pragma once


// NoteDlg 对话框

class NoteDlg : public CDialog
{
	DECLARE_DYNAMIC(NoteDlg)

public:
	NoteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NoteDlg();

// 对话框数据
	enum { IDD = IDD_SETFONT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFontstyle();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ViewFont();
	CString m_lable;
	CStatic m_show;
	LOGFONT m_font;
	COLORREF m_color;
	CPoint pos;
};
