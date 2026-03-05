// PointView.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "PointView.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"
#include "SetPointDlg.h"



// PointView 对话框

IMPLEMENT_DYNAMIC(PointView, CDialog)

PointView::PointView(CWnd* pParent /*=NULL*/)
	: CDialog(PointView::IDD, pParent)
{
	selectIndex=-1;
	oldIndex=-1;
	count=0;
	hasInit=false;
}

PointView::~PointView()
{
}

void PointView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PointSet, m_PointSet);
}


BEGIN_MESSAGE_MAP(PointView, CDialog)
	ON_BN_CLICKED(IDC_EditBnt, &PointView::OnBnClickedEditbnt)
	ON_BN_CLICKED(IDC_DeleteBnt, &PointView::OnBnClickedDeletebnt)
	ON_BN_CLICKED(IDOK, &PointView::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// PointView 消息处理程序


void PointView::OnBnClickedEditbnt()
{
	// TODO: 在此添加控件通知处理程序代
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
				pMainFrame->db.AlterPoint(gpp,table,pDC,pZoom);
			this->gp[index]=gpp;
			this->KillTimer(1);
			pView->Refresh();
			int i=0;
			CString size;
			this->m_PointSet.DeleteAllItems();
			while(i<count)
			{
				m_PointSet.InsertItem(i,this->gp[i].ID);
				m_PointSet.SetItemText(i,1,this->gp[i].pType);
				size.Format(L"%d",this->gp[i].size);
				m_PointSet.SetItemText(i,2,size);
				m_PointSet.SetItemText(i,3,table);
				i++;
			}
			this->oldgp=this->selectgp=gpp;
	    }
		else
		{
			this->KillTimer(0);
			if(this->oldgp.ID!=L""&&this->hasInit)
				this->endflash(pDC);
		}
		pView->ReleaseDC(pDC);
	}
	else MessageBox(L"你未选择任何点!");
}


void PointView::OnBnClickedDeletebnt()
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
		pMainFrame->db.DeletePoint(gp,table);
		while(index<count-1)
		{
			this->gp[index]=this->gp[index+1];
			index++;
		}
		count--;
		this->m_PointSet.DeleteAllItems();
		while(i<count)
		{
			m_PointSet.InsertItem(i,this->gp[i].ID);
			m_PointSet.SetItemText(i,1,this->gp[i].pType);
			size.Format(L"%d",this->gp[i].size);
			m_PointSet.SetItemText(i,2,size);
			m_PointSet.SetItemText(i,3,table);
			i++;
		}
		this->KillTimer(1);
		this->hasInit=false;
		this->oldgp.ID=L"";
		pView->Refresh();
	}
}


void PointView::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnOK();
}


void PointView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==0)
	{
		selectIndex=m_PointSet.GetNextItem(-1,LVNI_SELECTED);
		if(selectIndex!=-1)
		{
			selectID=m_PointSet.GetItemText(selectIndex,0);
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
			if(this->oldgp.ID!=this->selectgp.ID)
			{
				this->KillTimer(1);
				this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldgp=selectgp;
			}
			this->flash(pDC);
			pView->ReleaseDC(pDC);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL PointView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect r;
	m_PointSet.GetClientRect(&r);
	m_PointSet.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_PointSet.InsertColumn(0,_T("ID"));
	m_PointSet.InsertColumn(1,_T("类型"));
	m_PointSet.InsertColumn(2,L"大小");
	m_PointSet.InsertColumn(3,L"所属表");

	m_PointSet.SetColumnWidth(0,r.Width()*0.25);
	m_PointSet.SetColumnWidth(1,r.Width()*0.25);
	m_PointSet.SetColumnWidth(2,r.Width()*0.25);
	m_PointSet.SetColumnWidth(3,r.Width()*0.25);
	int i=0;
	CString size;
	while(i<count)
	{
		m_PointSet.InsertItem(i,gp[i].ID);
		m_PointSet.SetItemText(i,1,gp[i].pType);
		size.Format(L"%d",gp[i].size);
		m_PointSet.SetItemText(i,2,size);
		m_PointSet.SetItemText(i,3,table);
		i++;
	}
	SetTimer(0,10,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int PointView::getPoint(gisPoint & gp,CString id)
{
	int i=0;
	while(i<count&&id!=this->gp[i].ID)
		i++;
	if(i<count) gp=this->gp[i];
	return i;
}
void PointView::flash(CDC * pDC)
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

void PointView::endflash(CDC * pDC)
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

void PointView::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgp.ID!=L"")
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnCancel();
}
