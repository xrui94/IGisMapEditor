// NoteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "NoteDlg.h"
#include "afxdialogex.h"


// NoteDlg 对话框

IMPLEMENT_DYNAMIC(NoteDlg, CDialog)

NoteDlg::NoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NoteDlg::IDD, pParent)
	, m_lable(_T(""))
{
	m_color=RGB(0,0,0);
		m_font.lfCharSet=134;
		m_font.lfClipPrecision=2;
		m_font.lfEscapement=0;
		m_font.lfHeight=16;
		m_font.lfItalic=FALSE;
		m_font.lfOrientation=0;
		m_font.lfOutPrecision=1;
		m_font.lfPitchAndFamily=34;
		m_font.lfQuality=2;
		m_font.lfStrikeOut=FALSE;
		m_font.lfUnderline=FALSE;
		m_font.lfWeight=700;
		m_font.lfWidth=7;
		m_font.lfFaceName[0]=83;m_font.lfFaceName[1]=121;
		m_font.lfFaceName[2]=115;m_font.lfFaceName[3]=116;
		m_font.lfFaceName[4]=101;m_font.lfFaceName[5]=109;
		m_font.lfFaceName[6]=0;m_font.lfFaceName[7]=845;
		m_font.lfFaceName[8]=63648;m_font.lfFaceName[9]=65535;
		m_font.lfFaceName[10]=498;m_font.lfFaceName[11]=0;
		m_font.lfFaceName[12]=0;m_font.lfFaceName[13]=0;
		m_font.lfFaceName[14]=52540;m_font.lfFaceName[15]=633;
		m_font.lfFaceName[16]=63616;m_font.lfFaceName[17]=65535;
		m_font.lfFaceName[18]=8;m_font.lfFaceName[19]=32344;
		m_font.lfFaceName[20]=63227;m_font.lfFaceName[21]=65535;
		m_font.lfFaceName[22]=0;m_font.lfFaceName[23]=0;
		m_font.lfFaceName[24]=0;m_font.lfFaceName[25]=0;
		m_font.lfFaceName[26]=52512;m_font.lfFaceName[27]=633;
		m_font.lfFaceName[28]=63616;m_font.lfFaceName[29]=65535;
		m_font.lfFaceName[30]=65535;m_font.lfFaceName[31]=0;
		pos=CPoint(500,250);
}

NoteDlg::~NoteDlg()
{
}

void NoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LABLE, m_lable);
	DDX_Control(pDX, IDC_ViewControl, m_show);
}


BEGIN_MESSAGE_MAP(NoteDlg, CDialog)
	ON_BN_CLICKED(IDC_FontStyle, &NoteDlg::OnBnClickedFontstyle)
	ON_BN_CLICKED(IDOK, &NoteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &NoteDlg::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// NoteDlg 消息处理程序


void NoteDlg::OnBnClickedFontstyle()
{
	// TODO: 在此添加控件通知处理程序代码
	CFontDialog dlg;
	if(IDOK==dlg.DoModal())
	{
		dlg.GetCurrentFont(&m_font);
		m_color=dlg.GetColor();
	}
	ViewFont();
}


void NoteDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_lable!=L"")
	    CDialog::OnOK();
	else AfxMessageBox(L"请填写注记文本");
	CDialog::OnOK();
}


void NoteDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


BOOL NoteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetWindowPos(&wndTopMost,pos.x,pos.y,100,100,SWP_NOSIZE);
	SetTimer(0,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void NoteDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->KillTimer(0);
	HFONT hfont = ::CreateFontIndirect(&m_font);
	CFont* pfont = CFont::FromHandle(hfont);
	CDC *pDC=m_show.GetDC();
	CRect r;
	m_show.GetClientRect(&r);
	pDC->FillSolidRect(&r,RGB(255,255,255));
	CFont *old=pDC->SelectObject(pfont);
	COLORREF oldcl=pDC->SetTextColor(m_color);
	int nbk=pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(3,3,m_lable);
	pDC->SetTextColor(oldcl);
	pDC->SelectObject(old);
	pDC->SetBkMode(nbk);
	m_show.ReleaseDC(pDC);
	::DeleteObject(hfont);
	CDialog::OnTimer(nIDEvent);
}

void NoteDlg::ViewFont()
{
	UpdateData();
	if(!(m_lable==L""))
	{
		HFONT hfont = ::CreateFontIndirect(&m_font);
		CFont* pfont = CFont::FromHandle(hfont);
		CDC *pDC=m_show.GetDC();
		CRect r;
		m_show.GetClientRect(&r);
		pDC->FillSolidRect(&r,RGB(255,255,255));
		CFont *old=pDC->SelectObject(pfont);
		COLORREF oldcl=pDC->SetTextColor(m_color);
		int nbk=pDC->SetBkMode(TRANSPARENT);
		pDC->TextOutW(3,3,m_lable);
		pDC->SetTextColor(oldcl);
		pDC->SelectObject(old);
		pDC->SetBkMode(nbk);
		m_show.ReleaseDC(pDC);
		::DeleteObject(hfont);
	}
	else AfxMessageBox(L"请填写注记文本");
}