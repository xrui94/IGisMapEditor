#pragma once

#include "DataTemplate.h"
#include "Cordins.h"
#include "afxcmn.h"

// MarkerView 对话框

class MarkerView : public CDialog
{
	DECLARE_DYNAMIC(MarkerView)

public:
	MarkerView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MarkerView();

// 对话框数据
	enum { IDD = IDD_MArkerView };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEditbnt();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDeletebnt();
public:
	gisMarker * gm,selectgm,oldgm;
	int selectIndex;
	CString selectID;
	CString table;
	int count;
	int oldIndex;
	CZoomManager * pZoom;
	void flash(CDC * pDC);
	void endflash(CDC * pDC);
	bool hasInit;
	void getFontStyle(gisMarker &gl,CString width,CString fn);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	int getMarker(gisMarker&gl,CString id);
	CListCtrl MarkerSet;
	int CurrentColor;
	virtual void OnCancel();
};
