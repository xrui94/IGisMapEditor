// MarkerAttri.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "MarkerAttri.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"
#include "NoteDlg.h"


// MarkerAttri 对话框

IMPLEMENT_DYNAMIC(MarkerAttri, CDialog)

MarkerAttri::MarkerAttri(CWnd* pParent /*=NULL*/)
	: CDialog(MarkerAttri::IDD, pParent)
{
	selectIndex=-1;
	oldIndex=-1;
	count=0;
	hasInit=false;
	this->CurrentColor=0;
	this->gm=NULL;
}

MarkerAttri::~MarkerAttri()
{
}

void MarkerAttri::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, MarkerTable);
}


BEGIN_MESSAGE_MAP(MarkerAttri, CDialog)
	ON_BN_CLICKED(IDC_AlterBnt, &MarkerAttri::OnBnClickedAlterbnt)
	ON_BN_CLICKED(IDC_DelBnt, &MarkerAttri::OnBnClickedDelbnt)
	ON_BN_CLICKED(IDOK, &MarkerAttri::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MarkerAttri::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// MarkerAttri 消息处理程序


void MarkerAttri::OnBnClickedAlterbnt()
{
	// TODO: 在此添加控件通知处理程序代码
	gisMarker gll;
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	if(this->selectIndex!=-1)
	{
		CDC * pDC = pView->GetDC();
		int index=this->getMarker(gll,this->selectID);
		NoteDlg dlg;
		dlg.m_color=this->selectgm.color;
		dlg.m_font=selectgm.lfont;
		dlg.m_lable=selectgm.text;
		if(IDOK==dlg.DoModal())
		{
			selectgm.color=dlg.m_color;
			selectgm.lfont=dlg.m_font;
			selectgm.text=dlg.m_lable;
			if(pMainFrame->db.isConnection())
				pMainFrame->db.AlterMarker(selectgm,layer->GetName(),pDC,pZoom);
			this->gm[index]=gll;
			this->KillTimer(1);
			pView->Refresh();
			int i=0;
			CString width,font;
			this->MarkerTable.DeleteAllItems();
			while(i<count)
			{
				CString color,x,y;
				color.Format(L"%d",this->gm[i].color);
				x.Format(L"%f",this->gm[i].point.x);
				y.Format(L"%f",this->gm[i].point.x);
				MarkerTable.InsertItem(i,this->gm[i].ID);
				MarkerTable.SetItemText(i,1,x);
				MarkerTable.SetItemText(i,2,y);
				MarkerTable.SetItemText(i,3,gm[i].text);
				MarkerTable.SetItemText(i,4,color);
				MarkerTable.SetItemText(i,5,L"<二进制数据>");
				i++;
			}
			this->selectgm=gll;
			this->oldgm=gll;
	    }
		pView->ReleaseDC(pDC);
	}
	else MessageBox(L"你未选择任注记!");
}


void MarkerAttri::OnBnClickedDelbnt()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	if(this->selectIndex==-1)
		MessageBox(L"请选择要删除的注记");
	else
	{
		gisMarker gl;
		int i=0;
		CString width,font;
		int index=this->getMarker(gl,selectID);
		pMainFrame->db.DeleteMarker(gl,layer->GetName());
		while(index<count-1)
		{
			this->gm[index]=this->gm[index+1];
			index++;
		}
		count--;
		this->MarkerTable.DeleteAllItems();
		while(i<count)
		{
			CString color,x,y;
			color.Format(L"%d",this->gm[i].color);
			x.Format(L"%f",this->gm[i].point.x);
			y.Format(L"%f",this->gm[i].point.x);
			MarkerTable.InsertItem(i,this->gm[i].ID);
			MarkerTable.SetItemText(i,1,x);
			MarkerTable.SetItemText(i,2,y);
			MarkerTable.SetItemText(i,3,gm[i].text);
			MarkerTable.SetItemText(i,4,color);
			MarkerTable.SetItemText(i,5,L"<二进制数据>");
			i++;
		}
		this->KillTimer(1);
		this->hasInit=false;
		this->oldgm.ID=L"";
		pView->Refresh();
	}

}


void MarkerAttri::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgm.ID!=L""&&layer->IsVisible())
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnOK();
	CDialog::OnOK();
}


void MarkerAttri::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgm.ID!=L""&&this->layer->IsVisible())
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnCancel();
}


