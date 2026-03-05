
// IGisMapEditorView.cpp : CIGisMapEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "IGisMapEditor.h"
#endif

#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"

#include "DataBase.h"
#include "MainFrm.h"
#include "SetPointDlg.h"
#include "LineTypeDlg.h"
#include "RegionTypeDlg.h"
#include "NoteDlg.h"
#include "PointView.h"
#include "LineView.h"
#include "PolygonView.h"
#include "MarkerView.h"
#include "DjAnalyse.h"
#include "ImageryDlg.h"
//
#include <atlimage.h>

//

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIGisMapEditorView

IMPLEMENT_DYNCREATE(CIGisMapEditorView, CView)

BEGIN_MESSAGE_MAP(CIGisMapEditorView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIGisMapEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_ZOOMIN, &CIGisMapEditorView::OnZoomin)
	ON_COMMAND(ID_ZOOMOUT, &CIGisMapEditorView::OnZoomout)
	ON_COMMAND(ID_ZOOMMOVE, &CIGisMapEditorView::OnZoommove)
	ON_COMMAND(ID_REDRAW, &CIGisMapEditorView::OnRedraw)
	ON_COMMAND(ID_REFRESHVIEW, &CIGisMapEditorView::OnRefreshview)
	ON_COMMAND(ID_CREATEPOINT, &CIGisMapEditorView::OnCreatepoint)
	ON_COMMAND(ID_CREATENOTE, &CIGisMapEditorView::OnCreatenote)
	ON_COMMAND(ID_CREATELINE, &CIGisMapEditorView::OnCreateline)
	ON_COMMAND(ID_CREATEREGION, &CIGisMapEditorView::OnCreateregion)
	ON_COMMAND(ID_AlterPoint, &CIGisMapEditorView::OnAlterPoint)
	ON_COMMAND(ID_MovePoint, &CIGisMapEditorView::OnMovePoint)
	ON_COMMAND(ID_DeletePoint, &CIGisMapEditorView::OnDeletePoint)
	ON_COMMAND(ID_AlterMarker, &CIGisMapEditorView::OnAlterMarker)
	ON_COMMAND(ID_MoveMarker, &CIGisMapEditorView::OnMoveMarker)
	ON_COMMAND(ID_DeleteMarker, &CIGisMapEditorView::OnDeleteMarker)
	ON_COMMAND(ID_AlterLine, &CIGisMapEditorView::OnAlterLine)
	ON_COMMAND(ID_MoveLine, &CIGisMapEditorView::OnMoveLine)
	ON_COMMAND(ID_DeleteLine, &CIGisMapEditorView::OnDeleteLine)
	ON_COMMAND(ID_CutLine, &CIGisMapEditorView::OnCutLine)
	ON_COMMAND(ID_AddPointToLine, &CIGisMapEditorView::OnAddPointToLine)
	ON_COMMAND(ID_DelLinePt, &CIGisMapEditorView::OnDelLinePt)
	ON_COMMAND(ID_AlterRegion, &CIGisMapEditorView::OnAlterRegion)
	ON_COMMAND(ID_MoveRegion, &CIGisMapEditorView::OnMoveRegion)
	ON_COMMAND(ID_DeleteRegion, &CIGisMapEditorView::OnDeleteRegion)
	ON_COMMAND(ID_DelRgnPt, &CIGisMapEditorView::OnDelRgnPt)
	ON_COMMAND(ID_AddRgnPt, &CIGisMapEditorView::OnAddRgnPt)
	ON_COMMAND(ID_SearchByRect, &CIGisMapEditorView::OnSearchByRect)
	ON_COMMAND(ID_SearchByCircle, &CIGisMapEditorView::OnSearchByCircle)
	ON_COMMAND(ID_SearchByPolygon, &CIGisMapEditorView::OnSearchByPolygon)
	ON_COMMAND(ID_ALTERMENU_Attr, &CIGisMapEditorView::OnAlterMenuAttr)
	ON_COMMAND(ID_ALTERMENU_Move, &CIGisMapEditorView::OnAlterMenuMove)
	ON_COMMAND(ID_ALTERMENU_Delete, &CIGisMapEditorView::OnAlterMenuDelete)
	ON_WM_TIMER()
	ON_COMMAND(ID_MoveLinePt, &CIGisMapEditorView::OnMoveLinePt)
	ON_COMMAND(ID_MovePolygonPt, &CIGisMapEditorView::OnMovePolygonPt)
	ON_COMMAND(ID_LineDjPolygon, &CIGisMapEditorView::OnLineDjPolygon)
	ON_COMMAND(ID_PolyDjPoly, &CIGisMapEditorView::OnPolyDjPoly)
	ON_COMMAND(ID_HcAnalyse, &CIGisMapEditorView::OnHcAnalyse)
	ON_COMMAND(ID_SavePicture, &CIGisMapEditorView::OnSavepicture)
