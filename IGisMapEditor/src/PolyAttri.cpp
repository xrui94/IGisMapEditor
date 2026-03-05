// PolyAttri.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "PolyAttri.h"
#include "afxdialogex.h"
#include "RegionTypeDlg.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"


// PolyAttri 对话框

IMPLEMENT_DYNAMIC(PolyAttri, CDialog)

PolyAttri::PolyAttri(CWnd* pParent /*=NULL*/)
	: CDialog(PolyAttri::IDD, pParent)
{
	selectIndex=-1;
	oldIndex=-1;
	count=0;
	hasInit=false;
	this->gPoly=NULL;
}

PolyAttri::~PolyAttri()
{
	if(this->gPoly!=NULL) delete [] this->gPoly;
}

void PolyAttri::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RgnLC, PolyTable);
}


BEGIN_MESSAGE_MAP(PolyAttri, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &PolyAttri::OnBnClickedAlter)
	ON_BN_CLICKED(IDC_DelBnt, &PolyAttri::OnBnClickedDelbnt)
	ON_BN_CLICKED(IDOK, &PolyAttri::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &PolyAttri::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// PolyAttri 消息处理程序


void PolyAttri::OnBnClickedAlter()
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
				pMainFrame->db.AlterPolygon(g,layer->GetName(),pDC,pZoom);
			this->gPoly[index]=g;
			this->KillTimer(1);
			pView->Refresh();
			int i=0;
			CString width;
			this->PolyTable.DeleteAllItems();
			while(i<count)
			{
				CString color,type,borderColor,xMin,yMin,xMax,yMax,pCount;
				color.Format(L"%d",this->gPoly[i].color);
				type.Format(L"%d",this->gPoly[i].BorderType);
				borderColor.Format(L"%d",this->gPoly[i].BorderColor);
				xMin.Format(L"%f",this->gPoly[i].xMin);
				yMin.Format(L"%f",this->gPoly[i].yMin);
				xMax.Format(L"%f",this->gPoly[i].xMax);
				yMax.Format(L"%f",this->gPoly[i].yMax);
				pCount.Format(L"%d",this->gPoly[i].pCount);
				PolyTable.InsertItem(i,this->gPoly[i].ID);
				PolyTable.SetItemText(i,1,pCount);
				PolyTable.SetItemText(i,2,type);
				PolyTable.SetItemText(i,3,color);
				PolyTable.SetItemText(i,4,L"<二进制数据>");
				PolyTable.SetItemText(i,5,borderColor);
				PolyTable.SetItemText(i,6,xMin);
				PolyTable.SetItemText(i,7,yMin);
				PolyTable.SetItemText(i,8,xMax);
				PolyTable.SetItemText(i,9,yMax);
				i++;
			}
			this->selectgPoly=g;
			this->oldgPoly=g;
	    }
		pView->ReleaseDC(pDC);
	}
	else MessageBox(L"你未选择任何面!");
}


void PolyAttri::OnBnClickedDelbnt()
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
		pMainFrame->db.DeletePolygon(gl,layer->GetName());
		count--;
		while(index<count)
		{
			this->gPoly[index]=this->gPoly[index+1];
			index++;
		}
		this->PolyTable.DeleteAllItems();
		while(i<count)
		{
			CString color,type,borderColor,xMin,yMin,xMax,yMax,pCount;
			color.Format(L"%d",this->gPoly[i].color);
			type.Format(L"%d",this->gPoly[i].BorderType);
			borderColor.Format(L"%d",this->gPoly[i].BorderColor);
			xMin.Format(L"%f",this->gPoly[i].xMin);
			yMin.Format(L"%f",this->gPoly[i].yMin);
			xMax.Format(L"%f",this->gPoly[i].xMax);
			yMax.Format(L"%f",this->gPoly[i].yMax);
			pCount.Format(L"%d",this->gPoly[i].pCount);
			PolyTable.InsertItem(i,this->gPoly[i].ID);
			PolyTable.SetItemText(i,1,pCount);
			PolyTable.SetItemText(i,2,type);
			PolyTable.SetItemText(i,3,color);
			PolyTable.SetItemText(i,4,L"<二进制数据>");
			PolyTable.SetItemText(i,5,borderColor);
			PolyTable.SetItemText(i,6,xMin);
			PolyTable.SetItemText(i,7,yMin);
			PolyTable.SetItemText(i,8,xMax);
			PolyTable.SetItemText(i,9,yMax);
			i++;
		}
		this->KillTimer(1);
		this->hasInit=false;
		this->oldgPoly.ID=L"";
		pView->Refresh();
	}
}


