
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "IGisMapEditor.h"
#include "IGisMapEditorView.h"
#include "MainFrm.h"
#include "ConnectDbDlg.h"
#include "CreateDBDlg.h"
#include "DeleteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MY_WM_NOTIFYICON (WM_USER+1001)

UINT	ID_INDICATOR_X;
UINT	ID_INDICATOR_Y;
UINT	ID_INDICATOR_STATUS;
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_MESSAGE(MY_WM_NOTIFYICON, &CMainFrame::OnNotifyIcon)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_NOTIFY_CLOSE, &CMainFrame::OnNotifyClose)
	ON_WM_CLOSE()
	ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
	ON_COMMAND(ID_SETDBCONNECT, &CMainFrame::OnSetdbconnect)
	ON_COMMAND(ID_CLEARCONNECTION, &CMainFrame::OnClearconnection)
	ON_COMMAND(ID_CREATEDB, &CMainFrame::OnCreatedb)
	ON_COMMAND(ID_DeleteTable, &CMainFrame::OnDeletetable)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,	

};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_bMin=true;
	server=dbname=user=code=L"";
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_MAINFRAME) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//===========
	if (!m_wndBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_DBTOOLBAR) ||
		!m_wndBar.LoadToolBar(IDR_DBTOOLBAR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	//CString nstrToolBarName;
	//bNameValid = nstrToolBarName.LoadString(IDS_TOOLBAR_MY);
	//ASSERT(bNameValid);
	m_wndBar.SetWindowText(L"IGisDBToolBar");

	//CString nstrCustomize;
	//bNameValid = nstrCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);
	m_wndBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//========
//===========
	if (!m_zoomBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_ZOOMBAR) ||
		!m_zoomBar.LoadToolBar(IDR_ZOOMBAR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	//CString nstrToolBarName;
	//bNameValid = nstrToolBarName.LoadString(IDS_TOOLBAR_MY);
	//ASSERT(bNameValid);
	m_zoomBar.SetWindowText(L"IGisZoomBar");

	//CString nstrCustomize;
	//bNameValid = nstrCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);
	m_zoomBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//========
//===========
	if (!m_editBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_EDITBAR) ||
		!m_editBar.LoadToolBar(IDR_EDITBAR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	//CString nstrToolBarName;
	//bNameValid = nstrToolBarName.LoadString(IDS_TOOLBAR_MY);
	//ASSERT(bNameValid);
	m_editBar.SetWindowText(L"IGisEditBar");

	//CString nstrCustomize;
	//bNameValid = nstrCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);
	m_editBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//========
//===========
	if (!m_extBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_EXTBAR) ||
		!m_extBar.LoadToolBar(IDR_EXTBAR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	//CString nstrToolBarName;
	//bNameValid = nstrToolBarName.LoadString(IDS_TOOLBAR_MY);
	//ASSERT(bNameValid);
	m_extBar.SetWindowText(L"IGisExtBar");

	//CString nstrCustomize;
	//bNameValid = nstrCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);
	m_extBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
//========	
	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(0,0,SBPS_STRETCH,50);
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_X,SBPS_NORMAL,100);// 设置X坐标信息窗口(indicators)宽度为100
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_Y,SBPS_NORMAL,100);// 设置Y坐标信息窗口(indicators)宽度为100
	m_wndStatusBar.SetPaneInfo(3,ID_INDICATOR_STATUS,SBPS_NORMAL,150);// 设置Y坐标信息窗口(indicators)宽度为50

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndBar.EnableDocking(CBRS_ALIGN_ANY);
	m_zoomBar.EnableDocking(CBRS_ALIGN_ANY);
	m_editBar.EnableDocking(CBRS_ALIGN_ANY);
	m_extBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndBar);
	DockPane(&m_zoomBar);
	DockPane(&m_extBar);
	DockPaneLeftOf(&m_editBar,&m_zoomBar);//DockPane(&m_editBar);
//	DockPane(&m_wndMenuBar);
//	DockPaneLeftOf(&m_wndToolBar,&m_wndBar);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);


	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);
	//icon
	HICON m_hIcon;
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE); // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon
	//notify icon
    m_ntIcon.cbSize = sizeof(NOTIFYICONDATA);                            //该结构体变量的大小
    m_ntIcon.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);  //图标，通过资源ID得到
    m_ntIcon.hWnd = this->m_hWnd;                                                 //接收托盘图标通知消息的窗口句柄
	WCHAR atip[128] = L"IGis-MapEditor";//鼠标设上面时显示的提示
	//strcpy(m_ntIcon.szTip, 128, atip);
	memcpy(m_ntIcon.szTip,atip,128*sizeof(WCHAR));
    m_ntIcon.uCallbackMessage = MY_WM_NOTIFYICON;                //应用程序定义的消息ID号
    m_ntIcon.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;               //图标的属性：设置成员uCallbackMessage、hIcon、szTip有效
    ::Shell_NotifyIconW(NIM_ADD, &m_ntIcon);                                 //在系统通知区域增加这个图标

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 创建类视图

	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件视图”窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);
	
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

 LRESULT CMainFrame::OnNotifyIcon(WPARAM wparam, LPARAM lparam)
{
   if(lparam == WM_LBUTTONDOWN)
	{
        if(m_bMin == true)
         {
              AfxGetMainWnd()->ShowWindow(SW_SHOW);
              AfxGetMainWnd()->ShowWindow(SW_RESTORE);
              //这里貌似只有写这样两句才能保证恢复窗口后，该窗口处于活动状态（在最前面）
              m_bMin = false;
         }
         else
         {
              AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
			  AfxGetMainWnd()->ShowWindow(SW_HIDE);
              m_bMin = true;
         }  
                         //这里添加对鼠标左键点击的处理，具体处理见（4）
    }
    else if(lparam == WM_RBUTTONDOWN)
    {
		CMenu popMenu;
        popMenu.LoadMenuW(IDR_NOTIFY_MENU);   //IDR_MENU_POPUP是在ResourceView中创建并编辑的一个菜单
        CMenu* pmenu = popMenu.GetSubMenu(0);   //弹出的菜单实际上是IDR_MENU_POPUP菜单的某项的子菜单，这里是第一项
        CPoint pos;
        GetCursorPos(&pos);            //弹出菜单的位置，这里就是鼠标的当前位置
        //显示该菜单，第一个参数的两个值分别表示在鼠标的右边显示、响应鼠标右击
        pmenu->TrackPopupMenu(TPM_RIGHTALIGN|TPM_RIGHTBUTTON, pos.x, pos.y, AfxGetMainWnd(), 0);  
                        //这里添加对鼠标右键点击的处理，具体处理见（5）
    }
    return 0;
 }

 void CMainFrame::OnDestroy()
 {
	 CFrameWndEx::OnDestroy();

	 // TODO: 在此处添加消息处理程序代码
	 ::Shell_NotifyIcon(NIM_DELETE, &m_ntIcon);

 }


 void CMainFrame::OnSize(UINT nType, int cx, int cy)
 {
	 CFrameWndEx::OnSize(nType, cx, cy);

	 // TODO: 在此处添加消息处理程序代码
	 if(nType==SIZE_MINIMIZED)
		 m_bMin=true;
	 else
		 m_bMin=false;
 }


 void CMainFrame::OnNotifyClose()
 {
	 // TODO: 在此添加命令处理程序代码
	 //Call Close Function
	 CFrameWndEx::OnClose();
 }


 void CMainFrame::OnClose()
 {
	 // TODO: 在此添加消息处理程序代码和/或调用默认值
	 int nres=MessageBox(L"确定要退出吗？\n（点击否最小化到托盘）",L"您希望",MB_YESNOCANCEL|MB_ICONQUESTION);
	 switch(nres)
	 {
	 case IDYES:OnAppExit();break;
	 case IDNO:
		 {
			  AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
			  AfxGetMainWnd()->ShowWindow(SW_HIDE);
              m_bMin = true;
		 }break;
	 default:break;
	 }
	 //up
	//CFrameWndEx::OnClose();
 }


 void CMainFrame::OnAppExit()
 {
	 // TODO: 在此添加命令处理程序代码

	 //up
	 CFrameWndEx::OnClose();
 }


 CVisibleManager* CMainFrame::GetVisibles(void)
 {
	 VManager.Init(m_wndFileView);
	 return &(VManager);
 }


 CIGisMapEditorView* CMainFrame::GetView(void)
 {
	 CIGisMapEditorView* pview=(CIGisMapEditorView*)(this->GetActiveView());
	 return pview;
 }

 void CMainFrame::OnCreatedb()
 {
	 // TODO: 在此添加命令处理程序代码
	 CCreateDBDlg dlg;
	 if(IDOK==dlg.DoModal())
	 {
		MessageBox(L"成功创建数据库",L"提示",MB_ICONINFORMATION|MB_OK);
		 return;
	 }
 }

 void CMainFrame::OnSetdbconnect()
 {
	 // TODO: 在此添加命令处理程序代码
	 CConnectDbDlg dlg;
	 dlg.pdb=&db;
	 if(IDOK==dlg.DoModal())
	 {
		 server=dlg.m_server;
		 dbname=dlg.m_name;
		 user=dlg.m_user;
		 code=dlg.m_pin;
//		 this->db.Open(this->server,this->dbname,this->user,this->code);
	 }
 }
 void CMainFrame::OnClearconnection()
 {
	 Disconn();
 }

 void CMainFrame::Disconn(bool nocall)
 {
	 // TODO: 在此添加命令处理程序代码
	 if(!nocall)
	 {
		 int nres=MessageBox(L"确定废止连接吗？",L"断开数据库连接",MB_YESNO|MB_ICONQUESTION);
		 if(nres==IDYES)
		 {
			 this->db.disConnection();
			server=dbname=user=code=L"";
		 }
	 }
	 else
	 {
		 if(db.isConnection())
			 this->db.disConnection();
			server=dbname=user=code=L"";
	 }
 }
 void CMainFrame::showXy(double x,double y)
 {
	 CString cx,cy;
	 cx.Format(L"X: %.6f",x);
	 cy.Format(L"Y: %.6f",y);
	 m_wndStatusBar.SetPaneText(1,cx,TRUE);
	 m_wndStatusBar.SetPaneText(2,cy,TRUE);
 }
 void CMainFrame::showStatus(int s)
 {
	 CString str=L"当前状态：	";
	 switch(s)
	 {
	 case 0:str+=L"点编辑";break;
	 case 1:str+=L"线编辑";break;
	 case 2:str+=L"区编辑";break;
	 case 3:str+=L"注记编辑";break;
	 default:str+=L"普通/浏览";break;
	 }
	 m_wndStatusBar.SetPaneText(3,str,TRUE);
 }

 Layer* CMainFrame::GetEditingLayerPtr(void)
 {
	 return m_wndFileView.GetEditingPtr();
 }


 Basepic* CMainFrame::GetVisibleMapPtr(void)
 {
	 return m_wndFileView.GetViewpicPtr();
 }


 CFileView* CMainFrame::GetTree(void)
 {
	 return &m_wndFileView;
 }




 void CMainFrame::OnDeletetable()
 {
	 // TODO: 在此添加命令处理程序代码
	 if(!db.isConnection())
	 {
		 AfxMessageBox(L"数据库未连接");
	 }
	 else
	 {
		 CDeleteDlg g;
		 g.db=&db;
		 g.pView=&m_wndFileView;
		 if(IDOK==g.DoModal())
		 {
			 if(IDOK==MessageBox(L"确定要删除"+g.lname+L"吗",L"询问",MB_OKCANCEL|MB_ICONQUESTION))
			 db.DropTable(g.lname);
			 else
				 return ;
		 }
	 }
 }
