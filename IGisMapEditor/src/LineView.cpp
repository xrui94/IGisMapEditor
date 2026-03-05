// LineView.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "LineView.h"
#include "afxdialogex.h"
#include "LineTypeDlg.h"
#include "MainFrm.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"


// LineView 对话框

IMPLEMENT_DYNAMIC(LineView, CDialog)

LineView::LineView(CWnd* pParent /*=NULL*/)
	: CDialog(LineView::IDD, pParent)
{
	selectIndex=-1;
	oldIndex=-1;
	count=0;
	hasInit=false;
}

LineView::~LineView()
{
}

void LineView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINEVIEW, LineSet);
}


BEGIN_MESSAGE_MAP(LineView, CDialog)
	ON_BN_CLICKED(IDC_AlterBnt, &LineView::OnBnClickedAlterbnt)
	ON_BN_CLICKED(IDC_DeleteBnt, &LineView::OnBnClickedDeletebnt)
	ON_BN_CLICKED(IDOK, &LineView::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// LineView 消息处理程序


void LineView::OnBnClickedAlterbnt()
{
	// TODO: 在此添加控件通知处理程序代码
	gisLine gll;
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	if(this->selectIndex!=-1)
	{
		CDC * pDC = pView->GetDC();
		int index=this->getLine(gll,this->selectID);
		CLineTypeDlg dlg;
		dlg.m_color=gll.color;
		dlg.m_width=abs(int(gll.width));
		if(gll.width<0)
			dlg.m_curve=TRUE;
		else
			dlg.m_curve=FALSE;
		//dlg.penStyle=gll.lType;
		dlg.PENSTYLE=gll.lType;//&~PS_GEOMETRIC;
		if(IDOK==dlg.DoModal())
		{
			gll.color=dlg.m_color;
			int n=1;
			if(gll.width<0)
				n=-1;
			gll.width=n*dlg.m_width;
			gll.lType=dlg.penStyle;
			if(pMainFrame->db.isConnection())
				pMainFrame->db.AlterLine(gll,table,pDC,pZoom);
			this->gl[index]=gll;
			this->KillTimer(1);
			pView->Refresh();
			int i=0;
			CString width;
			this->LineSet.DeleteAllItems();
			while(i<count)
			{
				LineSet.InsertItem(i,gl[i].ID);
				LineSet.SetItemText(i,1,this->getLineType(gl[i]));
				width.Format(L"%d",gl[i].width);
				LineSet.SetItemText(i,2,width);
				LineSet.SetItemText(i,3,table);
				i++;
			}
			this->selectgl=gll;
			this->oldgl=gll;
	    }
		else
		{
			this->KillTimer(0);
			if(this->oldgl.ID!=L""&&this->hasInit)
				this->endflash(pDC);
		}
		pView->ReleaseDC(pDC);
	}
	else MessageBox(L"你未选择任何线!");
}


void LineView::OnBnClickedDeletebnt()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	if(this->selectIndex==-1)
		MessageBox(L"请选择要删除的线");
	else
	{
		gisLine gl;
		int i=0;
		CString width;
		int index=this->getLine(gl,selectID);
		pMainFrame->db.DeleteLine(gl,table);
		while(index<count-1)
		{
			this->gl[index]=this->gl[index+1];
			index++;
		}
		count--;
		this->LineSet.DeleteAllItems();
		while(i<count)
		{
			LineSet.InsertItem(i,this->gl[i].ID);
			LineSet.SetItemText(i,1,this->getLineType(this->gl[i]));
			width.Format(L"%d",this->gl[i].width);
			LineSet.SetItemText(i,2,width);
			LineSet.SetItemText(i,3,table);
			i++;
		}
		this->KillTimer(1);
		this->hasInit=false;
		this->oldgl.ID=L"";
		pView->Refresh();
	}
}


void LineView::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgl.ID!=L"")
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnOK();
}


