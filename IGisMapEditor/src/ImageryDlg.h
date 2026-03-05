#pragma once
#include "Cordins.h"
#include "DataBase.h"
#include "afxwin.h"

// CImageryDlg 对话框

class CImageryDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageryDlg)

public:
	CImageryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageryDlg();

// 对话框数据
	enum { IDD = IDD_SavePicDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_height;
	int m_width;
	double m_xmin;
	double m_ymin;
	double m_xmax;
	double m_ymax;
	virtual BOOL OnInitDialog();
	CComboBox m_mapunit;
	CComboBox m_itemunit;
	CComboBox m_bpp;
	bool mappix;
	bool itempix;
	bool bpp24;	
	DBConnection* db;
	CZoomManager *pZoom;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedOk();	
};
