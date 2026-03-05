
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "IGisMapEditor.h"
#include "IGisMapEditorView.h"
#include "DataBase.h"

#include "SColorDlg.h"
#include "NewLayerDlg.h"
#include "MapCordinDlg.h"
#include "AddLayerDlg.h"
#include "ConnectDbDlg.h"
#include "LineAttri.h"
#include "PointAttri.h"
#include "PolyAttri.h"
#include "MarkerAttri.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
/*	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)*/
	ON_COMMAND(ID_DUMMY_COMPILE, OnExport)
/*	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)*/
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_NewLayer, &CFileView::OnNewlayer)
	ON_COMMAND(ID_AddLayer, &CFileView::OnAddlayer)
	ON_COMMAND(ID_DelLayer, &CFileView::OnDellayer)
	ON_COMMAND(ID_VisibleLayer, &CFileView::OnVisiblelayer)
	ON_COMMAND(ID_EditLayer, &CFileView::OnEditlayer)
	ON_UPDATE_COMMAND_UI(ID_VisibleLayer, &CFileView::OnUpdateVisiblelayer)
	ON_UPDATE_COMMAND_UI(ID_EditLayer, &CFileView::OnUpdateEditlayer)
	ON_COMMAND(ID_ADDBMP, &CFileView::OnAddbmp)
	ON_COMMAND(ID_VIEWBMP, &CFileView::OnViewbmp)
	ON_UPDATE_COMMAND_UI(ID_VIEWBMP, &CFileView::OnUpdateViewbmp)
	ON_COMMAND(ID_DELBMP, &CFileView::OnDelbmp)
	ON_COMMAND(ID_MOVEUP, &CFileView::OnMoveup)
	ON_COMMAND(ID_MOVEDOWN, &CFileView::OnMovedown)
	ON_COMMAND(ID_AttriTable, &CFileView::OnAttriTable)
	ON_COMMAND(ID_SHOWALL, &CFileView::OnShowall)
	ON_COMMAND(ID_HIDEALL, &CFileView::OnHideall)
	ON_COMMAND(ID_Import, &CFileView::OnImport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_FULLROWSELECT;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	hRoot = m_wndFileView.InsertItem(_T("地图工程"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	hImages = m_wndFileView.InsertItem(_T("底图"), 1, 1, hRoot);

	hLayers = m_wndFileView.InsertItem(_T("数据图层"), 1, 1, hRoot);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hImages, TVE_EXPAND);
	m_wndFileView.Expand(hLayers, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
			pWndTree->SetFocus();
			if(hTreeItem==hImages||m_wndFileView.GetParentItem(hTreeItem)==hImages)
				theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_SORT, point.x, point.y, this, TRUE);
			else if(hTreeItem==hLayers||m_wndFileView.GetParentItem(hTreeItem)==hLayers)
				theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
			else
			{;}
		}
	}

	//pWndTree->SetFocus();
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	HTREEITEM hitem=m_wndFileView.GetSelectedItem();
	if(hitem==NULL){return;}
	if(hitem==hRoot)
		MessageBox(_T("地图工程管理"),_T("属性"),MB_ICONINFORMATION);
	else if(hitem==hImages)
		MessageBox(_T("底图集管理"),_T("属性"),MB_ICONINFORMATION);
	else if(hitem==hLayers)
		MessageBox(_T("数据图集管理"),_T("属性"),MB_ICONINFORMATION);
	else if(m_wndFileView.GetParentItem(hitem)==hImages)
	{
		Basepic*p=(Basepic*)m_wndFileView.GetItemData(hitem);
		MessageBox(L"底图\n"+p->GetName(),_T("属性"),MB_ICONINFORMATION);;
	}
	else if(m_wndFileView.GetParentItem(hitem)==hLayers)
	{
		Layer*p=(Layer*)m_wndFileView.GetItemData(hitem);
		CString a;int n=p->GetType();
		switch(n)
		{
		case 0:a=L"点图层";break;
		case 1:a=L"线图层";break;
		case 2:a=L"区图层";break;
		case 3:a=L"注记图层";break;
		default:a=L"？图层";break;
		}
		MessageBox(L"数据图\n"+p->GetName()+L"\n"+a,_T("属性"),MB_ICONINFORMATION);
	}
	else
	{
		;
	}
}
/*
void CFileView::OnFileOpen()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnFileOpenWith()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditCut()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditCopy()
{
	// TODO: 在此处添加命令处理程序代码
}

void CFileView::OnEditClear()
{
	// TODO: 在此处添加命令处理程序代码
}
*/
void CFileView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


