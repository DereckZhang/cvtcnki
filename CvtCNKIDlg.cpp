// CvtCNKIDlg.cpp : 实现文件
//
/*
* FileName: CvtCNKI.h
* Homepage: http://code.google.com/p/cvtcnki/
* License: GNU Public License
* CopyRight: (c)2005-2007 JiangZhanyong
* Author: JiangZhanyong
* email: Jiangzhanyong[at]gmail.com
* Website: http://jiangzhanyong.com
* Organization: Harbin Institute Of technology
* Data: 2008.02.24 19:30
* Version: 2.0.3.B2
* Description: See readme.txt
*/
#include "stdafx.h"
#include "CvtCNKI.h"
#include "CvtCNKIDlg.h"

#include <fstream>
#include <string>
#include <iosfwd>
#include <algorithm>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int	TXTFILE					= 1;	//	m_nReferenceFrom	从文件读取数据
const int	CLIPBOARD				= 2;	//	m_nReferenceFrom	从剪切板读取数据
const int	MAX_REFERENCENUMBER		= 500;	//	每次最多处理的题录个数；目前cnki限制为50个
const int	MAX_FORMATNUMBER		= 3;	//	最多能转换格式个数，目前为2，分别是 EndNote 和 RiS 和 BibTeX
const int	MAX_REFERENCETYPEVECTOR	= 9;	//	能处理的文献类型输出格式


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_sAutoInfo;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_sAutoInfo = "CvtCNKI是文献管理的辅助软件，可将中国期刊网，万方数据库，重庆维普数据库，中国互动网等站点的检索输出转换为 EndNote, RiS, BibTeX 等标准格式，以导入到 EndNote 等文献管理软件中。\r\n\r\nCvtCNKI 遵循 GNU 通用公共许可证(GPL) 开放源码，任何人均可自由使用。依照 GPL 对软件进行复制、修改和重新发布是允许的，但其作者(JiangZhanyong)不为衍生版本承担任何责任。\r\n\r\n项目网址: http://code.google.com/p/cvtcnki/ \r\n软件下载: http://code.google.com/p/cvtcnki/downloads/list \r\n作者网站: http://jiangzhanyong.com/\r\n";
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITINFO, m_sAutoInfo);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCvtCNKIDlg 对话框
CCvtCNKIDlg::CCvtCNKIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCvtCNKIDlg::IDD, pParent)
	, m_sPages(_T(""))
	, m_sDatabaseName(_T(""))
	, m_sAnnotation(_T(""))
	, m_sLang(_T(""))
{
	std::locale::global(std::locale(""));				//	以便正确处理中文路径问题
	//	记录程序所在路径
	char path[MAX_PATH] = {'\0'};
	GetModuleFileName(NULL, path, MAX_PATH);			//获得应用程序当前完整路径
	m_CurrentPath.Format("%s", path);
	int iPosition;
	iPosition = m_CurrentPath.ReverseFind('\\');
	m_CurrentPath = m_CurrentPath.Left(iPosition + 1);	//程序所在目录

	m_sFlagWord = "_flag";								//	标志词，表明这个是个关键词
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCvtCNKIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_PAGES, m_sPages);
	DDX_Text(pDX, IDC_DATABASENAME, m_sDatabaseName);
	DDX_Text(pDX, IDC_ANNOTATION, m_sAnnotation);
	DDX_Text(pDX, IDC_LANG, m_sLang);
}

BEGIN_MESSAGE_MAP(CCvtCNKIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP	
	ON_BN_CLICKED(IDC_FILE, &CCvtCNKIDlg::OnBnClickedFile)
	ON_BN_CLICKED(IDC_CLIPBOARD, &CCvtCNKIDlg::OnBnClickedMemory)
	ON_BN_CLICKED(IDC_CONVERT, &CCvtCNKIDlg::OnBnClickedConvert)
	ON_BN_CLICKED(IDC_SETOUTPUTDIR, &CCvtCNKIDlg::OnBnClickedSetoutputdir)
	ON_BN_CLICKED(IDC_OPENOUTPUTFILE, &CCvtCNKIDlg::OnBnClickedOpenoutputfile)
	ON_BN_CLICKED(IDC_ABOUT, &CCvtCNKIDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_POPUPMENU, &CCvtCNKIDlg::OnBnClickedPopupmenu)
	ON_COMMAND(ID_HELPME, &CCvtCNKIDlg::OnHelpme)
	ON_COMMAND(ID_LICENSE, &CCvtCNKIDlg::OnLicense)
	ON_COMMAND(ID_DBCFGUTF8, &CCvtCNKIDlg::OnDbcfgutf8)
	ON_COMMAND(ID_DBCFGOPENPLAN, &CCvtCNKIDlg::OnDbcfgOpenplan)
	ON_COMMAND(ID_PROJECTHOMEPAGE, &CCvtCNKIDlg::OnProjecthomepage)
	ON_COMMAND(ID_DOWNLOADHOMEPAGE, &CCvtCNKIDlg::OnDownloadhomepage)
	ON_COMMAND(ID_SERVICESHOMEPAGE, &CCvtCNKIDlg::OnServiceshomepage)
	ON_BN_CLICKED(ID_SPONSOR1, &CCvtCNKIDlg::OnBnClickedSponsor1)

	ON_BN_CLICKED(ID_SPONSOR2, &CCvtCNKIDlg::OnBnClickedSponsor2)
	ON_COMMAND(ID_NOTEEXPRESS_WEBSITE, &CCvtCNKIDlg::OnNoteexpressWebsite)
	ON_COMMAND(ID_NOTEEXPRESS_FORUM, &CCvtCNKIDlg::OnNoteexpressForum)
	ON_COMMAND(ID_NOTEEXPRESS_CMC, &CCvtCNKIDlg::OnNoteexpressCmc)
	ON_COMMAND(ID_NOTEEXPRESS_CTC, &CCvtCNKIDlg::OnNoteexpressCtc)
	ON_COMMAND(ID_NOTEEXPRESS_TUANGOU, &CCvtCNKIDlg::OnNoteexpressTuangou)
	ON_COMMAND(ID_PAANGOOD_WEBSITE, &CCvtCNKIDlg::OnPaangoodWebsite)
	ON_COMMAND(ID_PAANGOOD_FORUM, &CCvtCNKIDlg::OnPaangoodForum)
	ON_COMMAND(ID_PAANGOOD_WEBLOG, &CCvtCNKIDlg::OnPaangoodWeblog)
	ON_COMMAND(ID_PAANGOOD_AGENCY, &CCvtCNKIDlg::OnPaangoodAgency)
	ON_BN_CLICKED(IDC_WEBSITE, &CCvtCNKIDlg::OnBnClickedWebsite)
END_MESSAGE_MAP()


// CCvtCNKIDlg 消息处理程序

BOOL CCvtCNKIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 构造必要的map
	m_pmmap_ReferenceData		= new StrStrVectorMMap[MAX_REFERENCENUMBER];
	m_pmap_OutputStyle_Keyword	= new StrStrMMap[MAX_FORMATNUMBER];

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_SHOW);

	//	进行一些必要的初始化，并读取配置文件。
	SetFlagWord();							//	首先设置标志词	
	if ( !GetDataBaseKeyword() ){			//	从文件读取用户定义的数据库关键词
		return FALSE;
	}

	// 初始化　文献类型　的CListBox列表
	CListBox* pReferenceTypeListBox = static_cast<CListBox*>(GetDlgItem(IDC_REFERENCETYPE));
	
	StrVector::const_iterator citer = m_String_ReferenceTypeVector.begin();
	for( citer = m_String_ReferenceTypeVector.begin(); citer != m_String_ReferenceTypeVector.end(); ++citer )
	{	
		std::string str = *citer;
		pReferenceTypeListBox->AddString(CString(str.c_str()));

	}	
	pReferenceTypeListBox->AddString(_T("EndNote"));
	pReferenceTypeListBox->AddString(_T("RiS"));
	pReferenceTypeListBox->SetCurSel(0);

	// 初始化　输出格式　的CListBox列表
	CListBox* pOutPutListBox = static_cast<CListBox*>(GetDlgItem(IDC_OUTPUTSTYLE));
	pOutPutListBox->AddString(_T("EndNote"));
	pOutPutListBox->AddString(_T("RiS"));
	pOutPutListBox->AddString(_T("BibTeX"));
	pOutPutListBox->SetCurSel(0);


	AddToolTip();		// 添加提示工具条

	return TRUE;		// 除非将焦点设置到控件，否则返回 TRUE
}

