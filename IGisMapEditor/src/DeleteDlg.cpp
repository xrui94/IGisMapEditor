// DeleteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "DeleteDlg.h"
#include "afxdialogex.h"

UINT ID_TIMERDEL =901;
// CDeleteDlg 对话框

IMPLEMENT_DYNAMIC(CDeleteDlg, CDialog)

CDeleteDlg::CDeleteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteDlg::IDD, pParent)
	, nindex(0)
	, lname(_T(""))
{

}

CDeleteDlg::~CDeleteDlg()
{
}

void CDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_viewtable);
}


BEGIN_MESSAGE_MAP(CDeleteDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDeleteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDeleteDlg::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDeleteDlg 消息处理程序

CString GetType(int type)
{
	CString str;
	switch(type)
	{
	case 0:str=L"点图层";break;
	case 1:str=L"线图层";break;
	case 2:str=L"区图层";break;
	case 3:str=L"注记图层";break;
	default:str=L"";break;
	}
	return str;
}

void CDeleteDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(nindex!=-1&&!pView->isUsing(lname))
	{
		KillTimer(ID_TIMERDEL);
		CDialog::OnOK();
	}
	else AfxMessageBox(L"没有选中图层或图层正被使用!");
}


void CDeleteDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(ID_TIMERDEL);CDialog::OnCancel();
}


BOOL CDeleteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(ID_TIMERDEL,10,NULL);
	CRect r;
	CatalogTable tableSet[50];
	m_viewtable.GetClientRect(&r);
	m_viewtable.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_viewtable.InsertColumn(0,_T("ID"));
	m_viewtable.InsertColumn(1,_T("图层名"));
	m_viewtable.InsertColumn(2,_T("类型"));

	m_viewtable.SetColumnWidth(0,40);
	m_viewtable.SetColumnWidth(1,r.Width()-100);
	m_viewtable.SetColumnWidth(2,60);
	int tableCount=db->GetDataBaseTable(tableSet);
	int n=0;
	CString tmp;
	while(n<tableCount)
	{
		m_viewtable.InsertItem(n,tableSet[n].ID);
		m_viewtable.SetItemText(n,1,tableSet[n].tableName);
		tmp=GetType(tableSet[n].tableType-1);
		m_viewtable.SetItemText(n,2,tmp);
		n++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDeleteDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	nindex=m_viewtable.GetNextItem(-1,LVNI_SELECTED);
	if(nindex==-1){;}
	else
	{
		//CString str=m_viewtable.GetItemText(nindex,1);
		lname=m_viewtable.GetItemText(nindex,1);
	}
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