void LineView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==0)
	{
		selectIndex=LineSet.GetNextItem(-1,LVNI_SELECTED);
		if(selectIndex!=-1)
		{
			selectID=LineSet.GetItemText(selectIndex,0);
			this->getLine(this->selectgl,selectID);
			if(!this->hasInit)
			{
				this->oldIndex=this->selectIndex;
				oldgl=selectgl;
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
			if(this->oldgl.ID!=this->selectgl.ID)
			{
				this->KillTimer(1);
				this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldgl=selectgl;
			}
			this->flash(pDC);
			pView->ReleaseDC(pDC);
		}
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL LineView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect r;
	LineSet.GetClientRect(&r);
	LineSet.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	LineSet.InsertColumn(0,_T("ID"));
	LineSet.InsertColumn(1,_T("类型"));
	LineSet.InsertColumn(2,L"宽度");
	LineSet.InsertColumn(3,L"所属表");

	LineSet.SetColumnWidth(0,r.Width()*0.25);
	LineSet.SetColumnWidth(1,r.Width()*0.25);
	LineSet.SetColumnWidth(2,r.Width()*0.25);
	LineSet.SetColumnWidth(3,r.Width()*0.25);
	int i=0;
	CString width;
	while(i<count)
	{
		LineSet.InsertItem(i,this->gl[i].ID);
		LineSet.SetItemText(i,1,this->getLineType(this->gl[i]));
		width.Format(L"%d",abs(int(this->gl[i].width)));
		LineSet.SetItemText(i,2,width);
		LineSet.SetItemText(i,3,table);
		i++;
	}
	SetTimer(0,10,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


CString LineView::getLineType(gisLine&gl)
{
	int type=gl.lType;//&~PS_GEOMETRIC;
	CString str;
	CString str2;
	if(gl.width<0)
		str2=L"(曲线)";
	else
		str2=L"(折线)";

	switch(type)
	{
	case PS_SOLID:str=L"实线";break;
	case PS_DOT:str=L"点线";break;
	case PS_DASHDOTDOT:str=L"双点线";break;
	case PS_DASH:str=L"虚线";break;
	case PS_DASHDOT:str+L"虚点线";break;
	}
	return str+str2;
}

int LineView::getLine(gisLine & gl,CString id)
{
	int i=0;
	while(i<count&&id!=this->gl[i].ID)
		i++;
	if(i<count) gl=this->gl[i];
	return i;
}

void LineView::flash(CDC * pDC)
{
	CPen pen;
	CPen * pOldPen;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	BYTE r=GetRValue(this->selectgl.color);
	BYTE g=GetGValue(this->selectgl.color);
	BYTE b=GetBValue(this->selectgl.color);
	int color = RGB(255-r,255-g,255-b);
	if(pDC->GetPixel(pZoom->GPtoCP(this->selectgl.LinePoint[0]))==color)
		color=this->selectgl.color;
	logBrush.lbColor = color;
	pen.CreatePen(this->selectgl.lType|PS_GEOMETRIC,abs(int(this->selectgl.width))*pZoom->ntimes(),&logBrush);
	pOldPen=pDC->SelectObject(&pen);
	if(selectgl.width>=0)
	{
		int j=0;
		pDC->MoveTo(pZoom->GPtoCP(selectgl.LinePoint[j++]));
		while(j<selectgl.pCount)
			pDC->LineTo(pZoom->GPtoCP(selectgl.LinePoint[j++]));
	}
	else
	{
		int nt=0;
		CPoint *pts=new CPoint[selectgl.pCount];
		while(nt<selectgl.pCount)
		{
			pts[nt]=pZoom->GPtoCP(selectgl.LinePoint[nt]);
			nt++;
		}
		pDC->PolyBezier(pts,selectgl.pCount);
		delete []pts;
	}
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void LineView::endflash(CDC * pDC)
{
	CPen pen;
	CPen * pOldPen;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = this->oldgl.color;
	pen.CreatePen(this->oldgl.lType|PS_GEOMETRIC,abs(int(this->oldgl.width))*pZoom->ntimes(),&logBrush);
	pOldPen=pDC->SelectObject(&pen);
	if(oldgl.width>=0)
	{
		int j=0;
		pDC->MoveTo(pZoom->GPtoCP(oldgl.LinePoint[j++]));
		while(j<oldgl.pCount)
			pDC->LineTo(pZoom->GPtoCP(oldgl.LinePoint[j++]));
	}
	else
	{
		int nt=0;
		CPoint *pts=new CPoint[oldgl.pCount];
		while(nt<oldgl.pCount)
		{
			pts[nt]=pZoom->GPtoCP(oldgl.LinePoint[nt]);
			nt++;
		}
		pDC->PolyBezier(pts,oldgl.pCount);
		delete []pts;
	}
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void LineView::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgl.ID!=L"")
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnCancel();
}
