// MarkerView.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "MarkerView.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "IGisMapEditorDoc.h"
#include "IGisMapEditorView.h"
#include "NoteDlg.h"


// MarkerView 对话框

IMPLEMENT_DYNAMIC(MarkerView, CDialog)

MarkerView::MarkerView(CWnd* pParent /*=NULL*/)
	: CDialog(MarkerView::IDD, pParent)
{
	selectIndex=-1;
	oldIndex=-1;
	count=0;
	hasInit=false;
	this->CurrentColor=0;
}

MarkerView::~MarkerView()
{
}

void MarkerView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MarkerView, MarkerSet);
}


BEGIN_MESSAGE_MAP(MarkerView, CDialog)
	ON_BN_CLICKED(IDC_EditBnt, &MarkerView::OnBnClickedEditbnt)
	ON_BN_CLICKED(IDOK, &MarkerView::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DeleteBnt, &MarkerView::OnBnClickedDeletebnt)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// MarkerView 消息处理程序


void MarkerView::OnBnClickedEditbnt()
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
				pMainFrame->db.AlterMarker(selectgm,table,pDC,pZoom);
			this->gm[index]=gll;
			this->KillTimer(1);
			pView->Refresh();
			int i=0;
			CString width,font;
			this->MarkerSet.DeleteAllItems();
			while(i<count)
			{
				MarkerSet.InsertItem(i,this->gm[i].ID);
				//this->getFontStyle(this->gm[i],width,font);
				width.Format(L"%d",abs(gm[i].lfont.lfHeight));
				MarkerSet.SetItemText(i,1,width);
				MarkerSet.SetItemText(i,2,this->gm[i].text);
				MarkerSet.SetItemText(i,3,gm[i].lfont.lfFaceName);
				MarkerSet.SetItemText(i,4,table);
				i++;
			}
			this->selectgm=gll;
			this->oldgm=gll;
	    }
		else
		{
			this->KillTimer(0);
			if(this->oldgm.ID!=L""&&this->hasInit)
				this->endflash(pDC);
		}
		pView->ReleaseDC(pDC);
	}
	else MessageBox(L"你未选择任注记!");
}


void MarkerView::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgm.ID!=L"")
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnOK();
}


void MarkerView::OnBnClickedDeletebnt()
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
		pMainFrame->db.DeleteMarker(gl,table);
		while(index<count-1)
		{
			this->gm[index]=this->gm[index+1];
			index++;
		}
		count--;
		this->MarkerSet.DeleteAllItems();
		while(i<count)
		{
			MarkerSet.InsertItem(i,this->gm[i].ID);
			//this->getFontStyle(this->gm[i],width,font);
			width.Format(L"%d",abs(gm[i].lfont.lfHeight));
			MarkerSet.SetItemText(i,1,width);
			MarkerSet.SetItemText(i,2,this->gm[i].text);
			MarkerSet.SetItemText(i,3,gm[i].lfont.lfFaceName);
			MarkerSet.SetItemText(i,4,table);
			i++;
		}
		this->KillTimer(1);
		this->hasInit=false;
		this->oldgm.ID=L"";
		pView->Refresh();
	}
}


void MarkerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==0)
	{
		selectIndex=MarkerSet.GetNextItem(-1,LVNI_SELECTED);
		if(selectIndex!=-1)
		{
			selectID=MarkerSet.GetItemText(selectIndex,0);
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
				this->endflash(pDC);
				this->SetTimer(1,1000,NULL);
				this->oldgm=selectgm;
			}
			this->flash(pDC);
			pView->ReleaseDC(pDC);
		}
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL MarkerView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect r;
	MarkerSet.GetClientRect(&r);
	MarkerSet.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	MarkerSet.InsertColumn(0,_T("ID"));
	MarkerSet.InsertColumn(1,L"宽度");
	MarkerSet.InsertColumn(2,L"文本");
	MarkerSet.InsertColumn(3,L"字体");
	MarkerSet.InsertColumn(4,L"所属表");

	MarkerSet.SetColumnWidth(0,r.Width()*0.2);
	MarkerSet.SetColumnWidth(1,r.Width()*0.2);
	MarkerSet.SetColumnWidth(2,r.Width()*0.2);
	MarkerSet.SetColumnWidth(3,r.Width()*0.2);
	MarkerSet.SetColumnWidth(4,r.Width()*0.2);
	int i=0;
	CString width,font;
	while(i<count)
	{
		MarkerSet.InsertItem(i,this->gm[i].ID);
		//this->getFontStyle(this->gm[i],width,font);
		width.Format(L"%d",abs(gm[i].lfont.lfHeight));
		MarkerSet.SetItemText(i,1,width);
		MarkerSet.SetItemText(i,2,this->gm[i].text);
		MarkerSet.SetItemText(i,3,gm[i].lfont.lfFaceName);
		MarkerSet.SetItemText(i,4,table);
		i++;
	}
	SetTimer(0,10,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int MarkerView::getMarker(gisMarker & gl,CString id)
{
	int i=0;
	while(i<count&&id!=this->gm[i].ID)
		i++;
	if(i<count) gl=this->gm[i];
	return i;
}

void MarkerView::flash(CDC * pDC)
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

void MarkerView::endflash(CDC * pDC)
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

void getFontStyle(gisMarker &gl,CString width,CString fn)
{
	width.Format(L"%d",abs(gl.lfont.lfHeight));
	fn=gl.lfont.lfFaceName;
}

void MarkerView::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CMainFrame * pMainFrame = (CMainFrame * )AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView * )pMainFrame->GetActiveView();
	CDC * pDC = pView->GetDC();
	this->KillTimer(0);
	this->KillTimer(1);
	if(this->oldgm.ID!=L"")
	    this->endflash(pDC);
	pView->ReleaseDC(pDC);
	CDialog::OnCancel();
}