void CFileView::OnNewlayer()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMain=GetMain();
	if(!pMain->db.isConnection())
	{
		if(IDYES==AfxMessageBox(L"请设置数据连接!",MB_YESNO))
		{
			CConnectDbDlg dlg;
			dlg.pdb=&pMain->db;
			if(IDOK==dlg.DoModal())
			{
				pMain->server=dlg.m_server;
				pMain->dbname=dlg.m_name;
				pMain->user=dlg.m_user;
				pMain->code=dlg.m_pin;
				goto newlayer;
			}
		}	
	}
	else
	{
newlayer:	
		CNewLayerDlg dlg;
		if(IDOK==dlg.DoModal())
		{
			CString fname=dlg.m_name;
			int ntype=dlg.m_nindex;
			if(pMain->db.CreateTable(fname,ntype+1))
			{
				Layer L;
				L.SetEx(ntype,false,false,fname);
				int nsz=thelayers.size();
				thelayers.add(L);
				HTREEITEM hitem1=m_wndFileView.InsertItem(fname,ntype+2,ntype+2,hLayers);
				m_wndFileView.SetItemData(hitem1,(DWORD_PTR)thelayers.LayerPtr(nsz));
			}
		}
	}
	m_wndFileView.Expand(hLayers, TVE_EXPAND);
}


void CFileView::OnAddlayer()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pMain=GetMain();
	if(!pMain->db.isConnection())
	{
		if(AfxMessageBox(L"请设置数据连接!",MB_YESNO)==IDYES)
		{
			CConnectDbDlg dlg;
			dlg.pdb=&pMain->db;
			if(IDOK==dlg.DoModal())
			{
				pMain->server=dlg.m_server;
				pMain->dbname=dlg.m_name;
				pMain->user=dlg.m_user;
				pMain->code=dlg.m_pin;
				goto addlayer;
			}
		}
	}
	else
	{
addlayer:
		CAddLayerDlg dlg;
		CatalogTable table;
		dlg.server=pMain->server;
		dlg.name=pMain->dbname;
		dlg.user=pMain->user;
		dlg.pin=pMain->code;
		if(IDOK==dlg.DoModal())
		{
			int nindex=dlg.m_selid;
			if(pMain->db.GetTableMsg(table,nindex))
			{
				table.tableType--;
				Layer L;
				L.SetEx(table.tableType,false,false,table.tableName);
				int nsz=thelayers.size();
				thelayers.add(L);
				HTREEITEM hitem1=m_wndFileView.InsertItem(table.tableName,table.tableType+2,table.tableType+2,hLayers);
				m_wndFileView.SetItemData(hitem1,(DWORD_PTR)thelayers.LayerPtr(nsz));
			}
		}
	}
	m_wndFileView.Expand(hLayers, TVE_EXPAND);
}

void CFileView::OnExport()
{

}

void CFileView::OnImport()
{
	// TODO: 在此添加命令处理程序代码
}

