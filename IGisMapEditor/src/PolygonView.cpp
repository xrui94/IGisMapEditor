// PolygonView.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "PolygonView.h"
#include "afxdialogex.h"
#include "RegionTypeDlg.h"
#include "MainFrm.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"

// PolygonView 对话框

IMPLEMENT_DYNAMIC(PolygonView, CDialogEx)

PolygonView::PolygonView(CWnd* pParent /*=NULL*/)
	: CDialogEx(PolygonView::IDD, pParent)
{
	selectIndex=-1;
	oldIndex=-1;
	count=0;
	hasInit=false;
}

PolygonView::~PolygonView()
{
}

void PolygonView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PolygonView, PolyView);
}


BEGIN_MESSAGE_MAP(PolygonView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &PolygonView::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &PolygonView::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &PolygonView::OnBnClickedButton2)
END_MESSAGE_MAP()


// PolygonView 消息处理程序


void PolygonView::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	gisPolygon g;
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	if(this->selectIndex!=-1)
	{
		CDC * pDC = pView->GetDC();
		int index=this->getPolygon(g,this->selectID);
		CRegionTypeDlg dlg;
		dlg.m_sideColor=g.BorderColor;
		dlg.m_sideType=g.BorderType;
		dlg.m_fillColor=g.color;
		if(IDOK==dlg.DoModal())
		{
			g.BorderColor=dlg.m_sideColor;
			g.BorderType=dlg.m_sideType;
			g.color=dlg.m_fillColor;
			if(pMainFrame->db.isConnection())
				pMainFrame->db.AlterPolygon(g,table,pDC,pZoom);
			this->gPoly[index]=g;
			this->KillTimer(1);
			pView->Refresh();
			int i=0;
			CString width;
			this->PolyView.DeleteAllItems();
			while(i<count)
			{
				PolyView.InsertItem(i,this->gPoly[i].ID);
				PolyView.SetItemText(i,1,this->getBorderType(this->gPoly[i]));
				PolyView.SetItemText(i,2,table);
				i++;
			}
			this->selectgPoly=g;
			this->oldgPoly=g;
	    }
		else
		{
			this->KillTimer(0);
			if(this->oldgPoly.ID!=L""&&this->hasInit)
				this->endflash(pDC);
		}
		pView->ReleaseDC(pDC);
	}
	else MessageBox(L"你未选择任何面!");

}


void PolygonView::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgPoly.ID!=L"")
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialogEx::OnOK();
}


void PolygonView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==0)
	{
		selectIndex=PolyView.GetNextItem(-1,LVNI_SELECTED);
		if(selectIndex!=-1)
		{
			selectID=PolyView.GetItemText(selectIndex,0);
			this->getPolygon(this->selectgPoly,selectID);
			if(!this->hasInit)
			{
				this->oldIndex=this->selectIndex;
				oldgPoly=selectgPoly;
				this->hasInit=true;
				this->SetTimer(1,1000,NULL);
			}
			if(this->oldIndex!=this->selectIndex)
			{
				CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
				CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
				CDC * pDC =pView->GetDC();
				this->KillTimer(1);
				this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				pView->ReleaseDC(pDC);
				this->oldIndex=this->selectIndex;
			}
		}
		UpdateData(FALSE);
	}
	if(nIDEvent==1)
	{
		if(this->selectIndex!=-1)
		{
			CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
			CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
			CDC * pDC =pView->GetDC();
			if(this->oldgPoly.ID!=this->selectgPoly.ID)
			{
				this->KillTimer(1);
				this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldgPoly=selectgPoly;
			}
			this->flash(pDC);
			pView->ReleaseDC(pDC);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL PolygonView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect r;
	PolyView.GetClientRect(&r);
	PolyView.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	PolyView.InsertColumn(0,_T("ID"));
	PolyView.InsertColumn(1,_T("边界类型"));
	PolyView.InsertColumn(2,L"所属表");

	PolyView.SetColumnWidth(0,r.Width()*0.3);
	PolyView.SetColumnWidth(1,r.Width()*0.3);
	PolyView.SetColumnWidth(2,r.Width()*0.3);
	int i=0;
	while(i<count)
	{
		PolyView.InsertItem(i,this->gPoly[i].ID);
		PolyView.SetItemText(i,1,this->getBorderType(this->gPoly[i]));
		PolyView.SetItemText(i,2,table);
		i++;
	}
	SetTimer(0,10,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void PolygonView::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgPoly.ID!=L"")
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialogEx::OnCancel();
}


void PolygonView::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	if(this->selectIndex==-1)
		MessageBox(L"请选择要删除的区");
	else
	{
		gisPolygon gl;
		int i=0;
		CString width;
		int index=this->getPolygon(gl,selectID);
		pMainFrame->db.DeletePolygon(gl,table);
		count--;
		while(index<count)
		{
			this->gPoly[index]=this->gPoly[index+1];
			index++;
		}
		this->PolyView.DeleteAllItems();
		while(i<count)
		{
			PolyView.InsertItem(i,this->gPoly[i].ID);
			PolyView.SetItemText(i,1,this->getBorderType(this->gPoly[i]));
			PolyView.SetItemText(i,2,table);
			i++;
		}
		this->KillTimer(1);
		this->hasInit=false;
		this->oldgPoly.ID=L"";
		pView->Refresh();
	}

}


