// LineAttri.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "LineAttri.h"
#include "afxdialogex.h"
#include "LineTypeDlg.h"


// LineAttri 对话框

IMPLEMENT_DYNAMIC(LineAttri, CDialog)

LineAttri::LineAttri(CWnd* pParent /*=NULL*/)
	: CDialog(LineAttri::IDD, pParent)
{
	selectIndex=-1;
	oldIndex=-1;
	count=0;
	hasInit=false;
	this->gl=NULL;
}

LineAttri::~LineAttri()
{
	if(this->gl!=NULL) delete [] gl;
}

void LineAttri::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LineLC, LineTable);
}


BEGIN_MESSAGE_MAP(LineAttri, CDialog)
	ON_BN_CLICKED(IDOK, &LineAttri::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DelBnt, &LineAttri::OnBnClickedDelbnt)
	ON_BN_CLICKED(IDCANCEL, &LineAttri::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_AlterLine, &LineAttri::OnBnClickedAlterline)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// LineAttri 消息处理程序


void LineAttri::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgl.ID!=L""&&layer->IsVisible())
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnOK();
}


void LineAttri::OnBnClickedDelbnt()
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
		pMainFrame->db.DeleteLine(gl,layer->GetName());
		while(index<count-1)
		{
			this->gl[index]=this->gl[index+1];
			index++;
		}
		count--;
		this->LineTable.DeleteAllItems();
		while(i<count)
		{
			CString color,type,width,xMin,yMin,xMax,yMax,pCount;
			color.Format(L"%d",this->gl[i].color);
			type.Format(L"%d",this->gl[i].lType);
			width.Format(L"%d",this->gl[i].width);
			xMin.Format(L"%f",this->gl[i].xMin);
			yMin.Format(L"%f",this->gl[i].yMin);
			xMax.Format(L"%f",this->gl[i].xMax);
			yMax.Format(L"%f",this->gl[i].yMax);
			pCount.Format(L"%d",this->gl[i].pCount);
			LineTable.InsertItem(i,this->gl[i].ID);
			LineTable.SetItemText(i,1,color);
			LineTable.SetItemText(i,2,type);
			LineTable.SetItemText(i,3,width);
			LineTable.SetItemText(i,4,xMin);
			LineTable.SetItemText(i,5,yMin);
			LineTable.SetItemText(i,6,xMax);
			LineTable.SetItemText(i,7,yMax);
			LineTable.SetItemText(i,8,L"<二进制数据>");
			LineTable.SetItemText(i,9,pCount);
			i++;
		}
		this->KillTimer(1);
		this->hasInit=false;
		this->selectgl.ID=L"";;
		this->oldgl.ID=L"";
		pView->Refresh();
	}
	this->selectIndex=-1;
}


void LineAttri::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgl.ID!=L""&&layer->IsVisible())
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	this->selectIndex=-1;
	CDialog::OnCancel();
}


void LineAttri::OnBnClickedAlterline()
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
		dlg.m_width=gll.width;
		dlg.penStyle=gll.lType;
		dlg.PENSTYLE=gll.lType&~PS_GEOMETRIC;
		if(IDOK==dlg.DoModal())
		{
			gll.color=dlg.m_color;
			gll.width=dlg.m_width;
			gll.lType=dlg.penStyle;
			if(pMainFrame->db.isConnection())
				pMainFrame->db.AlterLine(gll,layer->GetName(),pDC,pZoom);
			this->gl[index]=gll;
			this->KillTimer(1);
			pView->Refresh();
			int i=0;
			CString width;
			this->LineTable.DeleteAllItems();
			while(i<count)
			{
				CString color,type,width,xMin,yMin,xMax,yMax,pCount;
				color.Format(L"%d",this->gl[i].color);
				type.Format(L"%d",this->gl[i].lType);
				width.Format(L"%d",this->gl[i].width);
				xMin.Format(L"%f",this->gl[i].xMin);
				yMin.Format(L"%f",this->gl[i].yMin);
				xMax.Format(L"%f",this->gl[i].xMax);
				yMax.Format(L"%f",this->gl[i].yMax);
				pCount.Format(L"%d",this->gl[i].pCount);
				LineTable.InsertItem(i,this->gl[i].ID);
				LineTable.SetItemText(i,1,color);
				LineTable.SetItemText(i,2,type);
				LineTable.SetItemText(i,3,width);
				LineTable.SetItemText(i,4,xMin);
				LineTable.SetItemText(i,5,yMin);
				LineTable.SetItemText(i,6,xMax);
				LineTable.SetItemText(i,7,yMax);
				LineTable.SetItemText(i,8,L"<二进制数据>");
				LineTable.SetItemText(i,9,pCount);
				i++;
			}
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
	else MessageBox(L"你未选择任何线!");
}