void CFileView::OnMoveup()
{
	// TODO: 在此处添加命令处理程序代码
	if(thelayers.size()>1)
	{
		HTREEITEM itm=m_wndFileView.GetSelectedItem();
		if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hLayers==m_wndFileView.GetParentItem(itm))
		{
			if(m_wndFileView.GetChildItem(hLayers)==itm)
			{
				MessageBox(L"无法移动，已经处于队列首端",L"不能移动",MB_OK|MB_ICONWARNING);
			}
			else
			{
				HTREEITEM hitem=m_wndFileView.GetNextItem(itm,TVGN_PREVIOUS);
				CString name=m_wndFileView.GetItemText(hitem);
				CString name2=m_wndFileView.GetItemText(itm);
				int img,simg,img2,simg2;
				img=simg=0;
				m_wndFileView.GetItemImage(hitem,img,simg);
				m_wndFileView.GetItemImage(itm,img2,simg2);
				Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
				Layer* q=(Layer*)m_wndFileView.GetItemData(hitem);
				
				thelayers.up(p);
				m_wndFileView.SetItemText(hitem,name2);
				m_wndFileView.SetItemText(itm,name);
				m_wndFileView.SetItemImage(hitem,img2,simg2);
				m_wndFileView.SetItemImage(itm,img,simg);
				m_wndFileView.SelectItem(hitem);
//				m_wndFileView.SetItemData(hitem,(DWORD_PTR)p);
//				m_wndFileView.SetItemData(itm,(DWORD_PTR)q);
				GetView()->Refresh();
			}

		}
	}
}


void CFileView::OnMovedown()
{
	// TODO: 在此添加命令处理程序代码
	if(thelayers.size()>1)
	{
		HTREEITEM itm=m_wndFileView.GetSelectedItem();
		if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hLayers==m_wndFileView.GetParentItem(itm))
		{
			if(m_wndFileView.GetNextItem(itm,TVGN_NEXT)==NULL)
			{
				MessageBox(L"无法移动，已经处于队列末端",L"不能移动",MB_OK|MB_ICONWARNING);
			}
			else
			{
				HTREEITEM hitem=m_wndFileView.GetNextItem(itm,TVGN_NEXT);
				CString name=m_wndFileView.GetItemText(hitem);
				CString name2=m_wndFileView.GetItemText(itm);
				int img,simg,img2,simg2;
				img=simg=0;
				m_wndFileView.GetItemImage(hitem,img,simg);
				m_wndFileView.GetItemImage(itm,img2,simg2);
				Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
				Layer* q=(Layer*)m_wndFileView.GetItemData(hitem);
				
				thelayers.down(p);

				m_wndFileView.SetItemText(hitem,name2);
				m_wndFileView.SetItemText(itm,name);
				m_wndFileView.SetItemImage(hitem,img2,simg2);
				m_wndFileView.SetItemImage(itm,img,simg);
				m_wndFileView.SelectItem(hitem);
				GetView()->Refresh();
			}
		}
	}
}

void CFileView::OnDellayer()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hLayers==m_wndFileView.GetParentItem(itm))
	{
		Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
		int res=MessageBox(L"确实要删除图层"+p->GetName()+L"吗？",L"询问",MB_ICONQUESTION|MB_YESNO);
		if(res==IDYES)
		{
			thelayers.remove(p);
			m_wndFileView.DeleteItem(itm);
		}

	}
}


void CFileView::OnVisiblelayer()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hLayers==m_wndFileView.GetParentItem(itm))
	{
		Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
		if(!p->IsEditing())
		{p->SetVisible(!p->IsVisible());}
		else
		{
			OnEditlayer();p->SetVisible(!p->IsVisible());
		}
		if(p->IsVisible())
		{m_wndFileView.SetItemImage(itm,p->GetType()+7,p->GetType()+7);}
		else
		{m_wndFileView.SetItemImage(itm,p->GetType()+2,p->GetType()+2);}
		GetView()->Refresh();
	}
}


