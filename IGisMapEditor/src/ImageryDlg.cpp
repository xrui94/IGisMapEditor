// ImageryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "ImageryDlg.h"
#include "afxdialogex.h"


// CImageryDlg 对话框

IMPLEMENT_DYNAMIC(CImageryDlg, CDialog)

CImageryDlg::CImageryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageryDlg::IDD, pParent)
	, m_height(0)
	, m_width(0)
	, m_xmin(0.0)
	, m_ymin(0.0)
	, m_xmax(0.0)
	, m_ymax(0.0)
{
	bpp24=mappix=itempix=true;
}

CImageryDlg::~CImageryDlg()
{
}

void CImageryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEG, m_height);
	DDX_Text(pDX, IDC_WID, m_width);
	DDX_Text(pDX, IDC_XMIN, m_xmin);
	DDX_Text(pDX, IDC_YMIN, m_ymin);
	DDX_Text(pDX, IDC_XMAX, m_xmax);
	DDX_Text(pDX, IDC_YMAX, m_ymax);
	DDX_Control(pDX, IDC_PicUnit, m_mapunit);
	DDX_Control(pDX, IDC_ItemUnit, m_itemunit);
	DDX_Control(pDX, IDC_BPP, m_bpp);
}


BEGIN_MESSAGE_MAP(CImageryDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CImageryDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CImageryDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CImageryDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDOK, &CImageryDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CImageryDlg 消息处理程序


BOOL CImageryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//UpdateData();
	// TODO:  在此添加额外的初始化
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	m_xmin=pZoom->lt.x;
	m_ymin=pZoom->rb.y;
	m_xmax=pZoom->rb.x;
	m_ymax=pZoom->lt.y;
	
	//m_width=m_width;
	//m_height=m_height;
	UpdateData(FALSE);
	m_mapunit.SetCurSel(0);
	m_itemunit.SetCurSel(0);
	m_bpp.SetCurSel(0);

	
/*
	int n=m_width;
	CString str;
	str.Format(_T("%d"),n);
	//SetDlgItemText(IDC_WID,str);
	((CEdit*)GetDlgItem(IDC_WID))->SetWindowText(str);
	((CEdit*)GetDlgItem(IDC_WID))->UpdateWindow();*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CImageryDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_xmin=pZoom->lt.x;
	m_ymin=pZoom->rb.y;
	m_xmax=pZoom->rb.x;
	m_ymax=pZoom->lt.y;
	UpdateData(FALSE);
}


void CImageryDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	db->getTableInfo(m_xmin,m_ymin,m_xmax,m_ymax);
	UpdateData(FALSE);
}


void CImageryDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_xmin=0.0;
	m_ymin=0.0;
	m_xmax=0.0;
	m_ymax=0.0;
	UpdateData(FALSE);
}


void CImageryDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_width<=0||m_height<=0)
	{MessageBox(_T("图像尺寸输入有误"));}
	else if(m_xmin>=m_xmax||m_ymin>=m_ymax)
	{MessageBox(_T("坐标信息有误"));}
	else
	{
		mappix=(0==m_mapunit.GetCurSel());
		itempix=(0==m_itemunit.GetCurSel());
		bpp24=(0==m_bpp.GetCurSel());
		//
		CDialog::OnOK();
	}
}
