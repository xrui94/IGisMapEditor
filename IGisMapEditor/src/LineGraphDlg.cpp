// LineGraphDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "LineGraphDlg.h"
#include "afxdialogex.h"


// CLineGraphDlg 对话框

IMPLEMENT_DYNAMIC(CLineGraphDlg, CDialog)

CLineGraphDlg::CLineGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineGraphDlg::IDD, pParent)
	, penstyle(-10)
{
	color=RGB(0,0,0);
}

CLineGraphDlg::~CLineGraphDlg()
{
}

void CLineGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STYLEVIEW, m_see);
}


BEGIN_MESSAGE_MAP(CLineGraphDlg, CDialog)
	ON_BN_CLICKED(IDC_RIVER, &CLineGraphDlg::OnBnClickedRiver)
	ON_BN_CLICKED(IDC_DRYRIVER, &CLineGraphDlg::OnBnClickedDryriver)
	ON_BN_CLICKED(IDC_PIPE, &CLineGraphDlg::OnBnClickedPipe)
	ON_BN_CLICKED(IDC_NATIONALROAD, &CLineGraphDlg::OnBnClickedNationalroad)
	ON_BN_CLICKED(IDC_PROVINCEROAD, &CLineGraphDlg::OnBnClickedProvinceroad)
	ON_BN_CLICKED(IDC_CITYROAD, &CLineGraphDlg::OnBnClickedCityroad)
	ON_BN_CLICKED(IDC_COUNTRYROAD, &CLineGraphDlg::OnBnClickedCountryroad)
	ON_BN_CLICKED(IDC_VILLAGEROAD, &CLineGraphDlg::OnBnClickedVillageroad)
	ON_BN_CLICKED(IDC_PROVINCEBORDER, &CLineGraphDlg::OnBnClickedProvinceborder)
	ON_BN_CLICKED(IDC_CITYBORDER, &CLineGraphDlg::OnBnClickedCityborder)
	ON_BN_CLICKED(IDOK, &CLineGraphDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLineGraphDlg 消息处理程序


void CLineGraphDlg::OnBnClickedRiver()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_SOLID;
	color=RGB(0,0,235);View();
}


void CLineGraphDlg::OnBnClickedDryriver()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_DASH;
	color=RGB(0,0,235);View();
}


void CLineGraphDlg::OnBnClickedPipe()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_SOLID;
	color=RGB(0,0,0);View();
}


void CLineGraphDlg::OnBnClickedNationalroad()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_SOLID;
	color=RGB(245,0,0);View();
}


void CLineGraphDlg::OnBnClickedProvinceroad()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_SOLID;
	color=RGB(227,138,66);View();
}


void CLineGraphDlg::OnBnClickedCityroad()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_SOLID;
	color=RGB(98,98,98);View();
}


void CLineGraphDlg::OnBnClickedCountryroad()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_SOLID;
	color=RGB(0,0,0);View();
}


void CLineGraphDlg::OnBnClickedVillageroad()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_DASH;
	color=RGB(0,0,0);View();
}


void CLineGraphDlg::OnBnClickedProvinceborder()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_DASHDOTDOT;
	color=RGB(0,0,0);View();
}


void CLineGraphDlg::OnBnClickedCityborder()
{
	// TODO: 在此添加控件通知处理程序代码
	penstyle=PS_DASHDOT;
	color=RGB(0,0,0);View();
}

void CLineGraphDlg::View()
{
	// TODO: 在此添加控件通知处理程序代码
	CDC *pDC=m_see.GetDC();
	CRect r;
	m_see.GetClientRect(&r);
	CPen pen;
	LOGBRUSH logBrush;  
	memset(&logBrush, 0, sizeof(logBrush)); 
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = color;
	pen.CreatePen(penstyle|PS_GEOMETRIC,3, &logBrush);	
	int rwidth=r.Width();
	int rheight=r.Height();
	pDC->SelectStockObject(WHITE_PEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->Rectangle(&r);
	pDC->SelectObject(pen);
	pDC->MoveTo(10,rheight/2);
	pDC->LineTo(rwidth-10,rheight/2);
	m_see.ReleaseDC(pDC);
}


void CLineGraphDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(penstyle<0)
	{
		AfxMessageBox(L"未选中");
	}
	else
	{
		CDialog::OnOK();
	}
}
