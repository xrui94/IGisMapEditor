// AddLayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "AddLayerDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DataBase.h"

UINT ID_TIMERADD =900;
// CAddLayerDlg 对话框

IMPLEMENT_DYNAMIC(CAddLayerDlg, CDialog)

CAddLayerDlg::CAddLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddLayerDlg::IDD, pParent)
	, m_selectedinfo(_T(""))
	, m_selindex(-1)
	, m_selid(0)
{

}

CAddLayerDlg::~CAddLayerDlg()
{
}

void CAddLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABLES, m_lists);
	DDX_Text(pDX, IDC_SELECTED, m_selectedinfo);
}


BEGIN_MESSAGE_MAP(CAddLayerDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAddLayerDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CAddLayerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddLayerDlg 消息处理程序


void CAddLayerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_selindex!=-1)
	{
		KillTimer(ID_TIMERADD);
		CDialog::OnOK();
	}
	else AfxMessageBox(L"没有选中图层!");
}

CString GetTypeStr(int type)
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

BOOL CAddLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(ID_TIMERADD,10,NULL);
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	if(!pMainFrame->db.isConnection())
	   pMainFrame->db.Open(server,name,user,pin);
	CRect r;
	CatalogTable tableSet[50];
	m_lists.GetClientRect(&r);
	m_lists.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lists.InsertColumn(0,_T("ID"));
	m_lists.InsertColumn(1,_T("图层名"));
	m_lists.InsertColumn(2,_T("类型"));

	m_lists.SetColumnWidth(0,40);
	m_lists.SetColumnWidth(1,r.Width()-100);
	m_lists.SetColumnWidth(2,60);
	int tableCount=pMainFrame->db.GetDataBaseTable(tableSet);
	int n=0;
	CString tmp;
	while(n<tableCount)
	{
		m_lists.InsertItem(n,tableSet[n].ID);
		m_lists.SetItemText(n,1,tableSet[n].tableName);
		tmp=GetTypeStr(tableSet[n].tableType-1);
		m_lists.SetItemText(n,2,tmp);
		n++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAddLayerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
/*	m_selindex=m_lists.GetNextItem(-1,LVNI_SELECTED);
	if(m_selindex==-1){m_selectedinfo=L"没有选中";}
	else
	{
		CString str=m_lists.GetItemText(m_selindex,1);
		m_selectedinfo=str;
	}
	UpdateData(FALSE);*/
	CDialog::OnMouseMove(nFlags, point);
}


void CAddLayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_selindex=m_lists.GetNextItem(-1,LVNI_SELECTED);
	if(m_selindex==-1){m_selectedinfo=L"没有选中";m_selid=0;}
	else
	{
		CString str=m_lists.GetItemText(m_selindex,1);
		m_selectedinfo=str;
		CString str2=m_lists.GetItemText(m_selindex,0);
		m_selid=strtoi(str2);
	}
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}


void CAddLayerDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(ID_TIMERADD);
	CDialog::OnCancel();
}
