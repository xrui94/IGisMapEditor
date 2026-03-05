// LineTypeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "LineTypeDlg.h"
#include "afxdialogex.h"
#include "SColorDlg.h"
#include "LineGraphDlg.h"

// CLineTypeDlg 对话框

IMPLEMENT_DYNAMIC(CLineTypeDlg, CDialog)

CLineTypeDlg::CLineTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineTypeDlg::IDD, pParent)
	, m_width(1)
	, PENSTYLE(PS_SOLID)
	, m_curve(FALSE)
{
	m_color=RGB(0,0,0);
	penStyle=PENSTYLE|PS_GEOMETRIC;
	p.x=500;
	p.y=250;
}

CLineTypeDlg::~CLineTypeDlg()
{
}

void CLineTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_width);
	DDX_Control(pDX, IDC_LINEVIEW, m_result);
	DDX_Control(pDX, IDC_COLORSEE, m_colorPic);
	DDX_Check(pDX, IDC_CHECK1, m_curve);
}


BEGIN_MESSAGE_MAP(CLineTypeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CLineTypeDlg::OnBnClickedColorbtn)
	ON_BN_CLICKED(IDOK, &CLineTypeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLineTypeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DASH, &CLineTypeDlg::OnBnClickedDash)
	ON_BN_CLICKED(IDC_DOT, &CLineTypeDlg::OnBnClickedDot)
	ON_BN_CLICKED(IDC_DASHDOT, &CLineTypeDlg::OnBnClickedDashdot)
	ON_BN_CLICKED(IDC_DASHDOTDOT, &CLineTypeDlg::OnBnClickedDashdotdot)
	ON_BN_CLICKED(IDC_SOLID, &CLineTypeDlg::OnBnClickedSolid)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CLineTypeDlg::OnDeltaposSpin1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CLineTypeDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLineTypeDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLineTypeDlg 消息处理程序


void CLineTypeDlg::OnBnClickedColorbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CColorDialog cldlg;//(NULL,CC_RGBINIT,this);
	cldlg.m_cc.Flags|=CC_RGBINIT;
	//CColorDlg cldlg;
	if(IDOK==cldlg.DoModal())
	{
		m_color=cldlg.GetColor();
		CRect r;
		m_colorPic.GetClientRect(&r);
		CDC *pDC=m_colorPic.GetDC();
		CBrush brush(m_color);
		pDC->FillRect(&r,&brush);
		m_colorPic.ReleaseDC(pDC);
		ViewEffect();
	}
}


void CLineTypeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}


void CLineTypeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CLineTypeDlg::OnBnClickedDash()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int n=1;
	if(m_curve==TRUE)
	{n=-1;}
	PENSTYLE=PS_DASH;
	penStyle=PENSTYLE|PS_GEOMETRIC;
	ViewEffect();
}


void CLineTypeDlg::OnBnClickedDot()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	PENSTYLE=PS_DOT;
	penStyle=PENSTYLE|PS_GEOMETRIC;
	ViewEffect();
}


void CLineTypeDlg::OnBnClickedDashdot()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	PENSTYLE=PS_DASHDOT;
	penStyle=PENSTYLE|PS_GEOMETRIC;
	ViewEffect();
}


void CLineTypeDlg::OnBnClickedDashdotdot()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	PENSTYLE=PS_DASHDOTDOT;
	penStyle=PENSTYLE|PS_GEOMETRIC;
	ViewEffect();
}


void CLineTypeDlg::OnBnClickedSolid()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	PENSTYLE=PS_SOLID;
	penStyle=PENSTYLE|PS_GEOMETRIC;
	ViewEffect();
}


void CLineTypeDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(pNMUpDown-> iDelta == 1) // 如果此值为1 , 说明点击了Spin的往下箭头 
	{  
		if(m_width>1) m_width--;
		UpdateData(FALSE);
	} 
	else if(pNMUpDown-> iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头 
	{ 
		if(m_width<20) m_width++;
		UpdateData(FALSE);
	}
	ViewEffect();
}