void MarkerAttri::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==0)
	{
		selectIndex=MarkerTable.GetNextItem(-1,LVNI_SELECTED);
		if(selectIndex!=-1)
		{
			selectID=MarkerTable.GetItemText(selectIndex,0);
			this->getMarker(this->selectgm,selectID);
			if(!this->hasInit)
			{
				this->oldIndex=this->selectIndex;
				oldgm=selectgm;
				this->hasInit=true;
				this->SetTimer(1,1000,NULL);
			}
			if(this->oldIndex!=this->selectIndex)
			{
				CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
				CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
				CDC * pDC =pView->GetDC();
				this->KillTimer(1);
				if(layer->IsVisible())
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
			if(this->oldgm.ID!=this->selectgm.ID)
			{
				this->KillTimer(1);
				if(layer->IsVisible())
				    this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldgm=selectgm;
			}
			if(layer->IsVisible())
			    this->flash(pDC);
			pView->ReleaseDC(pDC);
		}
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL MarkerAttri::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect r;
	MarkerTable.GetClientRect(&r);
	MarkerTable.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	MarkerTable.InsertColumn(0,_T("ID"));
	MarkerTable.InsertColumn(1,L"x坐标");
	MarkerTable.InsertColumn(2,L"y坐标");
	MarkerTable.InsertColumn(3,L"Lable");
	MarkerTable.InsertColumn(4,L"color");
	MarkerTable.InsertColumn(5,L"FontStyle");

	MarkerTable.SetColumnWidth(0,r.Width()*0.166);
	MarkerTable.SetColumnWidth(1,r.Width()*0.166);
	MarkerTable.SetColumnWidth(2,r.Width()*0.166);
	MarkerTable.SetColumnWidth(3,r.Width()*0.166);
	MarkerTable.SetColumnWidth(4,r.Width()*0.166);
	MarkerTable.SetColumnWidth(5,r.Width()*0.166);
	
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	int max=pMainFrame->db.getMaxID(layer->GetName());
	if(this->gm!=NULL) delete [] gm;
	gm = new gisMarker[max];
	pMainFrame->db.GetMarkerTable(layer->GetName(),this->gm,count);
	int i=0;
	while(i<count)
	{
		CString color,x,y;
		color.Format(L"%d",this->gm[i].color);
		x.Format(L"%f",this->gm[i].point.x);
		y.Format(L"%f",this->gm[i].point.x);
		MarkerTable.InsertItem(i,this->gm[i].ID);
		MarkerTable.SetItemText(i,1,x);
		MarkerTable.SetItemText(i,2,y);
		MarkerTable.SetItemText(i,3,gm[i].text);
		MarkerTable.SetItemText(i,4,color);
		MarkerTable.SetItemText(i,5,L"<二进制数据>");
		i++;
	}
	this->SetTimer(0,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int MarkerAttri::getMarker(gisMarker & gl,CString id)
{
	int i=0;
	while(i<count&&id!=this->gm[i].ID)
		i++;
	if(i<count) gl=this->gm[i];
	return i;
}

void MarkerAttri::flash(CDC * pDC)
{
	BYTE r=GetRValue(this->selectgm.color);
	BYTE g=GetGValue(this->selectgm.color);
	BYTE b=GetBValue(this->selectgm.color);
	if(this->CurrentColor!=this->selectgm.color)
		this->CurrentColor=this->selectgm.color;
	else this->CurrentColor=RGB(255-r,255-g,255-b);
	LOGFONT lfont=this->selectgm.lfont;
	int nwheight=int(this->selectgm.lfont.lfHeight*pZoom->ntimes());
	if(abs(nwheight)<=5)
	{
		int a=nwheight/abs(nwheight);
		nwheight=a*5;
	}
	lfont.lfHeight=nwheight;
	HFONT hfont = ::CreateFontIndirect(&lfont);
	CFont* pfont = CFont::FromHandle(hfont);
	CFont *oldfont=pDC->SelectObject(pfont);
	COLORREF oldcolor=pDC->SetTextColor(this->CurrentColor);///
	int nbk=pDC->SetBkMode(TRANSPARENT);///
	pDC->TextOutW(pZoom->GPtoCP(this->selectgm.point).x,pZoom->GPtoCP(this->selectgm.point).y,this->selectgm.text);///
	pDC->SetTextColor(oldcolor);//
	pDC->SelectObject(oldfont);//
	pDC->SetBkMode(nbk);//
	::DeleteObject(hfont);
}

void MarkerAttri::endflash(CDC * pDC)
{
	int nwheight=int(this->oldgm.lfont.lfHeight*pZoom->ntimes());
	LOGFONT lfont=this->oldgm.lfont;
	if(abs(nwheight)<=5)
	{
		int a=nwheight/abs(nwheight);
		nwheight=a*5;
	}
	lfont.lfHeight=nwheight;
	HFONT hfont = ::CreateFontIndirect(&lfont);
	CFont* pfont = CFont::FromHandle(hfont);
	CFont *oldfont=pDC->SelectObject(pfont);
	COLORREF oldcolor=pDC->SetTextColor(this->oldgm.color);///
	int nbk=pDC->SetBkMode(TRANSPARENT);///
	pDC->TextOutW(pZoom->GPtoCP(this->oldgm.point).x,pZoom->GPtoCP(this->oldgm.point).y,this->oldgm.text);///
	pDC->SetTextColor(oldcolor);//
	pDC->SelectObject(oldfont);//
	pDC->SetBkMode(nbk);//
	::DeleteObject(hfont);
}