void CFileView::OnEditlayer()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	int nindex=GetEditingIndex();
	int nsel=GetSelectedIndex();
	if(nindex==-1||nindex==nsel)
	{
		if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hLayers==m_wndFileView.GetParentItem(itm))
		{
			Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
			bool x=p->IsVisible();
			if(x)
			{p->SetEdit(!p->IsEditing());}
			else
			{
				OnVisiblelayer();p->SetEdit(!p->IsEditing());
			}
			if(p->IsEditing())
			{
				m_wndFileView.SetItemImage(itm,12,12);
			}
			else
			{m_wndFileView.SetItemImage(itm,p->GetType()+7,p->GetType()+7);}	
		}
	}
	else
	{
		CString str=thelayers.LayerPtr(nindex)->GetName();
		MessageBox(L"请先结束编辑图层"+str,L"另一图层正在使用",MB_OK|MB_ICONWARNING);
	}
}


void CFileView::OnUpdateVisiblelayer(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hLayers==m_wndFileView.GetParentItem(itm))
	{
		Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
		if(p->IsVisible())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}
	else
	{pCmdUI->SetCheck(FALSE);}
}


void CFileView::OnUpdateEditlayer(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hLayers==m_wndFileView.GetParentItem(itm))
	{
		Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
		if(p->IsEditing())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}
	else
	{pCmdUI->SetCheck(FALSE);}
}


void CFileView::OnAddbmp()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrTitle=_T("添加底图");
	dlg.m_ofn.lpstrFilter=_T("BitMap(*.bmp)\0*.bmp\0JPG(*.jpg)\0*.jpg\0GIF(*.gif)\0*.gif\0PNG(*.png)\0*.png\0\0");
	dlg.m_ofn.lpstrDefExt=_T("bmp");
	if(dlg.DoModal()==IDOK)
	{
		CString str=dlg.GetFileExt();
		CString pathname=dlg.GetPathName();
		CString fname=dlg.GetFileTitle();

		CImage img;
		img.Load(pathname);
		img.GetWidth();
		img.GetHeight();

		CMapCordinDlg g;
		g.pathname=pathname;
		g.DoModal();

		Basepic p;
		p.SetEx(false,pathname,img.GetWidth(),img.GetHeight());
		p.SetCordin(g.m_ltx,g.m_lty,g.m_rbx,g.m_rby);
		thepics.add(p);

		CRect r;
		CIGisMapEditorView* pView=GetView();
		pView->GetClientRect(&r);
		if(GetView()->needCordin)
		{pView->MapZoom.Init(g.m_ltx,g.m_lty,g.m_rbx,g.m_rby,r.Width(),r.Height());}
		
		
		HTREEITEM hitem1=m_wndFileView.InsertItem(fname,6,6,hImages);
		m_wndFileView.SetItemData(hitem1,(DWORD_PTR)thepics.PicPtr(thepics.size()-1));
		{m_wndFileView.SetItemImage(hitem1,6,6);}

		m_wndFileView.Expand(hImages, TVE_EXPAND);
		
	}
	//	HTREEITEM hitem1=m_wndFileView.InsertItem(L"fname",6,6,hImages);
	//	m_wndFileView.Expand(hImages, TVE_EXPAND);
}


void CFileView::OnViewbmp()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	int nindex=GetVisiblePicIndex();
	int selindex=GetSelectedIndex();
	if(nindex==-1||nindex==selindex)
	{
		if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hImages==m_wndFileView.GetParentItem(itm))
		{
			Basepic* p=(Basepic*)m_wndFileView.GetItemData(itm);
			p->SetVisible(!p->IsVisible());
			if(p->IsVisible())
			{m_wndFileView.SetItemImage(itm,11,11);}
			else
			{m_wndFileView.SetItemImage(itm,6,6);}
			GetView()->Refresh();
		}	
	}
	else
	{
		CString str=thepics.PicPtr(nindex)->GetName();
		int nfind=str.ReverseFind(L'\\');
		CString rt=str.Right(str.GetLength()-nfind-1);
		MessageBox(L"请先关闭显示图层"+rt,L"另一图层正在使用",MB_OK|MB_ICONWARNING);
	}
}