void LineAttri::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	if(nIDEvent==0)
	{
		selectIndex=LineTable.GetNextItem(-1,LVNI_SELECTED);
		if(selectIndex!=-1)
		{
			selectID=LineTable.GetItemText(selectIndex,0);
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
			if(this->oldgl.ID!=this->selectgl.ID)
			{
				this->KillTimer(1);
				if(layer->IsVisible())
					this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldgl=selectgl;
			}
			if(layer->IsVisible())
			   this->flash(pDC);
		}
	}
	pView->ReleaseDC(pDC);
	CDialog::OnTimer(nIDEvent);
}


BOOL LineAttri::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect r;
	LineTable.GetClientRect(&r);
	LineTable.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	LineTable.InsertColumn(0,_T("ID"));
	LineTable.InsertColumn(1,_T("LColor"));
	LineTable.InsertColumn(2,L"LType");
	LineTable.InsertColumn(3,L"LWidth");
	LineTable.InsertColumn(4,L"xMin");
	LineTable.InsertColumn(5,L"yMin");
	LineTable.InsertColumn(6,L"xMax");
	LineTable.InsertColumn(7,L"yMax");
	LineTable.InsertColumn(8,L"Point");
	LineTable.InsertColumn(9,L"pCount");

	LineTable.SetColumnWidth(0,r.Width()*0.1);
	LineTable.SetColumnWidth(1,r.Width()*0.1);
	LineTable.SetColumnWidth(2,r.Width()*0.1);
	LineTable.SetColumnWidth(3,r.Width()*0.1);
	LineTable.SetColumnWidth(4,r.Width()*0.1);
	LineTable.SetColumnWidth(5,r.Width()*0.1);
	LineTable.SetColumnWidth(6,r.Width()*0.1);
	LineTable.SetColumnWidth(7,r.Width()*0.1);
	LineTable.SetColumnWidth(8,r.Width()*0.1);
	LineTable.SetColumnWidth(9,r.Width()*0.1);

	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	int max=pMainFrame->db.getMaxID(layer->GetName());
	if(this->gl!=NULL) delete [] gl;
	gl = new gisLine[max];
	pMainFrame->db.GetLineTable(layer->GetName(),this->gl,count);
	int i=0;
	while(i<count)
	{
		CString color,type,width,xMin,yMin,xMax,yMax,pCount;
		color.Format(L"%d",this->gl[i].color);
		type.Format(L"%d",this->gl[i].lType);
		width.Format(L"%d",this->gl[i].width);
		xMin.Format(L"%f",this->gl[i].xMin);
		yMin.Format(L"%f",this->gl[i].yMin);
		xMax.Format(L"%f",this->gl[i].xMax);
		yMax.Format(L"%f",this->gl[i].yMax);
		pCount.Format(L"%d",this->gl[i].pCount);
		LineTable.InsertItem(i,this->gl[i].ID);
		LineTable.SetItemText(i,1,color);
		LineTable.SetItemText(i,2,type);
		LineTable.SetItemText(i,3,width);
		LineTable.SetItemText(i,4,xMin);
		LineTable.SetItemText(i,5,yMin);
		LineTable.SetItemText(i,6,xMax);
		LineTable.SetItemText(i,7,yMax);
		LineTable.SetItemText(i,8,L"<二进制数据>");
		LineTable.SetItemText(i,9,pCount);
		i++;
	}
	this->SetTimer(0,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int LineAttri::getLine(gisLine & gl,CString id)
{
	int i=0;
	while(i<count&&id!=this->gl[i].ID)
		i++;
	if(i<count) gl=this->gl[i];
	return i;
}

void LineAttri::flash(CDC * pDC)
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

void LineAttri::endflash(CDC * pDC)
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


