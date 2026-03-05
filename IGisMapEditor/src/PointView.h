#pragma once
#include "afxcmn.h"
#include "DataTemplate.h"
#include "AlterBaseTool.h"
#include "Cordins.h"

// PointView 对话框

class PointView : public CDialog
{
	DECLARE_DYNAMIC(PointView)

public:
	PointView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PointView();

// 对话框数据
	enum { IDD = IDD_PointView };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_PointSet;
	afx_msg void OnBnClickedEditbnt();
	afx_msg void OnBnClickedDeletebnt();
	afx_msg void OnBnClickedOk();
	int getPoint(gisPoint&gp,CString id);
public:
	gisPoint * gp,selectgp,oldgp;
	int selectIndex;
	CString selectID;
	CString table;
	int count;
	int oldIndex;
	CZoomManager * pZoom;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void flash(CDC * pDC);
	void endflash(CDC * pDC);
	virtual BOOL OnInitDialog();
	bool hasInit;
	virtual void OnCancel();
};