END_MESSAGE_MAP()

// CIGisMapEditorView 构造/析构

CIGisMapEditorView::CIGisMapEditorView()
	: AlterType(_T(""))
{
	// TODO: 在此处添加构造代码
	pDraw=NULL;
	pData=NULL;
	pZoom=NULL;
	pAlterTool=NULL;
	pSearchTool=NULL;
	needCordin=true;
	this->SearchType=L"";
}

CIGisMapEditorView::~CIGisMapEditorView()
{
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	status=-1;
}

BOOL CIGisMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CIGisMapEditorView 绘制

void CIGisMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CIGisMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	Refresh();
}

void CIGisMapEditorView::Refresh()
{
	CVisibleManager* pVmanage=GetVisibleManager();
	CMainFrame* pMain=GetMain();
	CRect r;
	GetClientRect(&r);
	CDC* pDC=GetDC();
	Basepic* pMap=pVmanage->GetMap();
	Layer* pLayer=pVmanage->first();
	CDC memDC;
	CBitmap memBitmap;
	CBitmap* poldBit;
	CImage img;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,r.Width(),r.Height());
	poldBit=memDC.SelectObject(&memBitmap);
	memDC.FillSolidRect(0,0,r.Width(),r.Height(),RGB(255,255,255));
	if(!MapZoom.Initalized())
	{
		if(pMap!=NULL)
		{
			MapZoom.Init(pMap->ltx,pMap->lty,pMap->rbx,pMap->rby,r.Width(),r.Height());
			needCordin=false;
		}
		else if(pLayer!=NULL)
		{
			double maxx,maxy,minx,miny;
			maxx=maxy=minx=miny=0.0;
			if(pMain->db.isConnection())
			{
				if(pMain->db.getTableInfo(minx,miny,maxx,maxy))
				{
					MapZoom.Init(minx,maxy,maxx,miny,r.Width(),r.Height());
					needCordin=false;
				}
				else
				{
					MapZoom.Init(0,r.Height(),r.Width(),0,r.Width(),r.Height());
					needCordin=true;
				}			
			}
		}
		else
		{
		    MapZoom.Init(0,r.Height(),r.Width(),0,r.Width(),r.Height());
			needCordin=true;
		}
	}
	MapZoom.recalc(r.Width(),r.Height());
	if(pMap!=NULL)
	{
		img.Load(pMap->GetName());
		CPoint lt=pMap->GetCP(MapZoom.lt.x,MapZoom.lt.y);
		CPoint rb=pMap->GetCP(MapZoom.rb.x,MapZoom.rb.y);
		::StretchBlt(memDC.m_hDC,0,0,r.Width(),r.Height(),img.GetDC(),lt.x,lt.y,(rb.x-lt.x),(rb.y-lt.y),SRCCOPY);
		img.ReleaseDC();
	}
	while(pLayer!=NULL)
	{
		if(pMain->db.isConnection())
		   pMain->db.DisplaySet(&memDC,&MapZoom,pLayer->GetName());
		pLayer=pVmanage->next(pLayer);
	}
	pDC->BitBlt(0,0,r.Width(),r.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(poldBit);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
	ReleaseDC(pDC);
	pVmanage->Release();
}

// CIGisMapEditorView 打印


void CIGisMapEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIGisMapEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CIGisMapEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CIGisMapEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CIGisMapEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIGisMapEditorView 诊断

#ifdef _DEBUG
void CIGisMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CIGisMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIGisMapEditorDoc* CIGisMapEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIGisMapEditorDoc)));
	return (CIGisMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CIGisMapEditorView 消息处理程序


CVisibleManager* CIGisMapEditorView::GetVisibleManager(void)
{
	CMainFrame* pmain=(CMainFrame*)AfxGetMainWnd();
	return pmain->GetVisibles();
}


CMainFrame* CIGisMapEditorView::GetMain(void)
{
	CMainFrame* pmain=(CMainFrame*)AfxGetMainWnd();
	return pmain;
}

Layer* CIGisMapEditorView::GetEditingLayerPtr(void)
{
	return GetMain()->GetEditingLayerPtr();
}


Basepic* CIGisMapEditorView::GetVisibleMapPtr(void)
{
	return GetMain()->GetVisibleMapPtr();
}


int CIGisMapEditorView::CheckEditing(void)
{
	//if(status>-1)
	//{
		Layer *p=GetEditingLayerPtr();
		CString str;
		switch(status)
		{
		case 0:str=L"点";break;
		case 1:str=L"线";break;
		case 2:str=L"区";break;
		case 3:str=L"注记";break;
		default:str=L"";break;
		}
		if(p!=NULL&&p->GetType()!=status)
		{
			AfxMessageBox(L"没有可编辑的"+str+"图层！");
			return -2;
		}
		else if(p==NULL)
		{
			AfxMessageBox(L"没有可编辑的"+str+"图层！");
			return -1;
		}
		else
		{
			int n=p->GetType();
			return n;
		}
	//}
	//else return -3;
}

void CIGisMapEditorView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnRButtonDblClk(nFlags, point);
}
//===================================================================================================================

void CIGisMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame * pMainFrame =GetMain();
	Layer *p=pMainFrame->GetEditingLayerPtr();
	if(pDraw!=NULL&&p!=NULL&&p->GetType()==status)
	{
		pDraw->LButtonDown(nFlags,point);
		pData->LButtonDown(point,&MapZoom);
	}
	else if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
		status=-1;
		delete pData;
		pData=NULL;
	}
	else if(pZoom!=NULL)
		pZoom->LButtonDown(point,&MapZoom);
	else if(pSearchTool!=NULL)
		pSearchTool->LButtonDown(nFlags,point);
	else if(p!=NULL&&p->GetType()==status&&pAlterTool!=NULL)
	{
		pAlterTool->LButtonDown(nFlags,point);
		CDC * pDC =GetDC();
		if(pAlterTool->hasFound())
		{
			if(pAlterTool->getAlterType()=="AlterAttri"&&this->AlterType=="point")
			{
				gisPoint  gp =pAlterTool->getPoint();
				SetPointDlg dlg;
				dlg.p=point;
				dlg.m_color=gp.color;
				dlg.shapeSize=gp.size;
				dlg.shapeType=gp.pType;
				if(IDOK==dlg.DoModal())
				{
					gp.color=dlg.m_color;
					gp.size=dlg.shapeSize;
				    gp.pType=dlg.shapeType;
					if(pMainFrame->db.isConnection())
					   pMainFrame->db.AlterPoint(gp,p->GetName(),pDC,&MapZoom);
					pAlterTool->setInit(false);
					this->KillTimer(0);
					this->Refresh();
				}
				else
				{
					this->KillTimer(0);
					pAlterTool->endflash(pDC);
				}
			}
			else if(pAlterTool->getAlterType()=="AlterAttri"&&this->AlterType=="line")
			{
				gisLine gl;
				pAlterTool->getLine(gl);
				CLineTypeDlg dlg;
				dlg.p=point;
				dlg.m_color=gl.color;
				dlg.m_width=abs(int(gl.width));
				if(gl.width<0)
					dlg.m_curve=TRUE;
				//dlg.penStyle=gl.lType;
				dlg.PENSTYLE=gl.lType;//&~PS_GEOMETRIC;
				if(IDOK==dlg.DoModal())
				{
					gl.color=dlg.m_color;
					int n=1;
					if(gl.width<0)
						n=-1;
					gl.width=n*dlg.m_width;
				    gl.lType=dlg.PENSTYLE;
					if(pMainFrame->db.isConnection())
					   pMainFrame->db.AlterLine(gl,p->GetName(),pDC,&MapZoom);
					pAlterTool->setInit(false);
					this->KillTimer(0);
					this->Refresh();
				}
				else
				{
					this->KillTimer(0);
					pAlterTool->endflash(pDC);////
				}
			}
			else if(pAlterTool->getAlterType()=="AlterAttri"&&this->AlterType=="Polygon")
			{
				gisPolygon gPoly;
				pAlterTool->getPoly(gPoly);
				CRegionTypeDlg dlg;
				dlg.p=point;
				dlg.m_sideColor=gPoly.BorderColor;
				dlg.m_sideType=gPoly.BorderType;
				dlg.m_fillColor=gPoly.color;
				if(IDOK==dlg.DoModal())
				{
					gPoly.BorderColor=dlg.m_sideColor;
				    gPoly.BorderType=dlg.m_sideType;
				    gPoly.color=dlg.m_fillColor;
					if(pMainFrame->db.isConnection())
					   pMainFrame->db.AlterPolygon(gPoly,p->GetName(),pDC,&MapZoom);
					pAlterTool->setInit(false);
					this->KillTimer(0);
					this->Refresh();
				}
				else
				{
					this->KillTimer(0);
					pAlterTool->endflash(pDC);
				}
			}
			else if(pAlterTool->getAlterType()=="AlterAttri"&&this->AlterType=="Marker")
			{
				gisMarker gm;
				pAlterTool->getMarker(gm);
				NoteDlg dlg;
				dlg.pos=point;
				dlg.m_color=gm.color;
				dlg.m_font=gm.lfont;
				dlg.m_lable=gm.text;
				if(IDOK==dlg.DoModal())
				{
					gm.color=dlg.m_color;
					gm.lfont=dlg.m_font;
					gm.text=dlg.m_lable;
					if(pMainFrame->db.isConnection())
					   pMainFrame->db.AlterMarker(gm,p->GetName(),pDC,&MapZoom);
					pAlterTool->setInit(false);
					this->KillTimer(0);
					this->Refresh();
				}
				else
				{
					this->KillTimer(0);
					pAlterTool->endflash(pDC);
				}
			}
			ReleaseDC(pDC);
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


void CIGisMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Layer *p=GetMain()->GetEditingLayerPtr();

	if(pDraw!=NULL&&p!=NULL&&p->GetType()==status)
	{
		pDraw->LButtonUp(nFlags,point);
		pData->LButtonUp(point,&MapZoom);
	}
	else if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
		status=-1;
		delete pData;
		pData=NULL;
	}
	else if(p!=NULL&&p->GetType()==status&&pAlterTool!=NULL)
		pAlterTool->LButtonUp(nFlags,point);
	else if(pSearchTool!=NULL)
	{
		pSearchTool->SetLayer(p->GetName());
		pSearchTool->SetType(p->GetType());
		pSearchTool->LButtonUp(nFlags,point);
		if(pSearchTool->getCount()&&this->SearchType!=L"Polygon")
		{
			switch(p->GetType()+1)
			{
				case 1:
				{
					PointView dlg;
					dlg.pZoom=&this->MapZoom;
					dlg.table=p->GetName();
					dlg.count=pSearchTool->getCount();
					dlg.gp=new gisPoint[dlg.count];
					pSearchTool->getPointSet(dlg.gp);
					dlg.DoModal();
					break;
				}
				case 2:
				{
					LineView dlg;
					dlg.pZoom=&this->MapZoom;
					dlg.table=p->GetName();
					dlg.count=pSearchTool->getCount();
					dlg.gl=new gisLine[dlg.count];
					pSearchTool->getLineSet(dlg.gl);
					dlg.DoModal();
					break;
				}
				case 3:
				{
					PolygonView dlg;
					dlg.pZoom=&this->MapZoom;
					dlg.table=p->GetName();
					dlg.count=pSearchTool->getCount();
					dlg.gPoly=new gisPolygon[dlg.count];
					pSearchTool->getPolySet(dlg.gPoly);
					dlg.DoModal();
					break;
				}
				case 4:
				{
					MarkerView dlg;
					dlg.pZoom=&this->MapZoom;
					dlg.table=p->GetName();
					dlg.count=pSearchTool->getCount();
					dlg.gm=new gisMarker[dlg.count];
					pSearchTool->getMarkerSet(dlg.gm);
					dlg.DoModal();
					break;
				}
			}
			pSearchTool->setCount(0);
		}
	}
	else
	{
		status=-1;
		if(pZoom!=NULL)
		{
			pZoom->LButtonUp(point,&MapZoom);
			Refresh();
		}	
	}
	CView::OnLButtonUp(nFlags, point);
}

void CIGisMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DPoint pt=MapZoom.CPtoGP(point);
	GetMain()->showXy(pt.x,pt.y);
	GetMain()->showStatus(status);
	Layer *p=GetMain()->GetEditingLayerPtr();
	if(pDraw!=NULL&&p!=NULL&&p->GetType()==status)
	{
		pDraw->MouseMove(nFlags,point);
		pData->MouseMove(point,&MapZoom);
	}
	else if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
		status=-1;
		delete pData;
		pData=NULL;
	}	
	else if(p!=NULL&&p->GetType()==status&&pAlterTool!=NULL)
		pAlterTool->MouseMove(nFlags,point);
	else if(pSearchTool!=NULL)
		pSearchTool->MouseMove(nFlags,point);
	else 
	{
		status=-1;
		if(pZoom!=NULL)
			pZoom->MouseMove(point,&MapZoom);
	}

	CView::OnMouseMove(nFlags, point);
}

void CIGisMapEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Layer *p=GetMain()->GetEditingLayerPtr();

	if(pDraw!=NULL&&p!=NULL&&p->GetType()==status)
	{
		pDraw->RButtonDown(nFlags,point);
		pData->RButtonDown(point,&MapZoom);
	}
	else if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
		status=-1;
		delete pData;
		pData=NULL;
	}
	else if(pSearchTool!=NULL)
	{
		pSearchTool->SetLayer(p->GetName());
		pSearchTool->SetType(p->GetType());
		pSearchTool->RButtonDown(nFlags,point);
		if(pSearchTool->getCount()&&this->SearchType==L"Polygon")
		{
			switch(p->GetType()+1)
			{
				case 1:
				{
					PointView dlg;
					dlg.pZoom=&this->MapZoom;
					dlg.table=p->GetName();
					dlg.count=pSearchTool->getCount();
					dlg.gp=new gisPoint[dlg.count];
					pSearchTool->getPointSet(dlg.gp);
					dlg.DoModal();
					break;
				}
				case 2:
				{
					LineView dlg;
					dlg.pZoom=&this->MapZoom;
					dlg.table=p->GetName();
					dlg.count=pSearchTool->getCount();
					dlg.gl=new gisLine[dlg.count];
					pSearchTool->getLineSet(dlg.gl);
					dlg.DoModal();
					break;
				}
				case 3:
				{
					PolygonView dlg;
					dlg.pZoom=&this->MapZoom;
					dlg.table=p->GetName();
					dlg.count=pSearchTool->getCount();
					dlg.gPoly=new gisPolygon[dlg.count];
					pSearchTool->getPolySet(dlg.gPoly);
					dlg.DoModal();
					break;
				}
				case 4:
				{
					MarkerView dlg;
					dlg.pZoom=&this->MapZoom;
					dlg.table=p->GetName();
					dlg.count=pSearchTool->getCount();
					dlg.gm=new gisMarker[dlg.count];
					pSearchTool->getMarkerSet(dlg.gm);
					dlg.DoModal();
					break;
				}
			}
			pSearchTool->setCount(0);
		}
	}
	else
	{
		status=-1;
		if(pZoom!=NULL)
		{
			delete pZoom;
			pZoom=NULL;
		}
	}	
	CView::OnRButtonDown(nFlags, point);
}

void CIGisMapEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	if(pDraw==NULL&&pSearchTool==NULL)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}
	else
	{
		CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
		Layer *p=pFrame->GetEditingLayerPtr();

		UINT nFlags=0;
		if(p!=NULL&&p->GetType()==status)
		{
			pDraw->RButtonUp(nFlags,point);
			pData->RButtonUp(point,&MapZoom);
		}
		else if(pSearchTool!=NULL)
			pSearchTool->RButtonDown(nFlags,point);
		else if(pDraw!=NULL)
		{
			delete pDraw;
			pDraw=NULL;
			status=-1;
			delete pData;
			pData=NULL;
		}
		else {status=-1;}
	}
}


void CIGisMapEditorView::OnZoomin()
{
	// TODO: 在此添加命令处理程序代码
	status=-1;
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(!GetVisibleManager()->None())
	{pZoom=new CZoominTool(this);}
}


void CIGisMapEditorView::OnZoomout()
{
	// TODO: 在此添加命令处理程序代码
	status=-1;
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(!GetVisibleManager()->None())
		pZoom=new CZoomoutTool(this);
}


void CIGisMapEditorView::OnZoommove()
{
	// TODO: 在此添加命令处理程序代码
	status=-1;
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(!GetVisibleManager()->None()) pZoom=new CMoveTool(this);
}


void CIGisMapEditorView::OnRedraw()
{
	// TODO: 在此添加命令处理程序代码
	if(pDraw!=NULL)
	{
		delete pDraw;
		delete pData;
		pDraw=NULL;
		pData=NULL;
	}
	if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	CRect r;
	GetClientRect(&r);
	CMainFrame* pMain=GetMain();
	Basepic*p=GetVisibleManager()->GetMap();
	if(p!=NULL)
	{
		MapZoom.Init(p->ltx,p->lty,p->rbx,p->rby,r.Width(),r.Height());
		needCordin=false;
	}
	else
	{
		Layer* l=GetVisibleManager()->first();
		if(l==NULL)
		{
			MapZoom.Init(0,r.Height(),r.Width(),0,r.Width(),r.Height());
			needCordin=true;
		}
		else
		{
			double maxx,maxy,minx,miny;
			maxx=maxy=minx=miny=0.0;
			if(pMain->db.isConnection())
			{
				if(pMain->db.getTableInfo(minx,miny,maxx,maxy))
				{
					MapZoom.Init(minx,maxy,maxx,miny,r.Width(),r.Height());
					needCordin=false;
				}
				else
				{
					MapZoom.Init(0,r.Height(),r.Width(),0,r.Width(),r.Height());
					needCordin=true;
				}			
			}
		}
	}
	Refresh();
}


void CIGisMapEditorView::OnRefreshview()
{
	// TODO: 在此添加命令处理程序代码
	if(pDraw!=NULL)
	{
		delete pDraw;
		delete pData;
		pDraw=NULL;
		pData=NULL;
	}
	if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	Refresh();
}


//----------------------------------------------------------------------------------------------------------------

void CIGisMapEditorView::OnCreatepoint()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	status=0;
	int oldStatus=CheckEditing();
	if(oldStatus>=0&&oldStatus==status)
	{
		SetPointDlg g;
		g.m_color=Styles.ps.color;
		g.shapeSize=Styles.ps.size;
		g.shapeType=Styles.ps.type;
		if(IDOK==g.DoModal())
		{
			Styles.ps.color=g.m_color;
			Styles.ps.size=g.shapeSize;
			Styles.ps.type=g.shapeType;
		}
		CString lname=GetEditingLayerPtr()->GetName();
		pDraw=new CPointTool(this,&Styles,MapZoom.ntimes());
		pData=new CPointDataTool(&Styles,lname);
		pData->SetConn(&pMainFrame->db);
	}
	else status=-1;
}


