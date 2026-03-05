#pragma once

#include "DataTemplate.h"
#include "DataBase.h"
#include "Cordins.h"
#include "afxcmn.h"

// PolygonView 对话框

class PolygonView : public CDialogEx
{
	DECLARE_DYNAMIC(PolygonView)

public:
	PolygonView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PolygonView();

// 对话框数据
	enum { IDD = IDD_PolygonView };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
public:
	gisPolygon * gPoly,selectgPoly,oldgPoly;
	int selectIndex;
	CString selectID;
	CString table;
	int count;
	int oldIndex;
	CZoomManager * pZoom;
	void flash(CDC * pDC);
	void endflash(CDC * pDC);
	bool hasInit;
	CString getBorderType(gisPolygon &g);
	virtual void OnCancel();
	CListCtrl PolyView;
	afx_msg void OnBnClickedButton2();
	int getPolygon(gisPolygon & g,CString id);
};