//	添加小提示
void CCvtCNKIDlg::AddToolTip()
{
	EnableToolTips(TRUE);

	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.SetMaxTipWidth(150);

	//	检查是否要显示小提示
	StrStrMMap::const_iterator citer;
	std::string tooltip_string;
	for( citer = m_mmap_Option.begin(); citer != m_mmap_Option.end(); ++citer ){
		if ( citer->first == o01_tooltip){
			tooltip_string = citer->second;
			break;
		}
	}

	if (tooltip_string == "1"){					//	如果为1，则显示出小提示

		m_tooltip.AddTool(GetDlgItem(IDC_FILE), "从要转换的题录文件中读取数据");					// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_CLIPBOARD), "从要剪贴板中读取数据");						// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_CONVERT), "进行转换操作");									// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_OPENOUTPUTFILE), "若输出扩展名为txt，点击查看结果；若输出扩展名为标准格式，点击导入文献管理软件。输出文件扩展名可在配置文件指定。");	// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_REFERENCETYPE), "指定要转换的题录的文献类型；拉到最下方，也可以进行不同文献题录格式的转换");											// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_OUTPUTSTYLE), "指定输出的文献题录的格式");					// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_PAGES), "如果只转换一篇文献题录，可指定页码");				// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_DATABASENAME), "如果转换的文献题录出自同一数据库，可在此指定");	// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_ANNOTATION), "可在此添加注释");							// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_LANG), "可在此指定文献题录语言");							// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_SETOUTPUTDIR), "可在此指定转换后文献题录的输出目录");		// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_POPUPMENU), "点击打开菜单");								// 添加tip
		m_tooltip.AddTool(GetDlgItem(ID_SPONSOR1), "最新最全的文献管理工具软件资讯");			// 添加tip
		m_tooltip.AddTool(GetDlgItem(ID_SPONSOR2), "国内优秀虚拟主机，适合个人建站，点击打开菜单");	// 添加tip
		m_tooltip.AddTool(GetDlgItem(IDC_WEBSITE), "访问作者网站: http://jiangzhanyong.com/");	// 添加tip
	}
}

BOOL CCvtCNKIDlg::PreTranslateMessage(MSG* pMsg)
{
	if(m_tooltip.m_hWnd!=NULL){
		m_tooltip.RelayEvent(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCvtCNKIDlg::OnDestroy()
{
	delete [] m_pmmap_ReferenceData;
	delete [] m_pmap_OutputStyle_Keyword;
}

void CCvtCNKIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCvtCNKIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCvtCNKIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//从文件中读取数据
void CCvtCNKIDlg::OnBnClickedFile()
{
	m_nReferenceFrom = TXTFILE;
	CFileDialog dlg( TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("文本文件(*.txt)|*.txt|EndNote题录(*.enw)|*.enw|Ris题录(*.ris)|*.ris|所有文件(*.*)|*.*||") );

	if ( IDOK == dlg.DoModal() ) {		
		m_PathName		= dlg.GetPathName();						//	获取要转换的文件的路径
		int iPosition	= m_PathName.ReverseFind('\\');				//	将程序文件名去掉
		m_PathName		= m_PathName.Left(iPosition+1);				//	程序所在目录，包括最后一个反斜杠
		m_FileName		= dlg.GetFileName();						//	获取要转换的文件的文件名（包括前缀和后缀）
		m_FileTitle		= dlg.GetFileTitle();						//	获取要转换的文件的文件名
	}
}

//从剪贴板中读取数据
void CCvtCNKIDlg::OnBnClickedMemory()
{
	m_nReferenceFrom = CLIPBOARD;

	if ( !IsClipboardFormatAvailable(CF_TEXT) ) {
		MessageBox("剪切板内容不符合要求。", "错误", MB_ICONERROR);
		return;
	}

	if ( !OpenClipboard() ){							//	打开剪切板
		MessageBox("读取剪切板错误。", "错误", MB_ICONERROR);
		return;	
	}

	CreateTempFile();

	m_PathName		= m_CurrentPath;					//	包括最后一个反斜杠
	m_FileName		= "tempRef.txt";					//	设置临时文件的文件名，扩展名等，以用于输出
	m_FileTitle		= "tempRef";
}

//如果从剪贴板读取数据，需要创建中间文件
void CCvtCNKIDlg::CreateTempFile()
{
	std::ofstream ofile("tempRef.txt");

	HANDLE hClipMemory = ::GetClipboardData(CF_TEXT);		// 获取剪贴板数据句柄
//	DWORD dwLength = GlobalSize(hClipMemory);				// 返回指定内存区域的当前大小
	LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory);	// 锁定内存
	CString sMessage = CString(lpClipMemory);				// 保存得到的文本数据
	ofile << sMessage; 
	GlobalUnlock(hClipMemory);								// 内存解锁
	::CloseClipboard();										// 关闭剪贴板
}

//进行转换操作
void CCvtCNKIDlg::OnBnClickedConvert()
{
	if ( m_FileName == "" ) {							//	防止未打开文件名，直接进行转换。			
		MessageBox("文件名为空。", "错误", MB_ICONERROR);
		return;
	}

	std::ifstream infile( m_FileName );					//	读取文件到 infile	
	if( !infile ) {										//	如果文件打不开，则退出
		MessageBox("无法打开源题录文件。", "错误", MB_ICONERROR);
		return;
	}

	if ( !InitializeSetting() ){
		return;
	}

	if ( !ReadData() ){
		MessageBox("读取题录文件出错。", "错误", MB_ICONERROR);
		return;
	}

	if ( !WriteData() ){
		MessageBox("写入到输出文件出错。", "错误", MB_ICONERROR);
		return;
	}

	return;	
}

//	打开输出结果
void CCvtCNKIDlg::OnBnClickedOpenoutputfile()
{
	ShellExecute(NULL, "open", m_OutputFileName, NULL, NULL, SW_SHOW);
}

//	关于对话框
void CCvtCNKIDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	if ( IDOK == dlg.DoModal() ) {		
	};
}

//	弹出菜单
void CCvtCNKIDlg::OnBnClickedPopupmenu()
{
	CMenu menu;
	menu.LoadMenu(IDR_POPUPMENU);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT_VALID(pPopup);

	CRect rc;
	CButton* pPopupMenuButton = static_cast<CButton*>(GetDlgItem(IDC_POPUPMENU));	
	pPopupMenuButton->GetWindowRect(&rc);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, (rc.right+rc.left)/2, (rc.bottom+rc.top)/2, this, NULL);
}