void CIGisMapEditorView::OnCreatenote()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame * pMain = (CMainFrame *)::AfxGetMainWnd();

	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	status=3;
	int oldStatus=CheckEditing();
	if(oldStatus>=0)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pDraw=new CNoteTool(this,&Styles,MapZoom.ntimes());
		pData=new CNoteDataTool(&Styles,lname);
		pData->SetConn(&pMain->db);
		NoteDlg g;
		if(IDOK==g.DoModal())
		{
			Styles.ns.color=g.m_color;
			Styles.ns.lfont=g.m_font;
			Styles.ns.text=g.m_lable;
		}
	}
	else status=-1;
}


void CIGisMapEditorView::OnCreateline()
{
	// TODO: 在此添加命令处理程序代码
    CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	status=1;
	int oldStatus=CheckEditing();
	if(oldStatus>=0&&oldStatus==status)
	{
		CLineTypeDlg g;
		g.m_color=Styles.ls.color;
		g.PENSTYLE=Styles.ls.PENTYPE;
		g.m_width=Styles.ls.width;
		g.m_curve=Styles.ls.curve;
		if(IDOK==g.DoModal())
		{
			Styles.ls.color=g.m_color;
			Styles.ls.PENTYPE=g.PENSTYLE;
			Styles.ls.width=g.m_width;
			Styles.ls.curve=g.m_curve;
		}
		CString lname=GetEditingLayerPtr()->GetName();
		pDraw=new CLineTool(this,&Styles,MapZoom.ntimes());
		pData=new CLineDataTool(&Styles,lname);
		CMainFrame* pMain=GetMain();
		pData->SetConn(&pMain->db);
	}
	else status=-1;
}

void CIGisMapEditorView::OnCreateregion()
{
	// TODO: 在此添加命令处理程序代码
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	status=2;
	int oldStatus=CheckEditing();
	if(oldStatus>=0&&oldStatus==status)
	{
		CRegionTypeDlg g;
		g.m_sideType=Styles.rs.sideType;
		g.m_sideColor=Styles.rs.sidecolor;
		g.m_fillColor=Styles.rs.fillcolor;
		if(IDOK==g.DoModal())
		{
			Styles.rs.sideType=g.m_sideType;
			Styles.rs.sidecolor=g.m_sideColor;
			Styles.rs.fillcolor=g.m_fillColor;
		}
		CString lname=GetEditingLayerPtr()->GetName();
		pDraw=new CPolyTool(this,&Styles,MapZoom.ntimes());
		pData=new CPolyDataTool(&Styles,lname);
		CMainFrame* pMain=GetMain();
		pData->SetConn(&pMain->db);
	}
	else status=-1;
}


void CIGisMapEditorView::OnAlterPoint()
{
	// TODO: 在此添加命令处理程序代码
	this->status=0;
	this->AlterType="point";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPointTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"AlterAttri");
	}
}


void CIGisMapEditorView::OnMovePoint()
{
	// TODO: 在此添加命令处理程序代码
	this->status=0;
	this->AlterType="point";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPointTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"Move");
	}
}


void CIGisMapEditorView::OnDeletePoint()
{
	// TODO: 在此添加命令处理程序代码
	this->status=0;
	this->AlterType="point";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPointTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"Delete");
	}
}

void CIGisMapEditorView::OnAlterMarker()
{
	// TODO: 在此添加命令处理程序代码
	this->status=3;
	this->AlterType="Marker";
	int oldStatus=CheckEditing();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterMarkerTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"AlterAttri");
	}
}


void CIGisMapEditorView::OnMoveMarker()
{
	// TODO: 在此添加命令处理程序代码
	this->status=3;
	this->AlterType="Marker";
	int oldStatus=CheckEditing();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterMarkerTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"Move");
	}
}


void CIGisMapEditorView::OnDeleteMarker()
{
	// TODO: 在此添加命令处理程序代码
	this->status=3;
	this->AlterType="Marker";
	int oldStatus=CheckEditing();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterMarkerTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"Delete");
	}
}


void CIGisMapEditorView::OnAlterLine()
{
	// TODO: 在此添加命令处理程序代码
	this->status=1;
	this->AlterType="line";
	int oldStatus=CheckEditing();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterLineTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"AlterAttri");
	}
}


void CIGisMapEditorView::OnMoveLine()
{
	// TODO: 在此添加命令处理程序代码
	this->status=1;
	this->AlterType="line";
	int oldStatus=CheckEditing();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CDC * pDC =GetDC();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterLineTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"Move");
	}

}


