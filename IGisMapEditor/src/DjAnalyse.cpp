// DjAnalyse.cpp : 实现文件
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "IGisMapEditorView.h"
#include "DjAnalyse.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// DjAnalyse 对话框

IMPLEMENT_DYNAMIC(DjAnalyse, CDialog)

DjAnalyse::DjAnalyse(CWnd* pParent /*=NULL*/)
	: CDialog(DjAnalyse::IDD, pParent)
	, ResultLayer(_T(""))
{
	layer1Count=0;
	layer2Count=0;
	selectIndex=-1;
}

DjAnalyse::~DjAnalyse()
{
}

void DjAnalyse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ResultLayer, ResultLayer);
}


BEGIN_MESSAGE_MAP(DjAnalyse, CDialog)
	ON_BN_CLICKED(IDOK, &DjAnalyse::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DjAnalyse::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// DjAnalyse 消息处理程序


void DjAnalyse::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CIGisMapEditorView * pView =(CIGisMapEditorView *)pMainFrame->GetActiveView();
	CFileView * treeView = pMainFrame->GetTree();
	UpdateData();
	if(((CComboBox*)GetDlgItem(IDC_DjType))->GetCurSel()==-1){MessageBox(L"请选择叠加分析类型！");return;}
	else if(((CComboBox*)GetDlgItem(IDC_Layer1))->GetCurSel()==-1){MessageBox(L"请选择叠加图层1！");return;}
	else if(((CComboBox*)GetDlgItem(IDC_Layer2))->GetCurSel()==-1){MessageBox(L"请选择叠加图层2！");return;}
	else if(this->ResultLayer==L""){MessageBox(L"请输入结果图层！");return;}
	else if(this->ResultLayer!=L""&&pMainFrame->db.SearchTable(this->ResultLayer)){MessageBox(L"图层已经存在！请重新命名！");return;}
	UpdateData(FALSE);
	CString table1,table2;
	(CComboBox*)GetDlgItemTextW(IDC_Layer1,table1);
	(CComboBox*)GetDlgItemTextW(IDC_Layer2,table2);
	if(((CComboBox*)GetDlgItem(IDC_DjType))->GetCurSel()==1)
		pMainFrame->db.LineDjPolygon(table1,table2,this->ResultLayer,&pView->MapZoom);
	else if(((CComboBox*)GetDlgItem(IDC_DjType))->GetCurSel()==0)
		pMainFrame->db.PolyDjPoly(table1,table2,this->ResultLayer,&pView->MapZoom);
	Layer L;
    int ntype = ((CComboBox*)GetDlgItem(IDC_DjType))->GetCurSel();
	if(ntype==0)
	    L.SetEx(2,false,false,this->ResultLayer);
	else if(ntype==1)
		L.SetEx(1,false,false,this->ResultLayer);
	int nsz=treeView->thelayers.size();
	treeView->thelayers.add(L);
	if(ntype==0)
		ntype=2;
	HTREEITEM hitem=treeView->m_wndFileView.InsertItem(this->ResultLayer,ntype+2,ntype+2,treeView->hLayers);
	treeView->m_wndFileView.SetItemData(hitem,(DWORD_PTR)treeView->thelayers.LayerPtr(nsz));
	treeView->m_wndFileView.Expand(treeView->hLayers, TVE_EXPAND);
	CDialog::OnOK();
}


void DjAnalyse::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void DjAnalyse::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i=0,layerCount=0,index=-1;
	Layer * layer;
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CFileView * treeView=pMainFrame->GetTree();
	layerCount=treeView->thelayers.size();
	index=((CComboBox*)GetDlgItem(IDC_DjType))->GetCurSel();
	if(index!=selectIndex)
	{
		UpdateData();
		this->selectIndex=index;
		while(i<this->layer1Count)
		{
			((CComboBox*)GetDlgItem(IDC_Layer1))->DeleteString(0);
			i++;
		}
		i=0;
		while(i<this->layer2Count)
		{
			((CComboBox*)GetDlgItem(IDC_Layer2))->DeleteString(0);
			i++;
		}
		this->layer1Count=this->layer2Count=i=0;
		while(i<layerCount)
		{
			layer=treeView->thelayers.LayerPtr(i);
			if(this->selectIndex==0&&layer->GetType()==2)///区叠加区
			{
				((CComboBox*)GetDlgItem(IDC_Layer1))->AddString(layer->GetName());
				this->layer1Count++;
			}
			else if(this->selectIndex==1&&layer->GetType()==1)///线叠加区
			{
				((CComboBox*)GetDlgItem(IDC_Layer1))->AddString(layer->GetName());
				this->layer1Count++;
			}
			if(layer->GetType()==2)////更新区
			{
				((CComboBox*)GetDlgItem(IDC_Layer2))->AddString(layer->GetName());
			    this->layer2Count++;
			}
			i++;
		}
		((CComboBox*)GetDlgItem(IDC_Layer1))->SetCurSel(0);
	    ((CComboBox*)GetDlgItem(IDC_Layer2))->SetCurSel(0);
		UpdateData(false);
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL DjAnalyse::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int i=0,layerCount=0;
	Layer * layer;
	CMainFrame * pMainFrame = (CMainFrame *)::AfxGetMainWnd();
	CFileView * treeView=pMainFrame->GetTree();
	layerCount=treeView->thelayers.size();
	while(i<layerCount)
	{
		layer=treeView->thelayers.LayerPtr(i);
		if(layer->GetType()==1)///线
		{
			((CComboBox*)GetDlgItem(IDC_Layer1))->AddString(layer->GetName());
			this->layer1Count++;
		}
		else if(layer->GetType()==2)///区
		{
			((CComboBox*)GetDlgItem(IDC_Layer2))->AddString(layer->GetName());
			this->layer2Count++;
		}
		i++;
	}
	((CComboBox*)GetDlgItem(IDC_DjType))->SetCurSel(1);////1线对区叠加，，0区对区叠加
	((CComboBox*)GetDlgItem(IDC_Layer1))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_Layer2))->SetCurSel(0);
	this->selectIndex=1;
	this->SetTimer(0,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}