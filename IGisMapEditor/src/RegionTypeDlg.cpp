// RegionTypeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "RegionTypeDlg.h"
#include "afxdialogex.h"
#include "SColorDlg.h"

// CRegionTypeDlg 对话框

IMPLEMENT_DYNAMIC(CRegionTypeDlg, CDialog)

CRegionTypeDlg::CRegionTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegionTypeDlg::IDD, pParent)
{
	m_sideType=PS_SOLID;
	m_fillColor=m_sideColor=RGB(0,0,0);
	p.x=500;
	p.y=250;
}

CRegionTypeDlg::~CRegionTypeDlg()
{
}

void CRegionTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Pic, m_viewTotal);
	DDX_Control(pDX, IDC_SIDESEE, m_viewRgnSide);
	DDX_Control(pDX, IDC_FILLSEE, m_viewRgnFill);
}


BEGIN_MESSAGE_MAP(CRegionTypeDlg, CDialog)
	ON_BN_CLICKED(IDC_SIDECL, &CRegionTypeDlg::OnBnClickedSidecl)
	ON_BN_CLICKED(IDC_SOLID, &CRegionTypeDlg::OnBnClickedSolid)
	ON_BN_CLICKED(IDC_DASH, &CRegionTypeDlg::OnBnClickedDash)
	ON_BN_CLICKED(IDC_DOT, &CRegionTypeDlg::OnBnClickedDot)
	ON_BN_CLICKED(IDC_DASHDOT, &CRegionTypeDlg::OnBnClickedDashdot)
	ON_BN_CLICKED(IDC_DASHDOTDOT, &CRegionTypeDlg::OnBnClickedDashdotdot)
	ON_BN_CLICKED(IDC_RGNCL, &CRegionTypeDlg::OnBnClickedRgncl)
	ON_BN_CLICKED(IDOK, &CRegionTypeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRegionTypeDlg::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRegionTypeDlg 消息处理程序


void CRegionTypeDlg::OnBnClickedSidecl()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog clg;//(NULL,CC_RGBINIT,this);
	clg.m_cc.Flags|=CC_RGBINIT;
	//CColorDlg clg;
	if(IDOK==clg.DoModal())
	{
		m_sideColor=clg.GetColor();
		CDC*pDC=m_viewRgnSide.GetDC();
		CRect r;m_viewRgnSide.GetClientRect(&r);
		CBrush b(m_sideColor);
		pDC->SelectObject(&b);
		pDC->FillRect(&r,&b);
		m_viewRgnSide.ReleaseDC(pDC);
		OnBnClickedViewit();
	}
}


void CRegionTypeDlg::OnBnClickedSolid()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sideType=PS_SOLID;
	OnBnClickedViewit();
}


void CRegionTypeDlg::OnBnClickedDash()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sideType=PS_DASH;
	OnBnClickedViewit();
}


void CRegionTypeDlg::OnBnClickedDot()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sideType=PS_DOT;
	OnBnClickedViewit();
}


void CRegionTypeDlg::OnBnClickedDashdot()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sideType=PS_DASHDOT;
	OnBnClickedViewit();
}


void CRegionTypeDlg::OnBnClickedDashdotdot()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sideType=PS_DASHDOTDOT;
	OnBnClickedViewit();
}


void CRegionTypeDlg::OnBnClickedRgncl()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog g;//(NULL,CC_RGBINIT,this);
	g.m_cc.Flags|=CC_RGBINIT;
	//CColorDlg g;
	if(IDOK==g.DoModal())
	{
		m_fillColor=g.GetColor();
		CDC*pDC=m_viewRgnFill.GetDC();
		CRect r;m_viewRgnFill.GetClientRect(&r);
		CBrush b(m_fillColor);
		pDC->SelectObject(&b);
		pDC->FillRect(&r,&b);
		m_viewRgnFill.ReleaseDC(pDC);
		OnBnClickedViewit();
	}
}


void CRegionTypeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}


void CRegionTypeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CRegionTypeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->KillTimer(0);
	CDC*pDC;
	CBrush * b;
	CRect r;
	pDC=m_viewTotal.GetDC();
	m_viewTotal.GetClientRect(&r);
	b=new CBrush(m_fillColor);
	CPen pen(m_sideType,1,m_sideColor);
	pDC->SelectObject(b);
	pDC->SelectObject(&pen);
	pDC->Rectangle(10,10,r.Width()-10,r.Height()-10);
	m_viewTotal.ReleaseDC(pDC);

	pDC=m_viewRgnSide.GetDC();
	m_viewRgnSide.GetClientRect(&r);
	b=new CBrush(m_sideColor);
	pDC->SelectObject(&b);
	pDC->FillRect(&r,b);
	m_viewRgnSide.ReleaseDC(pDC);

	pDC=m_viewRgnFill.GetDC();
	m_viewRgnFill.GetClientRect(&r);
	b=new CBrush(m_fillColor);
	pDC->SelectObject(b);
	pDC->FillRect(&r,b);
	m_viewRgnFill.ReleaseDC(pDC);
	
	switch(m_sideType)
	{
		case PS_DOT:((CButton*)GetDlgItem(IDC_DOT))->SetCheck(TRUE);break;
		case PS_DASHDOTDOT:((CButton*)GetDlgItem(IDC_DASHDOTDOT))->SetCheck(TRUE);break;
		case PS_DASH:((CButton*)GetDlgItem(IDC_DASH))->SetCheck(TRUE);break;
		case PS_DASHDOT:((CButton*)GetDlgItem(IDC_DASHDOT))->SetCheck(TRUE);break;
		case PS_SOLID:((CButton*)GetDlgItem(IDC_SOLID))->SetCheck(TRUE);break;
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL CRegionTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowPos(&wndTopMost,p.x,p.y,100,100,SWP_NOSIZE);
	SetTimer(0,100,NULL);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRegionTypeDlg::OnBnClickedViewit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDC*pDC=m_viewTotal.GetDC();
	CRect r;m_viewTotal.GetClientRect(&r);
	CBrush b(m_fillColor);
	CPen pen(m_sideType,1,m_sideColor);
	pDC->SelectObject(&b);
	pDC->SelectObject(&pen);
	pDC->Rectangle(10,10,r.Width()-10,r.Height()-10);
	m_viewTotal.ReleaseDC(pDC);
}