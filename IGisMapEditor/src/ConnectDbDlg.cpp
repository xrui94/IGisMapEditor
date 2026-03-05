// ConnectDbDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "ConnectDbDlg.h"
#include "afxdialogex.h"
#include <locale>
#include <string>

#include "MainFrm.h"
using namespace std;

// CConnectDbDlg 对话框

IMPLEMENT_DYNAMIC(CConnectDbDlg, CDialog)

CConnectDbDlg::CConnectDbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDbDlg::IDD, pParent)
	, m_server(_T(""))
	, m_name(_T(""))
	, m_user(_T(""))
	, m_pin(_T(""))
	, m_remember(FALSE)
	, m_rememberPin(FALSE)
{

}

CConnectDbDlg::~CConnectDbDlg()
{
}

void CConnectDbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DBSERVER, m_server);
	DDX_Text(pDX, IDC_DBNAME, m_name);
	DDX_Text(pDX, IDC_DBHOST, m_user);
	DDX_Text(pDX, IDC_DBPIN, m_pin);
	DDX_Check(pDX, IDC_REMEMBER, m_remember);
	DDX_Check(pDX, IDC_REMEMBERCODE, m_rememberPin);
}


BEGIN_MESSAGE_MAP(CConnectDbDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConnectDbDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CConnectDbDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CFileFind find;
	if(TRUE==find.FindFile(L"log.txt"))
	{
		m_remember=TRUE;m_rememberPin=FALSE;
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );    setlocale( LC_CTYPE, "chs" );
		CStdioFile f;
		f.Open(L"log.txt",CFile::modeRead);
		CString str;
		f.ReadString(str);
		
		f.ReadString(m_server);
		f.ReadString(m_name);
		f.ReadString(m_user);
		if(str==L"4")
		{f.ReadString(m_pin);m_rememberPin=TRUE;}
		f.Close();
		setlocale( LC_CTYPE, old_locale );    free( old_locale );//
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
// CConnectDbDlg 消息处理程序


void CConnectDbDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if(pdb->isConnection())
	{
		MessageBox(L"数据库已经连接!");
		return;

	}
	if(pdb->Open(m_server,m_name,m_user,m_pin))
	{
		if(m_remember==TRUE)
		{
			CFileFind ffind;
			if(TRUE==ffind.FindFile(L"log.txt"))
				CFile::Remove(L"log.txt");
			CStdioFile f;
			char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );    
			setlocale( LC_CTYPE, "chs" );
			f.Open(L"log.txt",CFile::modeCreate|CFile::modeWrite);
			if(TRUE==m_rememberPin) f.WriteString(L"4\n");
			else f.WriteString(L"3\n");
			f.WriteString((LPCTSTR)(m_server+L"\n"));
			f.WriteString((LPCTSTR)(m_name+L"\n"));
			f.WriteString((LPCTSTR)(m_user+L"\n"));
			if(TRUE==m_rememberPin) f.WriteString((LPCTSTR)(m_pin+L"\n"));
			f.Close();
			setlocale( LC_CTYPE, old_locale );    free( old_locale );//
		}
		else CFile::Remove(L"log.txt");
		CDialog::OnOK();
	}
	else AfxMessageBox(L"连接不可用");
	UpdateData(FALSE);
}