void CFileView::OnUpdateViewbmp(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hImages==m_wndFileView.GetParentItem(itm))
	{
		Basepic* p=(Basepic*)m_wndFileView.GetItemData(itm);
		if(p->IsVisible())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}


void CFileView::OnDelbmp()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hImages==m_wndFileView.GetParentItem(itm))
	{
		Basepic* p=(Basepic*)m_wndFileView.GetItemData(itm);
		if(p->IsVisible())
		{
			p->SetVisible(false);
		}
		thepics.remove(p);
		m_wndFileView.DeleteItem(itm);
		m_wndFileView.SelectItem(hRoot);
		GetView()->Refresh();
	}
}


int CFileView::GetSelectedIndex(void)
{
	HTREEITEM hsel=m_wndFileView.GetSelectedItem();
	if(hsel==hRoot||hsel==hImages||hsel==hLayers)
		{return -1;}
	else if(hImages==m_wndFileView.GetParentItem(hsel))
	{
		int n=0;
		HTREEITEM tree=m_wndFileView.GetChildItem(hImages);
		HTREEITEM next=tree;
		while(tree!=hsel)
		{
			next=m_wndFileView.GetNextItem(tree,TVGN_NEXT);
			tree=next;
			n++;
		}
		return n;
	}
	else if(hLayers==m_wndFileView.GetParentItem(hsel))
	{
		int n=0;
		HTREEITEM tree=m_wndFileView.GetChildItem(hLayers);
		HTREEITEM next=tree;
		while(tree!=hsel)
		{
			next=m_wndFileView.GetNextItem(tree,TVGN_NEXT);
			tree=next;
			n++;
		}
		return n;
	}
	else
	{
		return -1;
	}
}


CMainFrame* CFileView::GetMain(void)
{
	CMainFrame* pmain=(CMainFrame*)AfxGetMainWnd();
	return pmain;
}


CIGisMapEditorView* CFileView::GetView(void)
{
	 CIGisMapEditorView* pview=(CIGisMapEditorView*)(this->GetMain()->GetActiveView());
	 return pview;
}


int CFileView::GetEditingIndex(void)
{
	if(FALSE==m_wndFileView.ItemHasChildren(hLayers))
		return -1;
	else
	{
		int n=0;
		HTREEITEM tree=m_wndFileView.GetChildItem(hLayers);
		HTREEITEM next=tree;
		while(n<thelayers.size())
		{
			Layer*p=(Layer*)m_wndFileView.GetItemData(tree);
			if(p->IsEditing()){return n;}
			next=m_wndFileView.GetNextItem(tree,TVGN_NEXT);
			tree=next;
			n++;
		}
		return -1;
	}
}


int CFileView::GetVisiblePicIndex(void)
{
	if(FALSE==m_wndFileView.ItemHasChildren(hImages))
		return -1;
	else
	{
		int n=0;
		HTREEITEM tree=m_wndFileView.GetChildItem(hImages);
		HTREEITEM next=tree;
		while(n<thepics.size())
		{
			Basepic*p=(Basepic*)m_wndFileView.GetItemData(tree);
			if(p->IsVisible()){return n;}
			next=m_wndFileView.GetNextItem(tree,TVGN_NEXT);
			tree=next;
			n++;
		}
		return -1;
	}
}


Layer* CFileView::GetEditingPtr(void)
{
	if(FALSE==m_wndFileView.ItemHasChildren(hLayers))
		return NULL;
	else
	{
		int n=0;
		HTREEITEM tree=m_wndFileView.GetChildItem(hLayers);
		HTREEITEM next=tree;
		while(n<thelayers.size())
		{
			Layer*p=(Layer*)m_wndFileView.GetItemData(tree);
			if(p->IsEditing()){return p;}
			next=m_wndFileView.GetNextItem(tree,TVGN_NEXT);
			tree=next;
			n++;
		}
		return NULL;
	}
}


