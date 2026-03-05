// PointAttri.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "PointAttri.h"
#include "afxdialogex.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"
#include "SetPointDlg.h"


// PointAttri 对话框

IMPLEMENT_DYNAMIC(PointAttri, CDialog)

PointAttri::PointAttri(CWnd* pParent /*=NULL*/)
	: CDialog(PointAttri::IDD, pParent)
{
	selectIndex=-1;
	oldIndex=-1;
	count=0;
	hasInit=false;
	this->gp=NULL;
}

PointAttri::~PointAttri()
{
	if(this->gp!=NULL)
		delete [] gp;
}

void PointAttri::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PointTable, PointTable);
}


BEGIN_MESSAGE_MAP(PointAttri, CDialog)
	ON_BN_CLICKED(IDC_AlterBnt, &PointAttri::OnBnClickedAlterbnt)
	ON_BN_CLICKED(IDC_DelBnt, &PointAttri::OnBnClickedDelbnt)
	ON_BN_CLICKED(IDOK, &PointAttri::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &PointAttri::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// PointAttri 消息处理程序


void PointAttri::OnBnClickedAlterbnt()
{
	// TODO: 在此添加控件通知处理程序代码
	SetPointDlg dlg;
	gisPoint gpp;
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	if(this->selectIndex!=-1)
	{
		CDC * pDC = pView->GetDC();
		int index=this->getPoint(gpp,this->selectID);
		dlg.m_color=gpp.color;
		dlg.shapeSize=gpp.size;
		dlg.shapeType=gpp.pType;
		if(IDOK==dlg.DoModal())
		{
			gpp.color=dlg.m_color;
			gpp.size=dlg.shapeSize;
			gpp.pType=dlg.shapeType;
			if(pMainFrame->db.isConnection())
				pMainFrame->db.AlterPoint(gpp,layer->GetName(),pDC,pZoom);
			this->gp[index]=gpp;
			this->KillTimer(1);
			pView->Refresh();
			int i=0;
			CString size;
			this->PointTable.DeleteAllItems();
			while(i<count)
			{
				CString color,size,x,y;
				color.Format(L"%d",this->gp[i].color);
				size.Format(L"%d",this->gp[i].size);
				x.Format(L"%f",this->gp[i].point.x);
				y.Format(L"%f",this->gp[i].point.x);
				PointTable.InsertItem(i,this->gp[i].ID);
				PointTable.SetItemText(i,1,x);
				PointTable.SetItemText(i,2,y);
				PointTable.SetItemText(i,3,color);
				PointTable.SetItemText(i,4,size);
				PointTable.SetItemText(i,5,gp[i].pType);
				i++;
			}
			this->oldgp=this->selectgp=gpp;
		}
		else
		{
			this->KillTimer(1);
			if(layer->IsVisible())
				this->endflash(pDC);
		}
		this->selectIndex=-1;
		pView->ReleaseDC(pDC);
	}
	else MessageBox(L"你未选择任何点!");
}


void PointAttri::OnBnClickedDelbnt()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	if(this->selectIndex==-1)
		MessageBox(L"请选择要删除的点");
	else
	{
		gisPoint gp;
		int i=0;
		CString size;
		int index=this->getPoint(gp,selectID);
		pMainFrame->db.DeletePoint(gp,layer->GetName());
		while(index<count-1)
		{
			this->gp[index]=this->gp[index+1];
			index++;
		}
		count--;
		this->PointTable.DeleteAllItems();
		while(i<count)
		{
			CString color,size,x,y;
			color.Format(L"%d",this->gp[i].color);
			size.Format(L"%d",this->gp[i].size);
			x.Format(L"%f",this->gp[i].point.x);
			y.Format(L"%f",this->gp[i].point.x);
			PointTable.InsertItem(i,this->gp[i].ID);
			PointTable.SetItemText(i,1,x);
			PointTable.SetItemText(i,2,y);
			PointTable.SetItemText(i,3,color);
			PointTable.SetItemText(i,4,size);
			PointTable.SetItemText(i,5,this->gp[i].pType);
			i++;
		}
		this->KillTimer(1);
		this->hasInit=false;
		this->oldgp.ID=L"";
		pView->Refresh();
	}
}


void PointAttri::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->layer->IsVisible())
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnOK();
}


void PointAttri::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgp.ID!=L""&&this->layer->IsVisible())
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnCancel();
	CDialog::OnCancel();
}


void PointAttri::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC =pView->GetDC();
	if(nIDEvent==0)
	{
		selectIndex=PointTable.GetNextItem(-1,LVNI_SELECTED);
		if(selectIndex!=-1)
		{
			selectID=PointTable.GetItemText(selectIndex,0);
			this->getPoint(this->selectgp,selectID);
			if(!this->hasInit)
			{
				this->oldIndex=this->selectIndex;
				oldgp=selectgp;
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
			CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
			CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
			CDC * pDC =pView->GetDC();
			if(this->oldgp.ID!=this->selectgp.ID)
			{
				this->KillTimer(1);
				if(layer->IsVisible())
				    this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldgp=selectgp;
			}
			if(layer->IsVisible())
			    this->flash(pDC);
		}
	}
    pView->ReleaseDC(pDC);
	CDialog::OnTimer(nIDEvent);
}


