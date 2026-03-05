#pragma once


// DjAnalyse 对话框

class DjAnalyse : public CDialog
{
	DECLARE_DYNAMIC(DjAnalyse)

public:
	DjAnalyse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DjAnalyse();

// 对话框数据
	enum { IDD = IDD_DJAnalyse };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString ResultLayer;
	int layer1Count,layer2Count,selectIndex;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
