
// IGisMapEditorView.h : CIGisMapEditorView 类的接口
//

#pragma once

#include "FileView.h"
#include "GeoPoint.h"
#include "Cordins.h"
#include "GraphStyles.h"
#include "GraphTool.h"
#include "DataTools.h"
#include "AlterBaseTool.h"
#include "SearchTool.h"
#include "IGisMapEditorDoc.h"

class CIGisMapEditorView : public CView
{
protected: // 仅从序列化创建
	CIGisMapEditorView();
	DECLARE_DYNCREATE(CIGisMapEditorView)

// 特性
public:
	CIGisMapEditorDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CIGisMapEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CVisibleManager* GetVisibleManager(void);
	CMainFrame* GetMain(void);
	Layer* GetEditingLayerPtr(void);
	Basepic* GetVisibleMapPtr(void);
	int CheckEditing(void);
	void Refresh();
	CZoomManager MapZoom;
	CStyleManager Styles;
	bool needCordin;
	inline int GetStatus(){return status;}
protected:
	CZoomTool* pZoom;
	CDataTool* pData;
	CBaseGisTool* pDraw; 
	AlterBaseTool * pAlterTool;
	SearchTool * pSearchTool;
	int status;
protected:
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnZoommove();
	afx_msg void OnRedraw();
	afx_msg void OnRefreshview();
	afx_msg void OnCreatepoint();
	afx_msg void OnCreatenote();
	afx_msg void OnCreateline();
	afx_msg void OnCreateregion();
	afx_msg void OnAlterPoint();
	afx_msg void OnMovePoint();
	afx_msg void OnDeletePoint();
	afx_msg void OnAlterMarker();
	afx_msg void OnMoveMarker();
	afx_msg void OnDeleteMarker();
	afx_msg void OnAlterLine();
	afx_msg void OnMoveLine();
	afx_msg void OnDeleteLine();
	afx_msg void OnCutLine();
	afx_msg void OnAddPointToLine();
	afx_msg void OnDelLinePt();
	afx_msg void OnAlterRegion();
	afx_msg void OnMoveRegion();
	afx_msg void OnDeleteRegion();
	afx_msg void OnDelRgnPt();
	afx_msg void OnAddRgnPt();
	afx_msg void OnSearchByRect();
	afx_msg void OnSearchByCircle();
	afx_msg void OnSearchByPolygon();
	afx_msg void OnAlterMenuAttr();
	afx_msg void OnAlterMenuMove();
	afx_msg void OnAlterMenuDelete();
	CString AlterType;
	CString SearchType;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMoveLinePt();
	afx_msg void OnMovePolygonPt();
	afx_msg void OnLineDjPolygon();
	afx_msg void OnPolyDjPoly();
	afx_msg void OnHcAnalyse();
	afx_msg void OnSavepicture();
};

#ifndef _DEBUG  // IGisMapEditorView.cpp 中的调试版本
inline CIGisMapEditorDoc* CIGisMapEditorView::GetDocument() const
   { return reinterpret_cast<CIGisMapEditorDoc*>(m_pDocument); }
#endif