BOOL PointAttri::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect r;
	PointTable.GetClientRect(&r);
	PointTable.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	PointTable.InsertColumn(0,_T("ID"));
	PointTable.InsertColumn(1,_T("X坐标"));
	PointTable.InsertColumn(2,L"y坐标");
	PointTable.InsertColumn(3,L"颜色");
	PointTable.InsertColumn(4,L"大小");
	PointTable.InsertColumn(5,L"类型");

	PointTable.SetColumnWidth(0,r.Width()*0.166);
	PointTable.SetColumnWidth(1,r.Width()*0.166);
	PointTable.SetColumnWidth(2,r.Width()*0.166);
	PointTable.SetColumnWidth(3,r.Width()*0.166);
	PointTable.SetColumnWidth(4,r.Width()*0.166);
	PointTable.SetColumnWidth(5,r.Width()*0.166);


	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	int max=pMainFrame->db.getMaxID(layer->GetName());
	if(this->gp!=NULL) delete [] gp;
	gp = new gisPoint[max];
	pMainFrame->db.GetPointTable(layer->GetName(),this->gp,count);
	int i=0;
	while(i<count)
	{
		CString color,size,x,y;
		color.Format(L"%d",this->gp[i].color);
		size.Format(L"%d",this->gp[i].size);
		x.Format(L"%f",this->gp[i].point.x);
		y.Format(L"%f",this->gp[i].point.x);
		PointTable.InsertItem(i,this->gp[i].ID);
		PointTable.SetItemText(i,1,x);
		PointTable.SetItemText(i,2,y);
		PointTable.SetItemText(i,3,color);
		PointTable.SetItemText(i,4,size);
		PointTable.SetItemText(i,5,gp[i].pType);
		i++;
	}
	this->SetTimer(0,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void PointAttri::flash(CDC * pDC)
{
	BYTE r=GetRValue(this->selectgp.color);
	BYTE g=GetGValue(this->selectgp.color);
	BYTE b=GetBValue(this->selectgp.color);
	int color = RGB(255-r,255-g,255-b);
	if(pDC->GetPixel(pZoom->GPtoCP(this->selectgp.point))==color)
		color=this->selectgp.color;
	int times1=pZoom->ntimes()*(this->selectgp.size+5);
	if(times1==0) times1=1;
	int times2=pZoom->ntimes()*(this->selectgp.size+3);
	if(times2==0) times2=1;
	int times3=pZoom->ntimes()*(this->selectgp.size+1);
	if(times3==0) times3=1;
	CPoint scrpoint=pZoom->GPtoCP(this->selectgp.point);
	CPen * pen = new CPen(PS_SOLID,1,color);
	CPen * pOldPen = pDC->SelectObject(pen);
	CBrush * brush = new CBrush(color);
	CBrush * pOldBrush = pDC->SelectObject(brush);
	if(this->selectgp.pType=="Circle")
			pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
	else if(this->selectgp.pType=="cRect")
		pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times2,scrpoint.x+times1,scrpoint.y+times2);
	else if(this->selectgp.pType=="zRect")
		pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
	else if(this->selectgp.pType=="SJ")
	{
		CPoint _point[3];
		_point[0]=CPoint(scrpoint.x,scrpoint.y-times1);
		_point[1]=CPoint(scrpoint.x-times1,scrpoint.y+times1);
		_point[2]=CPoint(scrpoint.x+times1,scrpoint.y+times1);
		pDC->Polygon(_point,3);
	}
	else if(this->selectgp.pType=="Ellipes")
		pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times3,scrpoint.x+times1,scrpoint.y+times3);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void PointAttri::endflash(CDC * pDC)
{
	int times1=pZoom->ntimes()*(this->oldgp.size+5);
	if(times1==0) times1=1;
	int times2=pZoom->ntimes()*(this->oldgp.size+3);
	if(times2==0) times2=1;
	int times3=pZoom->ntimes()*(this->oldgp.size+1);
	if(times3==0) times3=1;
	CPoint scrpoint=pZoom->GPtoCP(this->oldgp.point);
	CPen * pen = new CPen(PS_SOLID,1,this->oldgp.color);
	CPen * pOldPen = pDC->SelectObject(pen);
	CBrush * brush = new CBrush(this->oldgp.color);
	CBrush * pOldBrush = pDC->SelectObject(brush);
	if(this->oldgp.pType=="Circle")
			pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
	else if(this->oldgp.pType=="cRect")
		pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times2,scrpoint.x+times1,scrpoint.y+times2);
	else if(this->oldgp.pType=="zRect")
		pDC->Rectangle(scrpoint.x-times1,scrpoint.y-times1,scrpoint.x+times1,scrpoint.y+times1);
	else if(this->oldgp.pType=="SJ")
	{
		CPoint _point[3];
		_point[0]=CPoint(scrpoint.x,scrpoint.y-times1);
		_point[1]=CPoint(scrpoint.x-times1,scrpoint.y+times1);
		_point[2]=CPoint(scrpoint.x+times1,scrpoint.y+times1);
		pDC->Polygon(_point,3);
	}
	else if(this->oldgp.pType=="Ellipes")
		pDC->Ellipse(scrpoint.x-times1,scrpoint.y-times3,scrpoint.x+times1,scrpoint.y+times3);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

int PointAttri::getPoint(gisPoint & gp,CString id)
{
	int i=0;
	while(i<count&&id!=this->gp[i].ID)
		i++;
	if(i<count) gp=this->gp[i];
	return i;
}