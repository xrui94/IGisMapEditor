#pragma once
#include "afxcmn.h"
#include "DataTemplate.h"
#include "Cordins.h"
#include "MainFrm.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"

// LineAttri 对话框

class LineAttri : public CDialog
{
	DECLARE_DYNAMIC(LineAttri)

public:
	LineAttri(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LineAttri();

// 对话框数据
	enum { IDD = IDD_LineAttri };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl LineTable;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelbnt();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAlterline();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
public:
	gisLine * gl,selectgl,oldgl;
	int selectIndex;
	CString selectID;
	Layer * layer;
	int count;
	int oldIndex;
	CZoomManager * pZoom;
	void flash(CDC * pDC);
	void endflash(CDC * pDC);
	bool hasInit;
	CString getLineType(gisLine &gl);
	int getLine(gisLine & gl,CString id);
};