void CIGisMapEditorView::OnDeleteLine()
{
	// TODO: 在此添加命令处理程序代码
	this->status=1;
	this->AlterType="line";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterLineTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"Delete");
	}
}


void CIGisMapEditorView::OnCutLine()
{
	// TODO: 在此添加命令处理程序代码
}


void CIGisMapEditorView::OnAddPointToLine()
{
	// TODO: 在此添加命令处理程序代码
	this->status=1;
	this->AlterType="line";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterLineTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"addPtToLine");
	}
}


void CIGisMapEditorView::OnDelLinePt()
{
	// TODO: 在此添加命令处理程序代码
	this->status=1;
	this->AlterType="line";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterLineTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"delPtFromLine");
	}
}

void CIGisMapEditorView::OnMoveLinePt()
{
	// TODO: 在此添加命令处理程序代码
	this->status=1;
	this->AlterType="line";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterLineTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"MoveLinePt");
	}
}

void CIGisMapEditorView::OnAlterRegion()
{
	// TODO: 在此添加命令处理程序代码
	this->status=2;
	this->AlterType="Polygon";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPolygonTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"AlterAttri");
	}
}


void CIGisMapEditorView::OnMoveRegion()
{
	// TODO: 在此添加命令处理程序代码
	this->status=2;
	this->AlterType="Polygon";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPolygonTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"Move");
	}
}


void CIGisMapEditorView::OnDeleteRegion()
{
	// TODO: 在此添加命令处理程序代码
	this->status=2;
	this->AlterType="Polygon";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPolygonTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"Delete");
	}
}


void CIGisMapEditorView::OnDelRgnPt()
{
	// TODO: 在此添加命令处理程序代码
	this->status=2;
	this->AlterType="Polygon";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPolygonTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"delPtFromPolygon");
	}
}


void CIGisMapEditorView::OnAddRgnPt()
{
	// TODO: 在此添加命令处理程序代码
	this->status=2;
	this->AlterType="Polygon";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPolygonTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"addPtToPolygon");
	}
}

void CIGisMapEditorView::OnMovePolygonPt()
{
	// TODO: 在此添加命令处理程序代码
	this->status=2;
	this->AlterType="Polygon";
	int oldStatus=CheckEditing();
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
    if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	if(oldStatus>=0&&oldStatus==status)
	{
		CString lname=GetEditingLayerPtr()->GetName();
		pAlterTool = new AlterPolygonTool(this,&MapZoom,&pMainFrame->db,lname);
		pAlterTool->setAlterType(L"MovePolygonPt");
	}
}


void CIGisMapEditorView::OnSearchByRect()
{
	// TODO: 在此添加命令处理程序代码
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	Layer * layer = GetEditingLayerPtr();
	if(layer!=NULL)
	{
		this->SearchType=L"";
		CString lname=GetEditingLayerPtr()->GetName();
		pSearchTool = new SearchByRect(this,&MapZoom,&pMainFrame->db,lname,layer->GetType());
	}
	else MessageBox(L"没有可编辑的图层!");
}


void CIGisMapEditorView::OnSearchByCircle()
{
	// TODO: 在此添加命令处理程序代码
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	Layer * layer = GetEditingLayerPtr();
	if(layer!=NULL)
	{
		this->SearchType=L"";
		CString lname=GetEditingLayerPtr()->GetName();
		pSearchTool = new SearchByCircle(this,&MapZoom,&pMainFrame->db,lname,layer->GetType());
	}
	else MessageBox(L"没有可编辑的图层!");
}


void CIGisMapEditorView::OnSearchByPolygon()
{
	// TODO: 在此添加命令处理程序代码
	CDC * pDC =GetDC();
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	if(pZoom!=NULL)
	{
		delete pZoom;
		pZoom=NULL;
	}
	if(pDraw!=NULL)
	{
		delete pDraw;
		pDraw=NULL;
	}
	if(pData!=NULL)
	{
		delete pData;
		pData=NULL;
	}
	if(pAlterTool!=NULL)
	{
		this->KillTimer(0);
		if(pAlterTool->getHasInit())
		   pAlterTool->endflash(pDC);
		delete pAlterTool;
		pAlterTool=NULL;
	}
	if(pSearchTool!=NULL)
	{
		delete pSearchTool;
		pSearchTool=NULL;
	}
	ReleaseDC(pDC);
	Layer * layer = GetEditingLayerPtr();
	if(layer!=NULL)
	{
		this->SearchType=L"Polygon";
		CString lname=GetEditingLayerPtr()->GetName();
		pSearchTool = new SearchByPolygon(this,&MapZoom,&pMainFrame->db,lname,layer->GetType());
	}
	else MessageBox(L"没有可编辑的图层!");
}