CString PolygonView::getBorderType(gisPolygon&gl)
{
	int type=gl.BorderType;
	CString str;
	switch(type)
	{
	case PS_SOLID:str=L"实线";break;
	case PS_DOT:str=L"点线";break;
	case PS_DASHDOTDOT:str=L"双点线";break;
	case PS_DASH:str=L"虚线";break;
	case PS_DASHDOT:str+L"虚点线";break;
	}
	return str;
}

int PolygonView::getPolygon(gisPolygon & gl,CString id)
{
	int i=0;
	while(i<count&&id!=this->gPoly[i].ID)
		i++;
	if(i<count) gl=this->gPoly[i];
	return i;
}

void PolygonView::flash(CDC * pDC)
{
	CPen pen;
	CPen * pOldPen;
	CBrush * pOldBrush;
	CBrush * brush;
	CPoint * p = new CPoint[this->selectgPoly.pCount];
	BYTE r=GetRValue(this->selectgPoly.color);
	BYTE g=GetGValue(this->selectgPoly.color);
	BYTE b=GetBValue(this->selectgPoly.color);
	int color = RGB(255-r,255-g,255-b);
	if(pDC->GetPixel(pZoom->GPtoCP(this->selectgPoly.point[0]))==color)
		color=this->selectgPoly.color;
	brush=new CBrush(color);
	pOldBrush=pDC->SelectObject(brush);//
	pen.CreatePen(this->selectgPoly.BorderType,1,color);
	pOldPen=pDC->SelectObject(&pen);///
	int i=0;
	p[i]=pZoom->GPtoCP(this->selectgPoly.point[i]);
	pDC->MoveTo(pZoom->GPtoCP(this->selectgPoly.point[i++]));///
	while(i<this->selectgPoly.pCount)
	{
		p[i]=pZoom->GPtoCP(this->selectgPoly.point[i]);
		pDC->LineTo(pZoom->GPtoCP(this->selectgPoly.point[i++]));//
	}
	pDC->LineTo(pZoom->GPtoCP(this->selectgPoly.point[0]));///
	pDC->Polygon(p,this->selectgPoly.pCount);//
	pDC->SelectObject(pOldBrush);///
	pDC->SelectObject(pOldPen);//
	pen.DeleteObject();
	delete [] p;
}

void PolygonView::endflash(CDC * pDC)
{
	CPen pen;
	CPen * pOldPen;
	CBrush * brush;
	CBrush * pOldBrush;
	CPoint * p = new CPoint[this->oldgPoly.pCount];
	brush=new CBrush(this->oldgPoly.color);
	pOldBrush=pDC->SelectObject(brush);//
	pen.CreatePen(this->oldgPoly.BorderType,1,this->oldgPoly.BorderColor);
	pOldPen=pDC->SelectObject(&pen);///
	int i=0;
	p[i]=pZoom->GPtoCP(this->oldgPoly.point[i]);
	pDC->MoveTo(pZoom->GPtoCP(this->oldgPoly.point[i++]));///
	while(i<this->oldgPoly.pCount)
	{
		p[i]=pZoom->GPtoCP(this->oldgPoly.point[i]);
		pDC->LineTo(pZoom->GPtoCP(this->oldgPoly.point[i++]));//
	}
	pDC->LineTo(pZoom->GPtoCP(this->oldgPoly.point[0]));///
	pDC->Polygon(p,this->oldgPoly.pCount);//
	pDC->SelectObject(pOldBrush);///
	pDC->SelectObject(pOldPen);//
	pen.DeleteObject();
	delete [] p;
}
