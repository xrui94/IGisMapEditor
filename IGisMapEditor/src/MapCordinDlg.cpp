// MapCordinDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "MapCordinDlg.h"
#include "afxdialogex.h"

UINT ID_TIMER1=30667;
// CMapCordinDlg 对话框

IMPLEMENT_DYNAMIC(CMapCordinDlg, CDialog)

CMapCordinDlg::CMapCordinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapCordinDlg::IDD, pParent)
	, m_ltx(0.0)
	, m_lty(0.0)
	, m_rbx(0.0)
	, m_rby(0.0)
{
	shown=false;
}

CMapCordinDlg::~CMapCordinDlg()
{
}

void CMapCordinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LTX, m_ltx);
	DDX_Text(pDX, IDC_LTY, m_lty);
	DDX_Text(pDX, IDC_RBX, m_rbx);
	DDX_Text(pDX, IDC_RBY, m_rby);
	DDX_Control(pDX, IDC_Map, m_map);
}


BEGIN_MESSAGE_MAP(CMapCordinDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMapCordinDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMapCordinDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapCordinDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMapCordinDlg 消息处理程序


void CMapCordinDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_ltx<m_rbx&&m_rby<m_lty)
		{CDialog::OnOK();KillTimer(ID_TIMER1);}
	else
	{AfxMessageBox(L"填入的数据有误");}
}


void CMapCordinDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

		CImage img;
		img.Load(pathname);
		m_ltx=m_rby=0;
		m_lty=img.GetHeight();
		m_rbx=img.GetWidth();
		CDialog::OnCancel();
		KillTimer(ID_TIMER1);
}


BOOL CMapCordinDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(ID_TIMER1,1,NULL);
	shown=false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMapCordinDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!shown)
	{
		CDC*pDC=m_map.GetDC();
		CRect r;
		m_map.GetClientRect(&r);
		CImage img;
		img.Load(pathname);
		::StretchBlt(pDC->m_hDC,0,0,r.Width(),r.Height(),img.GetDC(),0,0,img.GetWidth(),img.GetHeight(),SRCCOPY);
		img.ReleaseDC();
		shown=true;
	}
	CDialog::OnTimer(nIDEvent);
}


void CMapCordinDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
		CDC*pDC=m_map.GetDC();
		CRect r;
		m_map.GetClientRect(&r);
		CImage img;
		img.Load(pathname);
		::StretchBlt(pDC->m_hDC,0,0,r.Width(),r.Height(),img.GetDC(),0,0,img.GetWidth(),img.GetHeight(),SRCCOPY);
		img.ReleaseDC();
}