void PolyAttri::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgPoly.ID!=L""&&this->layer->IsVisible())
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnOK();
}


void PolyAttri::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgPoly.ID!=L""&&this->layer->IsVisible())
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnCancel();
}


void PolyAttri::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	if(nIDEvent==0)
	{
		selectIndex=PolyTable.GetNextItem(-1,LVNI_SELECTED);
		if(selectIndex!=-1)
		{
			selectID=PolyTable.GetItemText(selectIndex,0);
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
				this->KillTimer(1);
				if(layer->IsVisible())
					this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldIndex=this->selectIndex;
			}
		}
		UpdateData(FALSE);
	}
	if(nIDEvent==1)
	{
		if(this->selectIndex!=-1)
		{
			if(this->oldgPoly.ID!=this->selectgPoly.ID)
			{
				this->KillTimer(1);
				if(layer->IsVisible())
					this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldgPoly=selectgPoly;
			}
			if(layer->IsVisible())
			   this->flash(pDC);
		}
	}
	pView->ReleaseDC(pDC);
	CDialog::OnTimer(nIDEvent);
}


BOOL PolyAttri::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect r;
	PolyTable.GetClientRect(&r);
	PolyTable.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	PolyTable.InsertColumn(0,_T("ID"));
	PolyTable.InsertColumn(1,L"pCount");
	PolyTable.InsertColumn(2,L"BorderType");
	PolyTable.InsertColumn(3,L"color");
	PolyTable.InsertColumn(4,L"Point");
	PolyTable.InsertColumn(5,L"BorderColor");
	PolyTable.InsertColumn(6,L"xMin");
	PolyTable.InsertColumn(7,L"yMin");
	PolyTable.InsertColumn(8,L"xMax");
	PolyTable.InsertColumn(9,L"yMax");

	PolyTable.SetColumnWidth(0,r.Width()*0.1);
	PolyTable.SetColumnWidth(1,r.Width()*0.1);
	PolyTable.SetColumnWidth(2,r.Width()*0.1);
	PolyTable.SetColumnWidth(3,r.Width()*0.1);
	PolyTable.SetColumnWidth(4,r.Width()*0.1);
	PolyTable.SetColumnWidth(5,r.Width()*0.1);
	PolyTable.SetColumnWidth(6,r.Width()*0.1);
	PolyTable.SetColumnWidth(7,r.Width()*0.1);
	PolyTable.SetColumnWidth(8,r.Width()*0.1);
	PolyTable.SetColumnWidth(9,r.Width()*0.1);

	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	int max=pMainFrame->db.getMaxID(layer->GetName());
	if(this->gPoly!=NULL) delete [] gPoly;
	gPoly = new gisPolygon[max];
	pMainFrame->db.GetPolyTable(layer->GetName(),this->gPoly,count);
	int i=0;
	while(i<count)
	{
		CString color,type,borderColor,xMin,yMin,xMax,yMax,pCount;
		color.Format(L"%d",this->gPoly[i].color);
		type.Format(L"%d",this->gPoly[i].BorderType);
		borderColor.Format(L"%d",this->gPoly[i].BorderColor);
		xMin.Format(L"%f",this->gPoly[i].xMin);
		yMin.Format(L"%f",this->gPoly[i].yMin);
		xMax.Format(L"%f",this->gPoly[i].xMax);
		yMax.Format(L"%f",this->gPoly[i].yMax);
		pCount.Format(L"%d",this->gPoly[i].pCount);
		PolyTable.InsertItem(i,this->gPoly[i].ID);
		PolyTable.SetItemText(i,1,pCount);
		PolyTable.SetItemText(i,2,type);
		PolyTable.SetItemText(i,3,color);
		PolyTable.SetItemText(i,4,L"<二进制数据>");
		PolyTable.SetItemText(i,5,borderColor);
		PolyTable.SetItemText(i,6,xMin);
		PolyTable.SetItemText(i,7,yMin);
		PolyTable.SetItemText(i,8,xMax);
		PolyTable.SetItemText(i,9,yMax);
		i++;
	}
	this->SetTimer(0,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void PolyAttri::flash(CDC * pDC)
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

void PolyAttri::endflash(CDC * pDC)
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


int PolyAttri::getPolygon(gisPolygon & gl,CString id)
{
	int i=0;
	while(i<count&&id!=this->gPoly[i].ID)
		i++;
	if(i<count) gl=this->gPoly[i];
	return i;
}