Basepic* CFileView::GetViewpicPtr(void)
{
	if(FALSE==m_wndFileView.ItemHasChildren(hImages))
		return NULL;
	else
	{
		int n=0;
		HTREEITEM tree=m_wndFileView.GetChildItem(hImages);
		HTREEITEM next=tree;
		while(n<thepics.size())
		{
			Basepic*p=(Basepic*)m_wndFileView.GetItemData(tree);
			if(p->IsVisible()){return p;}
			next=m_wndFileView.GetNextItem(tree,TVGN_NEXT);
			tree=next;
			n++;
		}
		return NULL;
	}
}


void CFileView::RemoveAll(bool call)
{
	if(call)
	{
		if(IDYES==MessageBox(L"移除所有？",L"",MB_YESNO))
		{
			thelayers.clear();
			thepics.clear();
			m_wndFileView.DeleteAllItems();
			hRoot = m_wndFileView.InsertItem(_T("地图工程"), 0, 0);
			m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

			hImages = m_wndFileView.InsertItem(_T("底图"), 1, 1, hRoot);

			hLayers = m_wndFileView.InsertItem(_T("数据图层"), 1, 1, hRoot);
			GetView()->Refresh();
			m_wndFileView.Expand(hRoot, TVE_EXPAND);
			m_wndFileView.Expand(hImages, TVE_EXPAND);
			m_wndFileView.Expand(hLayers, TVE_EXPAND);
		}
		else
		{

		}
	}
	else
	{
		thelayers.clear();
		thepics.clear();
		m_wndFileView.DeleteAllItems();

		hRoot = m_wndFileView.InsertItem(_T("地图工程"), 0, 0);
		m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

		hImages = m_wndFileView.InsertItem(_T("底图"), 1, 1, hRoot);

		hLayers = m_wndFileView.InsertItem(_T("数据图层"), 1, 1, hRoot);
		GetView()->Refresh();
		m_wndFileView.Expand(hRoot, TVE_EXPAND);
		m_wndFileView.Expand(hImages, TVE_EXPAND);
		m_wndFileView.Expand(hLayers, TVE_EXPAND);
	}
}


void CFileView::addBmpEx(CString pathname,double ltx,double lty,double rbx,double rby)
{
	CImage img;
	img.Load(pathname);
	img.GetWidth();
	img.GetHeight();

	int npos=pathname.ReverseFind(L'\\');
	CString fname=pathname.Right(pathname.GetLength()-npos-1);
	fname.TrimRight(L".bmp");

	Basepic p;
	p.SetEx(false,pathname,img.GetWidth(),img.GetHeight());
	p.SetCordin(ltx,lty,rbx,rby);
	thepics.add(p);

	CRect r;
	CIGisMapEditorView* pView=GetView();
	pView->GetClientRect(&r);
	if(GetView()->needCordin)
	{pView->MapZoom.Init(ltx,lty,rbx,rby,r.Width(),r.Height());}
		
		
	HTREEITEM hitem1=m_wndFileView.InsertItem(fname,6,6,hImages);
	m_wndFileView.SetItemData(hitem1,(DWORD_PTR)thepics.PicPtr(thepics.size()-1));
	{m_wndFileView.SetItemImage(hitem1,6,6);}

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hImages, TVE_EXPAND);
	m_wndFileView.Expand(hLayers, TVE_EXPAND);
}


