
#pragma once

#include "ViewTree.h"
#include "treelayers.h"
#include "Cordins.h"
//
class CMainFrame;
class CIGisMapEditorView;
//
#include <vector>
using namespace std;

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 构造
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();
//layers...
public:
	TreeItems thelayers;
	ImageItems thepics;
// 特性
public:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;
	HTREEITEM hRoot,hImages,hLayers;
protected:
	void FillFileView();

// 实现
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
/*	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();*/
	afx_msg void OnExport();
/*	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();*/
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNewlayer();
	afx_msg void OnAddlayer();
	afx_msg void OnDellayer();
	afx_msg void OnVisiblelayer();
	afx_msg void OnEditlayer();
	afx_msg void OnUpdateVisiblelayer(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditlayer(CCmdUI *pCmdUI);
	afx_msg void OnAddbmp();
	afx_msg void OnViewbmp();
	afx_msg void OnUpdateViewbmp(CCmdUI *pCmdUI);
	afx_msg void OnDelbmp();
	//============================================funcs and outside calls
	int GetSelectedIndex(void);
	CMainFrame* GetMain(void);
	CIGisMapEditorView* GetView(void);
	int GetEditingIndex(void);
	int GetVisiblePicIndex(void);
	Layer* GetEditingPtr(void);
	Basepic* GetViewpicPtr(void);
	void RemoveAll(bool call=true);
	void addBmpEx(CString pathname,double ltx,double lty,double rbx,double rby);
	void addLayerEx(CString fname,CString server,CString name,CString user,CString pin);
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	afx_msg void OnAttriTable();
	bool isUsing(CString lname)
	{
		int n=thelayers.size();
		for(int i=0;i<n;i++)
		{
			Layer*p =thelayers.LayerPtr(i);
			if(p->GetName()==lname){return true;}
		}
		return false;
	}
	afx_msg void OnShowall();
	afx_msg void OnHideall();
	afx_msg void OnImport();
};

//=======================================================================================================================
class CVisibleManager
{
public: CVisibleManager(){curlayer=NULL;curpic=NULL;ptr=NULL;}
		~CVisibleManager(){curlayer=NULL;curpic=NULL;ptr=NULL;}

		bool None(){return curlayer==NULL&&curpic==NULL;}
		int GetLayerIndex(Layer *p)
		{
			int nindex=0;
			while(nindex<(ptr->thelayers.size()))
			{
				if(p==ptr->thelayers.LayerPtr(nindex))
				{return nindex;}
				nindex++;
			}
			return -1;
		}
		void Init(CFileView &obj){SetObject(obj);GetFirstVisibleLayer();GetBaseMap();}
		void Release(){curlayer=NULL;curpic=NULL;ptr=NULL;}
		Layer* first(){return curlayer;}
		Layer* next(Layer *p)
		{
			int n=GetLayerIndex(p);
			if(n<0)
			{return NULL;}
			else
			{
				n++;
				Layer *temp=ptr->thelayers.LayerPtr(n);
				while(n<ptr->thelayers.size())
				{
					if(temp->IsVisible())
					{return temp;}
					n++;
					temp=ptr->thelayers.LayerPtr(n);
				}
				return NULL;
			}

		}
		Basepic* GetMap(){return curpic;}
protected:
		void SetObject(CFileView &obj){ptr=&obj;curlayer=NULL;curpic=NULL;;}
		Layer* GetFirstVisibleLayer()//Init 
		{
			if(ptr->thelayers.size()>0)
			{
				int nindex=0;
				Layer *temp=ptr->thelayers.LayerPtr(nindex);
				while(nindex<ptr->thelayers.size())
				{
					if(temp->IsVisible())
					{curlayer=temp;return temp;}
					nindex++;
					temp=ptr->thelayers.LayerPtr(nindex);
				}
				return NULL;
			}
			else
			{return NULL;}
		}
		Basepic* GetBaseMap()
		{
			int nindex=0;
			if(ptr->thepics.size()>0)
			{
				Basepic *temp=ptr->thepics.PicPtr(nindex);
				while(nindex<ptr->thepics.size())
				{
					if(temp->IsVisible())
					{curpic=temp;return temp;}
					nindex++;
					temp=ptr->thepics.PicPtr(nindex);
				}
				return NULL;
			}
			else
			return NULL;
		}
private:
	Layer *curlayer;
	Basepic* curpic;
	CFileView *ptr;
};