#pragma once
#include "afxcmn.h"
#include "DataTemplate.h"
#include "Cordins.h"
#include "MainFrm.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"

// MarkerAttri 对话框

class MarkerAttri : public CDialog
{
	DECLARE_DYNAMIC(MarkerAttri)

public:
	MarkerAttri(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MarkerAttri();

// 对话框数据
	enum { IDD = IDD_NoteAttri };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl MarkerTable;
	afx_msg void OnBnClickedAlterbnt();
	afx_msg void OnBnClickedDelbnt();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
public:
	gisMarker * gm,selectgm,oldgm;
	int selectIndex;
	CString selectID;
	Layer * layer;
	int count;
	int oldIndex;
	CZoomManager * pZoom;
	void flash(CDC * pDC);
	void endflash(CDC * pDC);
	bool hasInit;
	void getFontStyle(gisMarker &gl,CString width,CString fn);;
	int getMarker(gisMarker&gl,CString id);
	int CurrentColor;
};