void CLineTypeDlg::ViewEffect(void)
{
	UpdateData();
	LOGBRUSH logBrush;  
	memset(&logBrush, 0, sizeof(logBrush)); 
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = m_color;
	CPen pen; 
	//int nps=abs(int(penStyle));
	pen.CreatePen(PENSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND|PS_JOIN_ROUND, m_width, &logBrush);
	CDC* pDC = m_result.GetDC();	
	CRect r;
	m_result.GetClientRect(&r);
	int rwidth=r.Width();
	int rheight=r.Height();
	pDC->SelectStockObject(WHITE_PEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->Rectangle(&r);
	pDC->SelectObject(pen);
	if(m_curve==TRUE)
	{
		CPoint pt[4]={CPoint(20,rheight-20),CPoint((rwidth-40)/2,20),CPoint((rwidth+20)/2,20),CPoint(rwidth-20,rheight-20)};
		pDC->PolyBezier(pt,4);
	}
	else
	{
		pDC->MoveTo(20,rheight-20);
		pDC->LineTo((rwidth-20)/2,20);
		pDC->MoveTo((rwidth-20)/2,20);
		pDC->LineTo(rwidth-20,rheight-20);
	}
	m_result.ReleaseDC(pDC);
}


BOOL CLineTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTypeCheck();
	ViewEffect();////初始化Picture Control控件IDC_LINEVIEW
	this->SetWindowPos(&wndTopMost,p.x,p.y,100,100,SWP_NOSIZE);
	SetTimer(0,100,NULL);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLineTypeDlg::SetTypeCheck(void)
{
	switch(PENSTYLE)
	{
		case PS_SOLID:((CButton*)GetDlgItem(IDC_SOLID))->SetCheck(TRUE);break;
		case PS_DASH:((CButton*)GetDlgItem(IDC_DASH))->SetCheck(TRUE);break;
		case PS_DOT:((CButton*)GetDlgItem(IDC_DOT))->SetCheck(TRUE);break;
		case PS_DASHDOT:((CButton*)GetDlgItem(IDC_DASHDOT))->SetCheck(TRUE);break;
		case PS_DASHDOTDOT:((CButton*)GetDlgItem(IDC_DASHDOTDOT))->SetCheck(TRUE);break;
	    default:{PENSTYLE=PS_SOLID;((CButton*)GetDlgItem(IDC_SOLID))->SetCheck(TRUE);}break;
	}
}

void CLineTypeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->ViewEffect();
	this->SetTypeCheck();
	CRect r;
	m_colorPic.GetClientRect(&r);
	CDC *pDC=m_colorPic.GetDC();
	CBrush brush(m_color);
	pDC->FillRect(&r,&brush);
	m_colorPic.ReleaseDC(pDC);
	this->KillTimer(0);
	CDialog::OnTimer(nIDEvent);
}


void CLineTypeDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	ViewEffect();
}


void CLineTypeDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CLineGraphDlg g;
	if(IDOK==g.DoModal())
	{
		m_color=g.color;

		switch(PENSTYLE)
		{
			case PS_SOLID:((CButton*)GetDlgItem(IDC_SOLID))->SetCheck(FALSE);break;
			case PS_DASH:((CButton*)GetDlgItem(IDC_DASH))->SetCheck(FALSE);break;
			case PS_DOT:((CButton*)GetDlgItem(IDC_DOT))->SetCheck(FALSE);break;
			case PS_DASHDOT:((CButton*)GetDlgItem(IDC_DASHDOT))->SetCheck(FALSE);break;
			case PS_DASHDOTDOT:((CButton*)GetDlgItem(IDC_DASHDOTDOT))->SetCheck(FALSE);break;
			default:{PENSTYLE=PS_SOLID;((CButton*)GetDlgItem(IDC_SOLID))->SetCheck(TRUE);}break;
		}

		PENSTYLE=g.penstyle;
		
		SetTypeCheck();
		ViewEffect();

		CRect r;
		m_colorPic.GetClientRect(&r);
		CDC *pDC=m_colorPic.GetDC();
		CBrush brush(m_color);
		pDC->FillRect(&r,&brush);
		m_colorPic.ReleaseDC(pDC);
	}
}
