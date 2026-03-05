#pragma once
#include "afxwin.h"


// CLineGraphDlg 对话框

class CLineGraphDlg : public CDialog
{
	DECLARE_DYNAMIC(CLineGraphDlg)

public:
	CLineGraphDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLineGraphDlg();

// 对话框数据
	enum { IDD = IDD_LineReserve };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_see;
	afx_msg void OnBnClickedRiver();
	afx_msg void OnBnClickedDryriver();
	afx_msg void OnBnClickedPipe();
	afx_msg void OnBnClickedNationalroad();
	afx_msg void OnBnClickedProvinceroad();
	afx_msg void OnBnClickedCityroad();
	afx_msg void OnBnClickedCountryroad();
	afx_msg void OnBnClickedVillageroad();
	afx_msg void OnBnClickedProvinceborder();
	afx_msg void OnBnClickedCityborder();
	int penstyle;
	COLORREF color;
	void View();
	afx_msg void OnBnClickedOk();
};