void CIGisMapEditorView::OnAlterMenuAttr()
{
	// TODO: 在此添加命令处理程序代码
}


void CIGisMapEditorView::OnAlterMenuMove()
{
	// TODO: 在此添加命令处理程序代码
}


void CIGisMapEditorView::OnAlterMenuDelete()
{
	// TODO: 在此添加命令处理程序代码
}


void CIGisMapEditorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if((this->AlterType=="point"||this->AlterType=="line"||this->AlterType=="Polygon"||this->AlterType=="Marker")&&pAlterTool!=NULL)
	{
		CDC * pDC =GetDC();
		pAlterTool->flash(pDC);
		ReleaseDC(pDC);
	}
	CView::OnTimer(nIDEvent);
}


void CIGisMapEditorView::OnLineDjPolygon()
{
	// TODO: 在此添加命令处理程序代码
	DjAnalyse dlg;
	dlg.DoModal();
}


void CIGisMapEditorView::OnPolyDjPoly()
{
	// TODO: 在此添加命令处理程序代码
	DjAnalyse dlg;
	dlg.DoModal();
}


void CIGisMapEditorView::OnHcAnalyse()
{
	// TODO: 在此添加命令处理程序代码
}


void CIGisMapEditorView::OnSavepicture()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame* pfrm=GetMain();
	CVisibleManager *pvm=GetVisibleManager();
	Layer *p=pvm->first();
	if(!pfrm->db.isConnection())
	{MessageBox(_T("数据库未连接"));pvm->Release();return;}
	else if(p==NULL&&pvm->GetMap()==NULL)
	{
		MessageBox(_T("没有可用图层，请把要输出的图层全部设为可见"));pvm->Release();return;
	}
	else
	{
		CImageryDlg dlg;
		dlg.pZoom=&MapZoom;
		dlg.db=&(pfrm->db);
		CRect r;
		GetClientRect(&r);
		dlg.m_width=r.Width();
		dlg.m_height=r.Height();
		if(IDOK==dlg.DoModal())
		{
			CDC idc;
			idc.CreateCompatibleDC(this->GetDC());			
			int nwidth=dlg.m_width;
			int nheight=dlg.m_height;
			int bpp=24;
			if(!dlg.mappix)
			{
				CSize sz(nwidth*100,nheight*100);
				idc.HIMETRICtoDP(&sz);
				nwidth=sz.cx;
				nheight=sz.cy;
			}
			if(!dlg.bpp24)
			{bpp=32;}
			CFileDialog g(FALSE);
			g.m_ofn.lpstrTitle=L"存为图像";
			g.m_ofn.lpstrDefExt=L"JPG";
			g.m_ofn.lpstrFilter=L"JPG(*.jpg)\0*.jpg\0BMP(*.bmp)\0*.bmp\0GIF(*.gif)\0*.gif\0PNG(*.png)\0*.png\0\0";
			if(IDOK==g.DoModal())
			{
				CImage img;
				img.Create(nwidth,nheight,bpp);
				CBitmap bmp;
				bmp.CreateCompatibleBitmap(this->GetDC(),nwidth,nheight);
				CBitmap *pold=idc.SelectObject(&bmp);
				idc.FillSolidRect(0,0,nwidth,nheight,RGB(255,255,255));
				CZoomManager zoom(MapZoom);
				zoom.Init(dlg.m_xmin,dlg.m_ymax,dlg.m_xmax,dlg.m_ymin,nwidth,nheight);
				Basepic* pMap=pvm->GetMap();
				if(pMap!=NULL)
				{
					int n=MessageBox(L"是否连同底图一起生成图像？",L"提示",MB_YESNO|MB_ICONQUESTION);
					if(n==IDYES)
					{
						CImage map;
						map.Load(pMap->GetName());
						CPoint lt=pMap->GetCP(zoom.lt.x,zoom.lt.y);
						CPoint rb=pMap->GetCP(zoom.rb.x,zoom.rb.y);
						::StretchBlt(idc.m_hDC,0,0,r.Width(),r.Height(),map.GetDC(),lt.x,lt.y,(rb.x-lt.x),(rb.y-lt.y),SRCCOPY);
						map.ReleaseDC();
					}
				}
				while(p!=NULL)
				{
					GetMain()->db.OutputImagery(&idc,p->GetName(),&zoom,dlg.itempix);
					Layer *temp=pvm->next(p);
					p=temp;
				}
				::BitBlt(img.GetDC(),0,0,nwidth,nheight,idc.m_hDC,0,0,SRCCOPY);
				img.Save(g.GetPathName());
				img.ReleaseDC();
				idc.SelectObject(pold);
				bmp.DeleteObject();
				idc.DeleteDC();
			}
			else
			{idc.DeleteDC();pvm->Release();return;}
		}
		else
		{pvm->Release();return;}
	}
}