void CFileView::addLayerEx(CString fname,CString server,CString name,CString user,CString pin)
{
	DBConnection db;
	db.Open(server,name,user,pin);

	CString sql;
	CString tmp;
	int ntype;
	sql.Format(L"select gisTable,Type from [CatalogTable] where gisTable='%s'",fname);
			
	CoInitialize(NULL);
	_RecordsetPtr pres(__uuidof(Recordset));
			
	pres=db.ExecuteSQL(sql);
	if(!pres->End)
	{
		BSTR BSTRtemp;
		BSTRtemp=(BSTR)((_bstr_t)pres->GetCollect((_variant_t)L"Type"));
		ntype=strtoi((CString)BSTRtemp);
		ntype%=5;
		ntype--;
		pres.Release();
		CoUninitialize();
		//		
		Layer L;
		L.SetEx(ntype,false,false,fname);
		int nsz=thelayers.size();
		thelayers.add(L);

		HTREEITEM hitem1=m_wndFileView.InsertItem(fname,ntype+2,ntype+2,hLayers);
			
		m_wndFileView.SetItemData(hitem1,(DWORD_PTR)thelayers.LayerPtr(nsz));

	}
	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hImages, TVE_EXPAND);
	m_wndFileView.Expand(hLayers, TVE_EXPAND);
}



void CFileView::OnAttriTable()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM itm=m_wndFileView.GetSelectedItem();
	int nindex=GetEditingIndex();
	int nsel=GetSelectedIndex();
	if(nindex==-1||nindex==nsel)
	{
		if(itm!=NULL&&itm!=hRoot&&itm!=hImages&&itm!=hLayers&&hLayers==m_wndFileView.GetParentItem(itm))
		{
			Layer * layer = (Layer*)m_wndFileView.GetItemData(itm);
			CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	        CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
			if(layer->GetType()==0)
			{
				PointAttri dlg;
				dlg.layer=layer;
				dlg.pZoom=&pView->MapZoom;
				dlg.DoModal();
			}
			else if(layer->GetType()==1)
			{
				LineAttri dlg;
				dlg.layer=layer;
				dlg.pZoom=&pView->MapZoom;
				dlg.DoModal();
			}
			else if(layer->GetType()==2)
			{
				PolyAttri dlg;
				dlg.layer=layer;
				dlg.pZoom=&pView->MapZoom;
				dlg.DoModal();
			}
			else if(layer->GetType()==3)
			{
				MarkerAttri dlg;
				dlg.layer=layer;
				dlg.pZoom=&pView->MapZoom;
				dlg.DoModal();
			}
		}
	}
}


void CFileView::OnShowall()
{
	// TODO: 在此添加命令处理程序代码
	if(m_wndFileView.ItemHasChildren(hLayers)==TRUE)
	{
		HTREEITEM itm=m_wndFileView.GetChildItem(hLayers);
		while(itm!=NULL)
		{
			Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
			if(!p->IsVisible())
			{
				p->SetVisible(true);
				m_wndFileView.SetItemImage(itm,p->GetType()+7,p->GetType()+7);
			}
			itm=m_wndFileView.GetNextSiblingItem(itm);
		}

		//HTREEITEM itm2=m_wndFileView.GetNextSiblingItem(itm);

		//MessageBox(m_wndFileView.GetItemText(itm2));
		GetView()->Refresh();
	}
}


void CFileView::OnHideall()
{
	// TODO: 在此添加命令处理程序代码
	if(m_wndFileView.ItemHasChildren(hLayers)==TRUE)
	{
		HTREEITEM itm=m_wndFileView.GetChildItem(hLayers);
		while(itm!=NULL)
		{
			Layer* p=(Layer*)m_wndFileView.GetItemData(itm);
			if(p->IsVisible())
			{
				if(p->IsEditing())
				{p->SetEdit(false);}
				p->SetVisible(false);
				m_wndFileView.SetItemImage(itm,p->GetType()+2,p->GetType()+2);
			}
			itm=m_wndFileView.GetNextSiblingItem(itm);
		}

		//HTREEITEM itm2=m_wndFileView.GetNextSiblingItem(itm);

		//MessageBox(m_wndFileView.GetItemText(itm2));
		GetView()->Refresh();
	}
}

