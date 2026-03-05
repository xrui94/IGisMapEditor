
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "DataBase.h"
class CIGisMapEditorView;
class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;

	CMFCToolBar       m_wndBar;//toolbar
	CMFCToolBar       m_zoomBar;//toolbar
	CMFCToolBar       m_editBar;//toolbar
	CMFCToolBar       m_extBar;//toolbar
	NOTIFYICONDATA m_ntIcon;
	bool m_bMin;//if minimized
	//
public:
	CVisibleManager VManager;
	CString dbname;
	CString server;
	CString user;
	CString code;
	DBConnection db;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnNotifyIcon(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	CVisibleManager* GetVisibles(void);
	CIGisMapEditorView* GetView(void);
	void showXy(double x,double y);
	void showStatus(int s);
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNotifyClose();
	afx_msg void OnClose();
	afx_msg void OnAppExit();

	afx_msg void OnSetdbconnect();
	afx_msg void OnClearconnection();

	Layer* GetEditingLayerPtr(void);
	Basepic* GetVisibleMapPtr(void);
	CFileView* GetTree(void);
	afx_msg void OnCreatedb();
	afx_msg void OnDeletetable();
	void Disconn(bool nocall=false);
};


