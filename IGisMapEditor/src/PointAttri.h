#pragma once
#include "afxcmn.h"
#include "DataTemplate.h"
#include "AlterBaseTool.h"
#include "Cordins.h"
#include "MainFrm.h"

// PointAttri 对话框

class PointAttri : public CDialog
{
	DECLARE_DYNAMIC(PointAttri)

public:
	PointAttri(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PointAttri();

// 对话框数据
	enum { IDD = IDD_PointAttri };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAlterbnt();
	afx_msg void OnBnClickedDelbnt();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
public:
	gisPoint * gp,selectgp,oldgp;
	int selectIndex;
	CString selectID;
	Layer * layer;
	int count;
	int oldIndex;
	CZoomManager * pZoom;
	void flash(CDC * pDC);
	void endflash(CDC * pDC);
	bool hasInit;
	int getPoint(gisPoint&gp,CString id);
	CListCtrl PointTable;
};