void CCvtCNKIDlg::OnHelpme()
{
	ShellExecute(NULL, "open", m_CurrentPath+"Readme.pdf", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnLicense()
{
	ShellExecute(NULL, "open", m_CurrentPath+"license.txt", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnDbcfgutf8()
{
	ShellExecute(NULL, "open", m_CurrentPath+"CvtCNKI.ini", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnDbcfgOpenplan()
{
	ShellExecute(NULL, "open", "http://jiangzhanyong.com/projects/cvtcnki/open-keyword-plan", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnProjecthomepage()
{
	ShellExecute(NULL, "open", "http://code.google.com/p/cvtcnki/", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnDownloadhomepage()
{
	ShellExecute(NULL, "open", "http://code.google.com/p/cvtcnki/downloads/list", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnServiceshomepage()
{
	ShellExecute(NULL, "open", "http://jiangzhanyong.com/projects/cvtcnki", NULL, NULL, SW_SHOW);
}


void CCvtCNKIDlg::OnBnClickedSponsor1()
{
/*	CMenu menu;
	menu.LoadMenu(IDR_SPONORMENU1);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT_VALID(pPopup);

	CRect rc;
	CButton* pPopupMenuButton = static_cast<CButton*>(GetDlgItem(ID_SPONSOR1));	
	pPopupMenuButton->GetWindowRect(&rc);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, (rc.right+rc.left)/2, (rc.bottom+rc.top)/2, this, NULL);
*/
	ShellExecute(NULL, "open", "http://www.endnote.cn/", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnNoteexpressWebsite()
{
	ShellExecute(NULL, "open", "http://www.scinote.com/index_chs.htm", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnNoteexpressForum()
{
	ShellExecute(NULL, "open", "http://www.scinote.com/supportcn/cgi-bin/forum_show.cgi", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnNoteexpressCmc()
{
	ShellExecute(NULL, "open", "http://noteexpress.jiangzhanyong.com/index_chs.htm", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnNoteexpressCtc()
{
	ShellExecute(NULL, "open", "http://scinote.jiangzhanyong.com/index_chs.htm", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnNoteexpressTuangou()
{
	ShellExecute(NULL, "open", "http://jiangzhanyong.com/service/noteexpress/", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnBnClickedSponsor2()
{
	CMenu menu;
	menu.LoadMenu(IDR_SPONORMENU2);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT_VALID(pPopup);

	CRect rc;
	CButton* pPopupMenuButton = static_cast<CButton*>(GetDlgItem(ID_SPONSOR2));	
	pPopupMenuButton->GetWindowRect(&rc);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, (rc.right+rc.left)/2, (rc.bottom+rc.top)/2, this, NULL);
}

void CCvtCNKIDlg::OnPaangoodWebsite()
{
	ShellExecute(NULL, "open", "http://www.paangood.com/", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnPaangoodForum()
{
	ShellExecute(NULL, "open", "http://www.paangood.com/forum/", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnPaangoodWeblog()
{
	ShellExecute(NULL, "open", "http://www.paangoodblog.com/", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnPaangoodAgency()
{
	ShellExecute(NULL, "open", "http://jiangzhanyong.com/service/paangood/", NULL, NULL, SW_SHOW);
}

void CCvtCNKIDlg::OnBnClickedWebsite()
{
	ShellExecute(NULL, "open", "http://jiangzhanyong.com/", NULL, NULL, SW_SHOW);
}

//	初始化设置
BOOL CCvtCNKIDlg::InitializeSetting()
{
	if ( !GetDataBaseKeyword() ){			//	从文件读取用户定义的数据库关键词
		return FALSE;
	}

	if ( !SetOutputStyleKeyword() ){		//	设置输出的格式 RiS - EndNote - BibTeX
		MessageBox("设置格式出错。", "错误", MB_ICONERROR);
		return FALSE;
	}

	SetCurrentMap();						//	设置当前要使用的MAP
	SetItemState();							//	设置每个item是否需要处理

	return TRUE;
}


//	从文件中读取数据库题录格式
BOOL CCvtCNKIDlg::GetDataBaseKeyword()
{	
	ClearData();							//	清空各项数据

	//	首先获取数据库题录格式文件所在位置
	CString CurrentPath;
	char path[MAX_PATH] = {'\0'};
	GetModuleFileName(NULL, path, MAX_PATH);			//获得应用程序当前完整路径
	CurrentPath.Format("%s", path);
	int iPosition = CurrentPath.ReverseFind('\\');
	CurrentPath = CurrentPath.Left(iPosition + 1);		//程序所在目录

	CString CfgFileName = CurrentPath + "CvtCNKI.ini";

	std::ifstream infile( CfgFileName );				//	读取文件到 infile	
	if( !infile ) {										//	如果文件打不开，则退出
		MessageBox("无法打开数据库配置文件。", "错误", MB_ICONERROR);
		return FALSE;
	}

	std::string	contextWord;							//	两个字符串，分别保留当前和之前读取的数据

	while ( std::getline(infile, contextWord, '\n') ){
		ExtractDataBaseKeyword(contextWord);
	}

	return TRUE;
}

//	从读到的数据中提取关键词
void CCvtCNKIDlg::ExtractDataBaseKeyword(std::string thestring)
{
	std::string	contextWordNoAnnotation;							//	去除掉注释部分后的字符串
	std::string annotationFlag = ";";								//	注释行标志

	DeleteBlankStringofBeginAndEnd(thestring);						//	删除开始和末尾的空格
	std::string::size_type iPos = thestring.find_first_of(annotationFlag);				//	查找注释行标志

	if( iPos == std::string::npos )	{								//	如果没有找到注释标志，直接处理
		contextWordNoAnnotation = thestring;
	}
	else{															//	找到注释标志，去掉注释部分
		contextWordNoAnnotation = thestring.substr(0, iPos);
		DeleteBlankStringofBeginAndEnd(contextWordNoAnnotation);	//	删除开始和末尾的空格
		if ( contextWordNoAnnotation.empty() )	{ return;	}
	}

	iPos = contextWordNoAnnotation.find(m_sFlagWord);				//	查找标示词
	if (iPos != 0 && iPos != std::string::npos){
		std::string key = contextWordNoAnnotation.substr(0, iPos+m_sFlagWord.size());
		std::string tempstring = contextWordNoAnnotation.substr(key.size());
		DeleteBlankStringofBeginAndEnd(tempstring);
		std::string value = tempstring.substr(1, tempstring.size()-2);		//	去掉首尾的{}号
		std::string first_string = key.substr(0, 1);
		if ( first_string == "n" )			//	标示词
		{
			m_mmap_DataBase_Keyword.insert( make_pair(key, value) );
			m_mmap_DataBase_Keyword.insert( make_pair(key, ConvertUtf8ToGBK(value)) );
		} 
		else if ( first_string == "o" )		//	选项
		{
			m_mmap_Option.insert( make_pair(key, value) );
			m_mmap_Option.insert( make_pair(key, ConvertUtf8ToGBK(value)) );
		}
		else if ( first_string == "s" )		//	文献类型对应的字符串
		{
			m_String_ReferenceTypeVector.push_back(ConvertUtf8ToGBK(value));
		} 
		else if ( first_string == "e" )		//	EndNote文献类型
		{
			m_EndNote_ReferenceTypeVector.push_back(ConvertUtf8ToGBK(value));
		}
		else if ( first_string == "r" )		//	Ris文献类型
		{
			m_Ris_ReferenceTypeVector.push_back(ConvertUtf8ToGBK(value));
		} 
		else if ( first_string == "b" )		//	BibTeX文献类型
		{
			m_BibTeX_ReferenceTypeVector.push_back(ConvertUtf8ToGBK(value));
		}
	}
}

//	从UTF8到gbk进行编码转换
string CCvtCNKIDlg::ConvertUtf8ToGBK(std::string thestrUtf8) {
	CString strUtf8(thestrUtf8.c_str());
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL, 0);
	wchar_t * wszGBK = new wchar_t[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

	std::string strGBK(szGBK);
	delete[] szGBK;
	delete[] wszGBK;
	return strGBK;
}

//	清空各项数据
void CCvtCNKIDlg::ClearData()
{
	int i;
	for(i = 0; i < MAX_REFERENCENUMBER; ++i){
		m_pmmap_ReferenceData[i].clear();				//	清空之前每个题录得数据
	}

	for(i = 0; i < MAX_FORMATNUMBER; ++i) {
		m_pmap_OutputStyle_Keyword[i].clear();			//	清空输出格式的map
	}

	m_mmap_DataBase_Keyword.clear();					//	清空数据库关键词
	m_mmap_Option.clear();								//	清空选项配置
	m_String_ReferenceTypeVector.clear();				//	清空文献类型对应的字符串
	m_EndNote_ReferenceTypeVector.clear();				//	清空EndNote文献类型
	m_Ris_ReferenceTypeVector.clear();					//	清空Ris文献类型
	m_BibTeX_ReferenceTypeVector.clear();				//	清空BibTeX 文献类型
	m_mmap_Curent_OutputStyle_Keyword.clear();			//	清空实际采用的输出map
	m_Source_ReferenceTypeVector.clear();				//	清空转换前的的文献格式表示列表
	m_Current_ReferenceTypeVector.clear();				//	清空实际采用的的文献格式表示列表
	m_map_Item_State.clear();							//	清空各项是否需要分割的map

	m_sFirstLibraryWord		= "";						//	计数的一个标志
}

//	删除其中无效的字符，并进行分词
void CCvtCNKIDlg::DeleteInvalidString(string& thestring, std::vector<std::string>& vResultString, string filter)
{
	std::string  tempstring;
	std::string::size_type pos1 = 0;
	std::string::size_type pos2 = 0;

	for ( ; pos2 != thestring.size(); )
	{
		pos2 = thestring.find_first_of(filter);
		if ( pos2 == 0 ){
			thestring.erase(pos2, 1);
		}
		else{
			tempstring.assign(thestring, pos1, pos2);
			vResultString.push_back(tempstring);
			thestring.erase(pos1, pos2);
		}

		if ( thestring.empty() ) {
			return;
		}else {
			pos2 = thestring.find_first_of(filter);
		}
	}
}


//	删除字符串开头和结尾位置的空格~
void CCvtCNKIDlg::DeleteBlankStringofBeginAndEnd(string& thestring)
{
	if (thestring.empty()){		return;		}

	int offset = 1;
	std::string::size_type posbegin = 0;
	std::string::size_type posend = (thestring.size()-1);
	std::string filterofblank = "\t ";
	std::string filterofnewline = "\n";

	for(; posbegin == 0 || (posend <= (thestring.size()-1) && posend >= (thestring.size()-1-offset)); ){
		posbegin = thestring.find_first_of(filterofblank);		
		if (posbegin == 0){
			thestring.erase(posbegin, 1);
		}
		if ( thestring.empty() ){	return;	}

		posend = thestring.find_last_of(filterofblank);
		if (posend <= (thestring.size()-1) && posend >= (thestring.size()-1-offset)){
			thestring.erase(posend, 2);
			posend = thestring.find_last_of(filterofblank);
		}

		if ( thestring.empty() || ((posbegin != 0) && (posend != (thestring.size()-1)) ) ){
			return;
		}
	}
}

//	删除字符串开头的数字
void CCvtCNKIDlg::DeleteNumberStringofBegin(string& thestring, string filter)
{
	if (thestring.empty()){		return;		}

	std::string::size_type posbegin = thestring.find_first_of(filter);		

	for(; posbegin == 0; ){
		thestring.erase(posbegin, 1);

		if ( thestring.empty() ){	return;	}
		else{
			posbegin = thestring.find_first_of(filter);	
		}
	}
}

//	删除字符串中的的数字
void CCvtCNKIDlg::DeleteNumberString(string& thestring, string filter)
{
	if (thestring.empty()){		return;		}

	std::string::size_type posbegin = thestring.find_first_of(filter);		

	for(; posbegin <= thestring.size(); ){
		thestring.erase(posbegin, 1);

		if ( thestring.empty() ){	return;	}
		else{
			posbegin = thestring.find_first_of(filter);	
		}
	}
}

//	检查字符串中是否包含英语字符
BOOL CCvtCNKIDlg::IsEnglishVersion(std::string thestring)
{
	std::string EngChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string PunctuationChar = ".";
	if ( (thestring.find_first_of(EngChar) != std::string::npos) && 
		(thestring.find_first_of(PunctuationChar) != std::string::npos)	){
		return TRUE;
	}
	return FALSE;
}


BOOL CCvtCNKIDlg::SetCurrentMap()
{	
	//	进行数据交换
	UpdateData(TRUE);

	//	确定输出目录	
	std::string UserSettingPath(m_UserSettingPath.GetBuffer());
	if( UserSettingPath.empty() ){
		m_FileTitle = m_PathName + m_FileTitle;
	}
	else{
		m_FileTitle = m_UserSettingPath + m_FileTitle;
	}

	//	确定输出Ris 还是 EndNote 还是 BibTeX
	CListBox* pOutPutListBox = static_cast<CListBox*>(GetDlgItem(IDC_OUTPUTSTYLE));
	m_iPosOutputStyle = pOutPutListBox->GetCurSel();
	ASSERT( m_iPosOutputStyle < MAX_FORMATNUMBER );

	m_mmap_Curent_OutputStyle_Keyword = m_pmap_OutputStyle_Keyword[m_iPosOutputStyle];
	if ( m_iPosOutputStyle == 0 ){		
		m_Current_ReferenceTypeVector = m_EndNote_ReferenceTypeVector;
		m_OutputFileName = m_FileTitle + ".enw";
	}
	else if ( m_iPosOutputStyle == 1 ){
		m_Current_ReferenceTypeVector = m_Ris_ReferenceTypeVector;
		m_OutputFileName = m_FileTitle + ".ris";
	}
	else if ( m_iPosOutputStyle == 2 ){
		m_Current_ReferenceTypeVector = m_BibTeX_ReferenceTypeVector;
		m_OutputFileName = m_FileTitle + ".bib";
	}

	StrStrMMap::const_iterator citer_option;
	for( citer_option = m_mmap_Option.begin(); citer_option != m_mmap_Option.end(); ++citer_option ){
		if ( citer_option->first == o05_outputfile_ext && citer_option->second != "0"){
			m_OutputFileName = m_OutputFileName + ".txt";
		}
		if ( citer_option->first == o06_abstract_split ){
			if ( citer_option->second == "0" ){
				m_nMaxInforChar = 0;
			} else {
				m_nMaxInforChar = atoi((citer_option->second).c_str());
				ASSERT( m_nMaxInforChar > 0 );
			}
		}
	}


	//	再确定输出的文献类型
	CListBox* pReferenceTypeListBox = static_cast<CListBox*>(GetDlgItem(IDC_REFERENCETYPE));
	std::vector<std::string>::const_iterator citer;
	m_iReferenceType = pReferenceTypeListBox->GetCurSel();
	int i = 0;
	for( citer = m_Current_ReferenceTypeVector.begin();
		 citer != m_Current_ReferenceTypeVector.end(); 
		 ++citer, ++i )
	{
		if( m_iReferenceType == i && (m_iReferenceType < m_String_ReferenceTypeVector.size()) ){
			m_Current_ReferenceType = (*citer);
		}
	}

	//	如果是进行 EndNote Ris，需要用下面代码指定
	if ( m_iReferenceType == m_String_ReferenceTypeVector.size() ){
		m_Source_ReferenceTypeVector = m_EndNote_ReferenceTypeVector;
	}
	else if( (m_iReferenceType == m_String_ReferenceTypeVector.size()+1) )	{
		m_Source_ReferenceTypeVector = m_Ris_ReferenceTypeVector;
	}


	return TRUE;
}

//	读取数据
BOOL CCvtCNKIDlg::ReadData()
{
	m_nReferenceNumber = -1;
	std::ifstream infile( m_FileName );			//	读取文件到 infile	
	std::string	contextWord;					//	两个字符串，分别保留当前和之前读取的数据

	while ( std::getline(infile, contextWord, '\n') ){
		if ( contextWord != "" ){
			ProcessData(contextWord);
		}		
	}

	return TRUE;
}

//	将结果输出到文件
BOOL CCvtCNKIDlg::WriteData()
{
	std::string	fmt_space, fmt_endofrefer, fmt_refertype;
	fmt_space		= m_mmap_Curent_OutputStyle_Keyword.find(m_space)->second;
	fmt_endofrefer	= m_mmap_Curent_OutputStyle_Keyword.find(m_endofrefer)->second;
	fmt_refertype	= m_mmap_Curent_OutputStyle_Keyword.find(n00_refertype)->second;

	std::ofstream outfile(m_OutputFileName);		//	输出到 outfile 文件，文件名 result.txt

	if ( !outfile ) {								//	如果无法创建文件，则退出		
		MessageBox("创建输出文件失败。", "错误", MB_ICONERROR);
		return FALSE;
	}

	++m_nReferenceNumber;
	for (int i = 0; i < m_nReferenceNumber; ++i)
	{
		StrStrVectorMMap::iterator iter_referdata;
		StrStrMMap::iterator iter_format;
		StrVector::iterator iter_itemstring;

		//	首行输出文献类型
		if ( m_iReferenceType < m_String_ReferenceTypeVector.size() )		//	从列表中选择文献类型
		{
			if( m_iPosOutputStyle == 2 ){		//	针对 BibTeX 作处理
				outfile << "@" << fmt_refertype << m_Current_ReferenceType << "{," << std::endl;
			}
			else{
				outfile << fmt_refertype << fmt_space << m_Current_ReferenceType << std::endl;
			}
		}
		else{													//	进行文献格式转换时输出文献类型
			StrStrVectorMMap::iterator iter_referdata_type = m_pmmap_ReferenceData[i].find(n00_refertype);
			std::string	tempstring_refertype = iter_referdata_type->second.front();
			std::string bibtex_refertype;

			// 以下是通用类型的转换，需要特殊处理
			if( tempstring_refertype == "Generic" 
				&& (m_Source_ReferenceTypeVector == m_EndNote_ReferenceTypeVector) )	{		//	从 EndNote 转换
				if( m_Current_ReferenceTypeVector == m_Ris_ReferenceTypeVector ){				//	从 EndNote 到 Ris
					iter_referdata_type->second.pop_back();
					iter_referdata_type->second.push_back("GEN");
				}
				else if( m_Current_ReferenceTypeVector == m_BibTeX_ReferenceTypeVector ){		//	从 EndNote 到 BibTex
					iter_referdata_type->second.pop_back();
					bibtex_refertype = "misc";
				}
			}
			else if( tempstring_refertype == "GEN"												//	从 RiS 转换
				&& (m_Source_ReferenceTypeVector == m_Ris_ReferenceTypeVector) ){		
				if( m_Current_ReferenceTypeVector == m_EndNote_ReferenceTypeVector ){			//	从 Ris 到 EndNote
					iter_referdata_type->second.pop_back();
					iter_referdata_type->second.push_back("Generic");
				}
				else if( m_Current_ReferenceTypeVector == m_BibTeX_ReferenceTypeVector ){		//	从 Ris 到 BibTex
					iter_referdata_type->second.pop_back();
					bibtex_refertype = "misc";
				}
			}
			else{
				StrVector::const_iterator citer_refertype_source, citer_refertype_current;
				for( citer_refertype_source = m_Source_ReferenceTypeVector.begin(), 
					 citer_refertype_current = m_Current_ReferenceTypeVector.begin(); 
					 citer_refertype_source != m_Source_ReferenceTypeVector.end(),
					 citer_refertype_current != m_Current_ReferenceTypeVector.end(); 
				     ++citer_refertype_source, ++citer_refertype_current ){
						if( tempstring_refertype == (*citer_refertype_source) ){
							iter_referdata_type->second.pop_back();
							if( m_Current_ReferenceTypeVector != m_BibTeX_ReferenceTypeVector ){
								iter_referdata_type->second.push_back(*citer_refertype_current);
							}
							else{
								bibtex_refertype = (*citer_refertype_current);
							}
						}
						break;
				}
			}

			if( m_iPosOutputStyle == 2 ){		//	针对 BibTeX 作处理
				outfile << "@" << fmt_refertype << bibtex_refertype << "{," << std::endl;
			}
		}

		//	遍历输出所有的题录
		for( iter_referdata = m_pmmap_ReferenceData[i].begin(); 
			 iter_referdata != m_pmmap_ReferenceData[i].end();
			 ++iter_referdata )	{

			//	遍历题录的每一项，进行输出
			for( iter_format = m_mmap_Curent_OutputStyle_Keyword.begin(); 
				 iter_format != m_mmap_Curent_OutputStyle_Keyword.end();
				 ++iter_format ){
				if ( (iter_referdata->first) == (iter_format->first) )
				{
					if( m_iPosOutputStyle == 2 ){	//	针对 BibTeX 输出格式，做特殊处理
						size_t iTotalNumber = iter_referdata->second.size();
						int index = 1;
						for( iter_itemstring = iter_referdata->second.begin();
							iter_itemstring != iter_referdata->second.end();
							++iter_itemstring, ++index ){
								if (index == 1)	{			//	index == 1，说明第一次，要输出前面的部分
									if ( iter_format->first == n41_publisher ){
										//	对于这一项作一下特别处理，调整一下，其他需要调整的，也是在这里进行
										if ( (m_Current_ReferenceType == "phdthesis") || (m_Current_ReferenceType == "masterthesis") ) {
											outfile << "school";
										}
										else if( m_Current_ReferenceType == "techreport" ){
											outfile << "insitution";
										}
									}
									else if(iter_format->first == n30_volume){
										//	学位论文级别，对应的卷项为type
										if ( (m_Current_ReferenceType == "phdthesis") || (m_Current_ReferenceType == "masterthesis") ) {
											outfile << "type";
										}
									}
									else{
										outfile << iter_format->second;
									}

									outfile << fmt_space << "{";
									outfile << (*iter_itemstring);
								}
								else{					//	index != 1	要输出中间部分
									outfile << " and " << (*iter_itemstring) ;	
								}

								if ( index == iTotalNumber ){	//	index == iTotalNumber，输出要结束，输出结束部分
									outfile << "}," << std::endl;
									index = 1;
								}
						}
					}
					else{
						for( iter_itemstring = iter_referdata->second.begin();
							 iter_itemstring != iter_referdata->second.end();
							 ++iter_itemstring ){
							outfile << iter_format->second << fmt_space << (*iter_itemstring) << std::endl;
						}
					}
					break;
				}
			}
		}

		//	输出用户自定义项 
		if( m_iPosOutputStyle == 2 ){
			std::string sPages(m_sPages.GetBuffer());
			if( !sPages.empty() ){
				outfile << m_mmap_Curent_OutputStyle_Keyword.find(n09_totalpages)->second << fmt_space << "{" << sPages << "}," << std::endl;
			}
			std::string sDatabaseName(m_sDatabaseName.GetBuffer());
			if( !sDatabaseName.empty() ){
				outfile << m_mmap_Curent_OutputStyle_Keyword.find(n26_database)->second << fmt_space << "{" << sDatabaseName << "}," << std::endl;
			}
			std::string sAnnotation(m_sAnnotation.GetBuffer());
			if( !sAnnotation.empty() ){
				outfile << m_mmap_Curent_OutputStyle_Keyword.find(n20_note)->second << fmt_space << "{" << sAnnotation << "}," << std::endl;
			}
			std::string sLang(m_sLang.GetBuffer());
			if( !sLang.empty() ){
				outfile << m_mmap_Curent_OutputStyle_Keyword.find(n28_language)->second << fmt_space << "{" << sLang << "}," << std::endl;
			}

			//	输出结束标志
			outfile << "}" << "\n\n" << std::endl;	
		}
		else{
			std::string sPages(m_sPages.GetBuffer());
			if( !sPages.empty() ){
				outfile << m_mmap_Curent_OutputStyle_Keyword.find(n09_totalpages)->second << fmt_space << sPages << std::endl;
			}
			std::string sDatabaseName(m_sDatabaseName.GetBuffer());
			if( !sDatabaseName.empty() ){
				outfile << m_mmap_Curent_OutputStyle_Keyword.find(n26_database)->second << fmt_space << sDatabaseName << std::endl;
			}
			std::string sAnnotation(m_sAnnotation.GetBuffer());
			if( !sAnnotation.empty() ){
				outfile << m_mmap_Curent_OutputStyle_Keyword.find(n20_note)->second << fmt_space << sAnnotation << std::endl;
			}
			std::string sLang(m_sLang.GetBuffer());
			if( !sLang.empty() ){
				outfile << m_mmap_Curent_OutputStyle_Keyword.find(n28_language)->second << fmt_space << sLang << std::endl;
			}

			//	输出结束标志
			outfile << fmt_endofrefer << fmt_space << "\n\n" << std::endl;			
		}
	}

	return TRUE;
}

//	处理读取得到的每一行数据
void CCvtCNKIDlg::ProcessData(string& thestring)
{	
	int temptemptemtp = 0;
	DeleteBlankStringofBeginAndEnd(thestring);							//	删除开始和末尾的空格
	DeleteNumberStringofBegin(thestring, "1234567890.,-()[]/ ");		//	删除开始的数字

	if ( thestring.size() == 0 ){	return;	}							//	如果 thestring 为0，则不必处理

	std::string::size_type pos = 999;	
	std::string::size_type ExactPos = std::string::npos;				//	记录一个精确的位置
	std::string::size_type validindex = 6;								//	有效索引开始值的最大限制
	std::string::size_type targetwordsize = 0;							//	下面要比较的关键词的大小
	StrStrMMap::const_iterator citer;
	StrStrMMap::const_iterator Exactciter;								//	记录此时的位置
	std::vector<std::string> vStringKeyword;
	std::string tempstring;

	//	首先找到最匹配的位置
	for( citer = m_mmap_DataBase_Keyword.begin(); citer != m_mmap_DataBase_Keyword.end(); ++citer )
	{
		pos = thestring.find(citer->second);
		if( (targetwordsize < citer->second.size()) && (pos < ExactPos || pos == 0) ){	
			ExactPos = pos;
			Exactciter = citer;	
			tempstring = thestring;
			targetwordsize = citer->second.size();
			DeleteInvalidString(tempstring, vStringKeyword);
			tempstring = vStringKeyword.front();						//	这一行貌似没用，tempstring 下面并没有用到，2007.05.31
		} 
	}

	//	匹配成功后，检查有没有需要删除的字符串
	std::string::size_type deletewordpos;
	for( citer = m_mmap_Option.begin(); citer != m_mmap_Option.end(); ++citer ){
		deletewordpos = thestring.find(citer->second);
		if ( (deletewordpos != std::string::npos) && (citer->first == o04_delete_word) ){
			std::string strbegin = thestring.substr(0, deletewordpos);	//	提取有效的字符串
			std::string strend = thestring.substr(deletewordpos+1);	
			thestring = strbegin + strend;
		}
	}

	//	记录下最匹配的位置后，开始进行数据处理
	if (ExactPos != std::string::npos && ExactPos < validindex){
		if ( m_sFirstLibraryWord == "" ){
			m_sFirstLibraryWord = Exactciter->second;
			++m_nReferenceNumber;
		}
		else if( m_sFirstLibraryWord == Exactciter->second )	{
			++m_nReferenceNumber;
		}

		tempstring = thestring.substr(ExactPos+Exactciter->second.size());	//	提取有效的字符串
		DeleteBlankStringofBeginAndEnd(tempstring);							//	删除得到的字符串的首尾空白
		if( n03_author == Exactciter->first ){
			DeleteNumberString(tempstring, "1234567890-()[]/");
		}

		if( !tempstring.empty() ){
			std::vector<std::string> vString;
			if ( m_iReferenceType < m_String_ReferenceTypeVector.size() ){					//	正常的格式转换
				if ( (Exactciter->first == n30_volume) && (m_Current_ReferenceType == "phdthesis") ){		//	如果是学位论文，需要判断是硕士论文还是博士论文
					std::string::size_type thesispos;
					for( citer = m_mmap_Option.begin(); citer != m_mmap_Option.end(); ++citer ){
						thesispos = tempstring.find(citer->second);
						if ( thesispos != std::string::npos ){
							if ( citer->first == o02_doctor_thesis ){
								tempstring = "博士学位论文";
								break;
							}
							else if( citer->first == o03_master_thesis ){
								tempstring = "硕士学位论文";
								break;
							}
						}
					}
				}

				if ( m_map_Item_State.find(Exactciter->first)->second && !IsEnglishVersion(tempstring) ){		//	判断字符串是否需要分成多项，比如关键词，作者
					DeleteInvalidString(tempstring, vString);
				}
				else {
					if ( Exactciter->first == n19_abstract && m_nMaxInforChar != 0 && m_nMaxInforChar < tempstring.size()){
						vString.push_back(tempstring.substr(0, m_nMaxInforChar));
					}else{
						vString.push_back(tempstring);
					}			
				}

				m_pmmap_ReferenceData[m_nReferenceNumber].insert( make_pair(Exactciter->first, vString) );	
			}
			else{		//	m_iReferenceType >= m_String_ReferenceTypeVector.size()			//	EndNote Ris 互换，或者从 EndNote Ris 转换到 BibTeX
				DeleteNumberStringofBegin(tempstring, ".,-()[] ");		//	这一步主要去掉 - 符号
				StrStrVectorMMap::const_iterator citer_str_strVector = m_pmmap_ReferenceData[m_nReferenceNumber].find(Exactciter->first);
				if( citer_str_strVector == m_pmmap_ReferenceData[m_nReferenceNumber].end() ){
					if ( m_map_Item_State.find(Exactciter->first)->second && !IsEnglishVersion(tempstring) ){		//	判断字符串是否需要分成多项，比如关键词，作者
						DeleteInvalidString(tempstring, vString);
					}
					else {
						vString.push_back(tempstring);
					}
					m_pmmap_ReferenceData[m_nReferenceNumber].insert( make_pair(Exactciter->first, vString) );
				}
				else{
					m_pmmap_ReferenceData[m_nReferenceNumber].find(Exactciter->first)->second.push_back(tempstring);	
				}
			}
		}
	}
}

//	用户选择输出目录
void CCvtCNKIDlg::OnBnClickedSetoutputdir()
{	
	char* path = ChooseDirectory("设定 CvtCNKI 转换结果输出位置");
	m_UserSettingPath.Format("%s", path);
	m_UserSettingPath = m_UserSettingPath + "\\";				//	加上最后一个
}

char* CCvtCNKIDlg::ChooseDirectory(char *szTitle)
{
	BROWSEINFO  bi;
	ITEMIDLIST  *pItemIDList;
	static char szPath[MAX_PATH];

	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = szTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	pItemIDList = SHBrowseForFolder(&bi);

	if(! SHGetPathFromIDList(pItemIDList, szPath) )
		return NULL;

	return (char*)szPath;
}

//	设置每一项是否需要分割处理的状态, true为需要处理，fase为不需要处理
BOOL CCvtCNKIDlg::SetItemState()
{
	m_map_Item_State.insert( make_pair(n00_refertype,			0) );
	m_map_Item_State.insert( make_pair(n02_title,				0) );
	m_map_Item_State.insert( make_pair(n03_author,				1) );	// 1
	m_map_Item_State.insert( make_pair(n04_year,				0) );
	m_map_Item_State.insert( make_pair(n06_placepublished,		0) );
	m_map_Item_State.insert( make_pair(n09_totalpages,			0) );
	m_map_Item_State.insert( make_pair(n11_date,				0) );
	m_map_Item_State.insert( make_pair(n12_revieweditem,		0) );
	m_map_Item_State.insert( make_pair(n13_shorttitle,			0) );
	m_map_Item_State.insert( make_pair(n14_resourcenum,			0) );
	m_map_Item_State.insert( make_pair(n15_call,				0) );
	m_map_Item_State.insert( make_pair(n16_accessnumber,		0) );
	m_map_Item_State.insert( make_pair(n17_label,				1) );	// 1
	m_map_Item_State.insert( make_pair(n18_keyword,				1) );	// 1
	m_map_Item_State.insert( make_pair(n19_abstract,			0) );
	m_map_Item_State.insert( make_pair(n20_note,				0) );
	m_map_Item_State.insert( make_pair(n21_researchnote,		0) );
	m_map_Item_State.insert( make_pair(n22_url,					0) );
	m_map_Item_State.insert( make_pair(n23_address,				0) );
	m_map_Item_State.insert( make_pair(n24_caption,				0) );
	m_map_Item_State.insert( make_pair(n25_accessdate,			0) );
	m_map_Item_State.insert( make_pair(n26_database,			0) );
	m_map_Item_State.insert( make_pair(n27_dbprovider,			0) );
	m_map_Item_State.insert( make_pair(n28_language,			0) );
	m_map_Item_State.insert( make_pair(n29_secondarytitle,		0) );
	m_map_Item_State.insert( make_pair(n30_volume,				0) );
	m_map_Item_State.insert( make_pair(n31_number,				0) );
	m_map_Item_State.insert( make_pair(n32_section,				0) );
	m_map_Item_State.insert( make_pair(n33_issn,				0) );
	m_map_Item_State.insert( make_pair(n34_typeofwork,			0) );
	m_map_Item_State.insert( make_pair(n36_subsidiaryauthor,	0) );
	m_map_Item_State.insert( make_pair(n37_translatedauthor,	0) );
	m_map_Item_State.insert( make_pair(n38_originalpublication,	0) );
	m_map_Item_State.insert( make_pair(n39_secondaryauthor,		0) );
	m_map_Item_State.insert( make_pair(n40_reprintedition,		0) );
	m_map_Item_State.insert( make_pair(n41_publisher,			0) );
	m_map_Item_State.insert( make_pair(n42_numberofvolumes,		0) );
	m_map_Item_State.insert( make_pair(n43_tertiaryauthor,		1) );	// 1
	m_map_Item_State.insert( make_pair(n44_tertiarytitle,		0) );
	m_map_Item_State.insert( make_pair(n45_edition,				0) );
	m_map_Item_State.insert( make_pair(n46_alternatetitle,		0) );
	m_map_Item_State.insert( make_pair(n47_translatedtitle,		0) );
	m_map_Item_State.insert( make_pair(n48_linktopdf,			0) );
	m_map_Item_State.insert( make_pair(n49_image, 				0) );
	m_map_Item_State.insert( make_pair(n50_lastmodifieddate, 	0) );
	m_map_Item_State.insert( make_pair(n51_custom1, 			0) );
	m_map_Item_State.insert( make_pair(n52_custom2, 			0) );
	m_map_Item_State.insert( make_pair(n53_custom3, 			0) );
	m_map_Item_State.insert( make_pair(n54_custom4, 			0) );
	m_map_Item_State.insert( make_pair(n55_custom5,				0) );
	m_map_Item_State.insert( make_pair(n56_custom6,				0) );
	m_map_Item_State.insert( make_pair(n57_custom7,				0) );

	return TRUE;
}

//	设置标志字符串
void CCvtCNKIDlg::SetFlagWord()
{
	n00_refertype			= "n00_refertype"			+ m_sFlagWord;		//		n00.文献类型-已经不需要在此提供
	n02_title				= "n02_title" 				+ m_sFlagWord;		//		n02.标题
	n03_author				= "n03_author" 				+ m_sFlagWord;		//		n03.作者
	n04_year				= "n04_year" 				+ m_sFlagWord;		//		n04.年
	n06_placepublished		= "n06_placepublished" 		+ m_sFlagWord;		//		n06.出版地
	n09_totalpages			= "n09_totalpages" 			+ m_sFlagWord;		//		n09.页数
	n11_date				= "n11_date" 				+ m_sFlagWord;		//		n11.日期
	n12_revieweditem		= "n12_revieweditem" 		+ m_sFlagWord;		//		n12.评价条目
	n13_shorttitle			= "n13_shorttitle" 			+ m_sFlagWord;		//		n13.短标题
	n14_resourcenum			= "n14_resourcenum" 		+ m_sFlagWord;		//		n14.电子资源号
	n15_call				= "n15_call" 				+ m_sFlagWord;		//		n15.电话
	n16_accessnumber		= "n16_accessnumber" 		+ m_sFlagWord;		//		n16.获取序号.DOI
	n17_label				= "n17_label" 				+ m_sFlagWord;		//		n17.标签
	n18_keyword				= "n18_keyword" 			+ m_sFlagWord;		//		n18.关键词
	n19_abstract			= "n19_abstract" 			+ m_sFlagWord;		//		n19.摘要
	n20_note				= "n20_note" 				+ m_sFlagWord;		//		n20.注释
	n21_researchnote		= "n21_researchnote" 		+ m_sFlagWord;		//		n21.研究注释
	n22_url					= "n22_url" 				+ m_sFlagWord;		//		n22.网址
	n23_address				= "n23_address" 			+ m_sFlagWord;		//		n23.作者地址
	n24_caption				= "n24_caption" 			+ m_sFlagWord;		//		n24.caption
	n25_accessdate			= "n25_accessdate" 			+ m_sFlagWord;		//		n25.获取的日期
	n26_database			= "n26_database" 			+ m_sFlagWord;		//		n26.数据库名字
	n27_dbprovider			= "n27_dbprovider" 			+ m_sFlagWord;		//		n27.数据库提供商
	n28_language			= "n28_language" 			+ m_sFlagWord;		//		n28.语言
	n29_secondarytitle		= "n29_secondarytitle" 		+ m_sFlagWord;		//		n29.第二标题.期刊名称.系.从书名
	n30_volume				= "n30_volume" 				+ m_sFlagWord;		//		n30.卷.学位级别
	n31_number				= "n31_number" 				+ m_sFlagWord;		//		n31.数字.期
	n32_section				= "n32_section" 			+ m_sFlagWord;		//		n32.部分,项,开始页
	n33_issn				= "n33_issn" 				+ m_sFlagWord;		//		n33.issn/isbn
	n34_typeofwork			= "n34_typeofwork" 			+ m_sFlagWord;		//		n34.著作种类(学科)
	n36_subsidiaryauthor	= "n36_subsidiaryauthor"	+ m_sFlagWord;		//		n36.次要作者
	n37_translatedauthor	= "n37_translatedauthor"	+ m_sFlagWord;		//		n37.翻译者
	n38_originalpublication	= "n38_originalpublication"	+ m_sFlagWord;		//		n38.原出版社
	n39_secondaryauthor		= "n39_secondaryauthor"		+ m_sFlagWord;		//		n39.第二作者
	n40_reprintedition		= "n40_reprintedition"		+ m_sFlagWord;		//		n40.重印版次
	n41_publisher			= "n41_publisher" 			+ m_sFlagWord;		//		n41.出版社.大学
	n42_numberofvolumes		= "n42_numberofvolumes" 	+ m_sFlagWord;		//		n42.卷数
	n43_tertiaryauthor		= "n43_tertiaryauthor" 		+ m_sFlagWord;		//		n43.第三作者.导师
	n44_tertiarytitle		= "n44_tertiarytitle" 		+ m_sFlagWord;		//		n44.第四作者
	n45_edition				= "n45_edition" 			+ m_sFlagWord;		//		n45.版本
	n46_alternatetitle		= "n46_alternatetitle" 		+ m_sFlagWord;		//		n46.替换标题(缩写)
	n47_translatedtitle		= "n47_translatedtitle" 	+ m_sFlagWord;		//		n47.翻译题目.英文篇名
	n48_linktopdf			= "n48_linktopdf" 			+ m_sFlagWord;		//		n48.pdf链接
	n49_image				= "n49_image" 				+ m_sFlagWord;		//		n49.图像
	n50_lastmodifieddate	= "n50_lastmodifieddate" 	+ m_sFlagWord;		//		n50.最后修改日期
	n51_custom1				= "n51_custom1" 			+ m_sFlagWord;		//		n51.自定义项1
	n52_custom2				= "n52_custom2" 			+ m_sFlagWord;		//		n52.自定义项2
	n53_custom3				= "n53_custom3" 			+ m_sFlagWord;		//		n53.自定义项3
	n54_custom4				= "n54_custom4" 			+ m_sFlagWord;		//		n54.自定义项4
	n55_custom5				= "n55_custom5" 			+ m_sFlagWord;		//		n55.自定义项5
	n56_custom6	 			= "n56_custom6" 			+ m_sFlagWord;		//		n56.自定义项6
	n57_custom7	 			= "n57_custom7" 			+ m_sFlagWord;		//		n57.自定义项7

	m_space					= "space_flag";
	m_endofrefer			= "endofrefer_flag";

	//	系统选项
	o01_tooltip				= "o01_tooltip"				+ m_sFlagWord;		//	Tooltip状态表示词
	o02_doctor_thesis		= "o02_doctor_thesis"		+ m_sFlagWord;		//	博士论文标示词
	o03_master_thesis		= "o03_master_thesis"		+ m_sFlagWord;		//	硕士论文标示词
	o04_delete_word			= "o04_delete_word"			+ m_sFlagWord;		//	需要删除的词汇的标示词
	o05_outputfile_ext		= "o05_outputfile_ext"		+ m_sFlagWord;		//	扩展名表示词
	o06_abstract_split		= "o06_abstract_split"		+ m_sFlagWord;		//	截断摘要保留字符数
}


//	设置输出样式
BOOL CCvtCNKIDlg::SetOutputStyleKeyword()
{
	//	EndNote 格式
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n00_refertype, "%0") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n02_title, "%T") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n03_author, "%A") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n04_year, "%D") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n06_placepublished, "%C") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n09_totalpages, "%P") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n11_date, "%D") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n12_revieweditem, "%*") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n13_shorttitle, "%!") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n14_resourcenum, "%R") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n15_call, "%L") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n16_accessnumber, "%M") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n17_label, "%F") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n18_keyword, "%K") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n19_abstract, "%X") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n20_note, "%Z") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n21_researchnote, "%<") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n22_url, "%U") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n23_address, "%+") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n24_caption, "%^") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n25_accessdate, "%\\") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n26_database, "%~") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n27_dbprovider, "%W") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n28_language, "%G") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n29_secondarytitle, "%J") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n30_volume, "%V") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n31_number, "%N") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n32_section, "%&") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n33_issn, "%@") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n34_typeofwork, "%9") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n36_subsidiaryauthor, "%?") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n37_translatedauthor, "%H") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n38_originalpublication, "%(") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n39_secondaryauthor, "%E") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n40_reprintedition, "%)") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n41_publisher, "%I") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n42_numberofvolumes, "%6") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n43_tertiaryauthor, "%Y") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n44_tertiarytitle, "%S") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n45_edition, "%7") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n46_alternatetitle, "%O") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n47_translatedtitle, "%Q") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n48_linktopdf, "%>") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n49_image, "%Z") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n50_lastmodifieddate, "%=") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n51_custom1, "%1") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n52_custom2, "%2") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n53_custom3, "%3") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n54_custom4, "%4") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n55_custom5, "%#") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n56_custom6, "%$") );
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(n57_custom7, "%]") );

	m_pmap_OutputStyle_Keyword[0].insert( make_pair(m_space,		" ") );				//	a.	用来间隔标签和值域的间隔符号
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(m_endofrefer,		" ") );			//	b.	最后一项表示结束的标志

	//	RiS 格式
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n00_refertype, "TY") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n02_title, "T1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n03_author, "A1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n04_year, "Y1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n06_placepublished, "CP") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n09_totalpages, "SP") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n11_date, "Y2") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n12_revieweditem, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n13_shorttitle, "T2") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n14_resourcenum, "ID") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n15_call, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n16_accessnumber, "M3") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n17_label, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n18_keyword, "KW") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n19_abstract, "N2") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n20_note, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n21_researchnote, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n22_url, "UR") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n23_address, "AD") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n24_caption, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n25_accessdate, "Y2") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n26_database, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n27_dbprovider, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n28_language, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n29_secondarytitle, "JF") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n30_volume, "VL") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n31_number, "IS") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n32_section, "SP") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n33_issn, "SN") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n34_typeofwork, "TW") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n36_subsidiaryauthor, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n37_translatedauthor, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n38_originalpublication, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n39_secondaryauthor, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n40_reprintedition, "RP") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n41_publisher, "PB") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n42_numberofvolumes, "M1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n43_tertiaryauthor, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n44_tertiarytitle, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n45_edition, "SP") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n46_alternatetitle, "JO") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n47_translatedtitle, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n48_linktopdf, "L1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n49_image, "L4") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n50_lastmodifieddate, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n51_custom1, "U1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n52_custom2, "U2") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n53_custom3, "U3") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n54_custom4, "U4") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n55_custom5, "U5") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n56_custom6, "N1") );
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(n57_custom7, "N1") );

	m_pmap_OutputStyle_Keyword[1].insert( make_pair(m_space,			"  - ") );		//	a.	用来间隔标签和值域的间隔符号
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(m_endofrefer,		"ER") );		//	b.	最后一项表示结束的标志

	//	BibTeX 格式
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n00_refertype, "") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n02_title, "title") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n03_author, "author") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n04_year, "year") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n06_placepublished, "address") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n09_totalpages, "pages") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n11_date, "month") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n12_revieweditem, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n13_shorttitle, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n14_resourcenum, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n15_call, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n16_accessnumber, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n17_label, "label") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n18_keyword, "keywords") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n19_abstract, "abstract") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n20_note, "note") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n21_researchnote, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n22_url, "url") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n23_address, "address") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n24_caption, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n25_accessdate, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n26_database, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n27_dbprovider, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n28_language, "lang") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n29_secondarytitle, "journal") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n30_volume, "volume") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n31_number, "number") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n32_section, "chapter") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n33_issn, "isbn") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n34_typeofwork, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n36_subsidiaryauthor, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n37_translatedauthor, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n38_originalpublication, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n39_secondaryauthor, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n40_reprintedition, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n41_publisher, "publisher") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n42_numberofvolumes, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n43_tertiaryauthor, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n44_tertiarytitle, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n45_edition, "edition") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n46_alternatetitle, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n47_translatedtitle, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n48_linktopdf, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n49_image, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n50_lastmodifieddate, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n51_custom1, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n52_custom2, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n53_custom3, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n54_custom4, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n55_custom5, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n56_custom6, "annote") );
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(n57_custom7, "annote") );

	m_pmap_OutputStyle_Keyword[2].insert( make_pair(m_space,		" = ") );				//	a.	用来间隔标签和值域的间隔符号
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(m_endofrefer,		"") );			//	b.	最后一项表示结束的标志

	return TRUE;
}
