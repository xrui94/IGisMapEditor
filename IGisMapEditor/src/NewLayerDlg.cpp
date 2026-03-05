// NewLayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "NewLayerDlg.h"
#include "afxdialogex.h"


// CNewLayerDlg 对话框

IMPLEMENT_DYNAMIC(CNewLayerDlg, CDialog)

CNewLayerDlg::CNewLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewLayerDlg::IDD, pParent)
	, m_name(_T(""))
	, m_nindex(0)
{

}

CNewLayerDlg::~CNewLayerDlg()
{
}

void CNewLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Control(pDX, IDC_TYPE, m_combo);
}


BEGIN_MESSAGE_MAP(CNewLayerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNewLayerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNewLayerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL CNewLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_combo.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CNewLayerDlg 消息处理程序



void CNewLayerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(m_name!=L"")
	{
		m_nindex=m_combo.GetCurSel();
		CDialog::OnOK();
	}
	else
		MessageBox(L"请填写表名（图层名）",L"ERROR",MB_ICONWARNING|MB_OK);
}


void CNewLayerDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
