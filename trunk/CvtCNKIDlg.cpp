// CvtCNKIDlg.cpp : ʵ���ļ�
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

const int	TXTFILE					= 1;	//	m_nReferenceFrom	���ļ���ȡ����
const int	CLIPBOARD				= 2;	//	m_nReferenceFrom	�Ӽ��а��ȡ����
const int	MAX_REFERENCENUMBER		= 500;	//	ÿ����ദ�����¼������Ŀǰcnki����Ϊ50��
const int	MAX_FORMATNUMBER		= 3;	//	�����ת����ʽ������ĿǰΪ2���ֱ��� EndNote �� RiS �� BibTeX
const int	MAX_REFERENCETYPEVECTOR	= 9;	//	�ܴ�����������������ʽ


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_sAutoInfo;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_sAutoInfo = "CvtCNKI�����׹���ĸ���������ɽ��й��ڿ����������ݿ⣬����ά�����ݿ⣬�й���������վ��ļ������ת��Ϊ EndNote, RiS, BibTeX �ȱ�׼��ʽ���Ե��뵽 EndNote �����׹�������С�\r\n\r\nCvtCNKI ��ѭ GNU ͨ�ù������֤(GPL) ����Դ�룬�κ��˾�������ʹ�á����� GPL ��������и��ơ��޸ĺ����·���������ģ���������(JiangZhanyong)��Ϊ�����汾�е��κ����Ρ�\r\n\r\n��Ŀ��ַ: http://code.google.com/p/cvtcnki/ \r\n�������: http://code.google.com/p/cvtcnki/downloads/list \r\n������վ: http://jiangzhanyong.com/\r\n";
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITINFO, m_sAutoInfo);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCvtCNKIDlg �Ի���
CCvtCNKIDlg::CCvtCNKIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCvtCNKIDlg::IDD, pParent)
	, m_sPages(_T(""))
	, m_sDatabaseName(_T(""))
	, m_sAnnotation(_T(""))
	, m_sLang(_T(""))
{
	std::locale::global(std::locale(""));				//	�Ա���ȷ��������·������
	//	��¼��������·��
	char path[MAX_PATH] = {'\0'};
	GetModuleFileName(NULL, path, MAX_PATH);			//���Ӧ�ó���ǰ����·��
	m_CurrentPath.Format("%s", path);
	int iPosition;
	iPosition = m_CurrentPath.ReverseFind('\\');
	m_CurrentPath = m_CurrentPath.Left(iPosition + 1);	//��������Ŀ¼

	m_sFlagWord = "_flag";								//	��־�ʣ���������Ǹ��ؼ���
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


// CCvtCNKIDlg ��Ϣ�������

BOOL CCvtCNKIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �����Ҫ��map
	m_pmmap_ReferenceData		= new StrStrVectorMMap[MAX_REFERENCENUMBER];
	m_pmap_OutputStyle_Keyword	= new StrStrMMap[MAX_FORMATNUMBER];

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_SHOW);

	//	����һЩ��Ҫ�ĳ�ʼ��������ȡ�����ļ���
	SetFlagWord();							//	�������ñ�־��	
	if ( !GetDataBaseKeyword() ){			//	���ļ���ȡ�û���������ݿ�ؼ���
		return FALSE;
	}

	// ��ʼ�����������͡���CListBox�б�
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

	// ��ʼ���������ʽ����CListBox�б�
	CListBox* pOutPutListBox = static_cast<CListBox*>(GetDlgItem(IDC_OUTPUTSTYLE));
	pOutPutListBox->AddString(_T("EndNote"));
	pOutPutListBox->AddString(_T("RiS"));
	pOutPutListBox->AddString(_T("BibTeX"));
	pOutPutListBox->SetCurSel(0);


	AddToolTip();		// �����ʾ������

	return TRUE;		// ���ǽ��������õ��ؼ������򷵻� TRUE
}

//	���С��ʾ
void CCvtCNKIDlg::AddToolTip()
{
	EnableToolTips(TRUE);

	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.SetMaxTipWidth(150);

	//	����Ƿ�Ҫ��ʾС��ʾ
	StrStrMMap::const_iterator citer;
	std::string tooltip_string;
	for( citer = m_mmap_Option.begin(); citer != m_mmap_Option.end(); ++citer ){
		if ( citer->first == o01_tooltip){
			tooltip_string = citer->second;
			break;
		}
	}

	if (tooltip_string == "1"){					//	���Ϊ1������ʾ��С��ʾ

		m_tooltip.AddTool(GetDlgItem(IDC_FILE), "��Ҫת������¼�ļ��ж�ȡ����");					// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_CLIPBOARD), "��Ҫ�������ж�ȡ����");						// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_CONVERT), "����ת������");									// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_OPENOUTPUTFILE), "�������չ��Ϊtxt������鿴������������չ��Ϊ��׼��ʽ������������׹������������ļ���չ�����������ļ�ָ����");	// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_REFERENCETYPE), "ָ��Ҫת������¼���������ͣ��������·���Ҳ���Խ��в�ͬ������¼��ʽ��ת��");											// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_OUTPUTSTYLE), "ָ�������������¼�ĸ�ʽ");					// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_PAGES), "���ֻת��һƪ������¼����ָ��ҳ��");				// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_DATABASENAME), "���ת����������¼����ͬһ���ݿ⣬���ڴ�ָ��");	// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_ANNOTATION), "���ڴ����ע��");							// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_LANG), "���ڴ�ָ��������¼����");							// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_SETOUTPUTDIR), "���ڴ�ָ��ת����������¼�����Ŀ¼");		// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_POPUPMENU), "����򿪲˵�");								// ���tip
		m_tooltip.AddTool(GetDlgItem(ID_SPONSOR1), "������ȫ�����׹����������Ѷ");			// ���tip
		m_tooltip.AddTool(GetDlgItem(ID_SPONSOR2), "�������������������ʺϸ��˽�վ������򿪲˵�");	// ���tip
		m_tooltip.AddTool(GetDlgItem(IDC_WEBSITE), "����������վ: http://jiangzhanyong.com/");	// ���tip
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCvtCNKIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CCvtCNKIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���ļ��ж�ȡ����
void CCvtCNKIDlg::OnBnClickedFile()
{
	m_nReferenceFrom = TXTFILE;
	CFileDialog dlg( TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("�ı��ļ�(*.txt)|*.txt|EndNote��¼(*.enw)|*.enw|Ris��¼(*.ris)|*.ris|�����ļ�(*.*)|*.*||") );

	if ( IDOK == dlg.DoModal() ) {		
		m_PathName		= dlg.GetPathName();						//	��ȡҪת�����ļ���·��
		int iPosition	= m_PathName.ReverseFind('\\');				//	�������ļ���ȥ��
		m_PathName		= m_PathName.Left(iPosition+1);				//	��������Ŀ¼���������һ����б��
		m_FileName		= dlg.GetFileName();						//	��ȡҪת�����ļ����ļ���������ǰ׺�ͺ�׺��
		m_FileTitle		= dlg.GetFileTitle();						//	��ȡҪת�����ļ����ļ���
	}
}

//�Ӽ������ж�ȡ����
void CCvtCNKIDlg::OnBnClickedMemory()
{
	m_nReferenceFrom = CLIPBOARD;

	if ( !IsClipboardFormatAvailable(CF_TEXT) ) {
		MessageBox("���а����ݲ�����Ҫ��", "����", MB_ICONERROR);
		return;
	}

	if ( !OpenClipboard() ){							//	�򿪼��а�
		MessageBox("��ȡ���а����", "����", MB_ICONERROR);
		return;	
	}

	CreateTempFile();

	m_PathName		= m_CurrentPath;					//	�������һ����б��
	m_FileName		= "tempRef.txt";					//	������ʱ�ļ����ļ�������չ���ȣ����������
	m_FileTitle		= "tempRef";
}

//����Ӽ������ȡ���ݣ���Ҫ�����м��ļ�
void CCvtCNKIDlg::CreateTempFile()
{
	std::ofstream ofile("tempRef.txt");

	HANDLE hClipMemory = ::GetClipboardData(CF_TEXT);		// ��ȡ���������ݾ��
//	DWORD dwLength = GlobalSize(hClipMemory);				// ����ָ���ڴ�����ĵ�ǰ��С
	LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory);	// �����ڴ�
	CString sMessage = CString(lpClipMemory);				// ����õ����ı�����
	ofile << sMessage; 
	GlobalUnlock(hClipMemory);								// �ڴ����
	::CloseClipboard();										// �رռ�����
}

//����ת������
void CCvtCNKIDlg::OnBnClickedConvert()
{
	if ( m_FileName == "" ) {							//	��ֹδ���ļ�����ֱ�ӽ���ת����			
		MessageBox("�ļ���Ϊ�ա�", "����", MB_ICONERROR);
		return;
	}

	std::ifstream infile( m_FileName );					//	��ȡ�ļ��� infile	
	if( !infile ) {										//	����ļ��򲻿������˳�
		MessageBox("�޷���Դ��¼�ļ���", "����", MB_ICONERROR);
		return;
	}

	if ( !InitializeSetting() ){
		return;
	}

	if ( !ReadData() ){
		MessageBox("��ȡ��¼�ļ�����", "����", MB_ICONERROR);
		return;
	}

	if ( !WriteData() ){
		MessageBox("д�뵽����ļ�����", "����", MB_ICONERROR);
		return;
	}

	return;	
}

//	��������
void CCvtCNKIDlg::OnBnClickedOpenoutputfile()
{
	ShellExecute(NULL, "open", m_OutputFileName, NULL, NULL, SW_SHOW);
}

//	���ڶԻ���
void CCvtCNKIDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	if ( IDOK == dlg.DoModal() ) {		
	};
}

//	�����˵�
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

//	��ʼ������
BOOL CCvtCNKIDlg::InitializeSetting()
{
	if ( !GetDataBaseKeyword() ){			//	���ļ���ȡ�û���������ݿ�ؼ���
		return FALSE;
	}

	if ( !SetOutputStyleKeyword() ){		//	��������ĸ�ʽ RiS - EndNote - BibTeX
		MessageBox("���ø�ʽ����", "����", MB_ICONERROR);
		return FALSE;
	}

	SetCurrentMap();						//	���õ�ǰҪʹ�õ�MAP
	SetItemState();							//	����ÿ��item�Ƿ���Ҫ����

	return TRUE;
}


//	���ļ��ж�ȡ���ݿ���¼��ʽ
BOOL CCvtCNKIDlg::GetDataBaseKeyword()
{	
	ClearData();							//	��ո�������

	//	���Ȼ�ȡ���ݿ���¼��ʽ�ļ�����λ��
	CString CurrentPath;
	char path[MAX_PATH] = {'\0'};
	GetModuleFileName(NULL, path, MAX_PATH);			//���Ӧ�ó���ǰ����·��
	CurrentPath.Format("%s", path);
	int iPosition = CurrentPath.ReverseFind('\\');
	CurrentPath = CurrentPath.Left(iPosition + 1);		//��������Ŀ¼

	CString CfgFileName = CurrentPath + "CvtCNKI.ini";

	std::ifstream infile( CfgFileName );				//	��ȡ�ļ��� infile	
	if( !infile ) {										//	����ļ��򲻿������˳�
		MessageBox("�޷������ݿ������ļ���", "����", MB_ICONERROR);
		return FALSE;
	}

	std::string	contextWord;							//	�����ַ������ֱ�����ǰ��֮ǰ��ȡ������

	while ( std::getline(infile, contextWord, '\n') ){
		ExtractDataBaseKeyword(contextWord);
	}

	return TRUE;
}

//	�Ӷ�������������ȡ�ؼ���
void CCvtCNKIDlg::ExtractDataBaseKeyword(std::string thestring)
{
	std::string	contextWordNoAnnotation;							//	ȥ����ע�Ͳ��ֺ���ַ���
	std::string annotationFlag = ";";								//	ע���б�־

	DeleteBlankStringofBeginAndEnd(thestring);						//	ɾ����ʼ��ĩβ�Ŀո�
	std::string::size_type iPos = thestring.find_first_of(annotationFlag);				//	����ע���б�־

	if( iPos == std::string::npos )	{								//	���û���ҵ�ע�ͱ�־��ֱ�Ӵ���
		contextWordNoAnnotation = thestring;
	}
	else{															//	�ҵ�ע�ͱ�־��ȥ��ע�Ͳ���
		contextWordNoAnnotation = thestring.substr(0, iPos);
		DeleteBlankStringofBeginAndEnd(contextWordNoAnnotation);	//	ɾ����ʼ��ĩβ�Ŀո�
		if ( contextWordNoAnnotation.empty() )	{ return;	}
	}

	iPos = contextWordNoAnnotation.find(m_sFlagWord);				//	���ұ�ʾ��
	if (iPos != 0 && iPos != std::string::npos){
		std::string key = contextWordNoAnnotation.substr(0, iPos+m_sFlagWord.size());
		std::string tempstring = contextWordNoAnnotation.substr(key.size());
		DeleteBlankStringofBeginAndEnd(tempstring);
		std::string value = tempstring.substr(1, tempstring.size()-2);		//	ȥ����β��{}��
		std::string first_string = key.substr(0, 1);
		if ( first_string == "n" )			//	��ʾ��
		{
			m_mmap_DataBase_Keyword.insert( make_pair(key, value) );
			m_mmap_DataBase_Keyword.insert( make_pair(key, ConvertUtf8ToGBK(value)) );
		} 
		else if ( first_string == "o" )		//	ѡ��
		{
			m_mmap_Option.insert( make_pair(key, value) );
			m_mmap_Option.insert( make_pair(key, ConvertUtf8ToGBK(value)) );
		}
		else if ( first_string == "s" )		//	�������Ͷ�Ӧ���ַ���
		{
			m_String_ReferenceTypeVector.push_back(ConvertUtf8ToGBK(value));
		} 
		else if ( first_string == "e" )		//	EndNote��������
		{
			m_EndNote_ReferenceTypeVector.push_back(ConvertUtf8ToGBK(value));
		}
		else if ( first_string == "r" )		//	Ris��������
		{
			m_Ris_ReferenceTypeVector.push_back(ConvertUtf8ToGBK(value));
		} 
		else if ( first_string == "b" )		//	BibTeX��������
		{
			m_BibTeX_ReferenceTypeVector.push_back(ConvertUtf8ToGBK(value));
		}
	}
}

//	��UTF8��gbk���б���ת��
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

//	��ո�������
void CCvtCNKIDlg::ClearData()
{
	int i;
	for(i = 0; i < MAX_REFERENCENUMBER; ++i){
		m_pmmap_ReferenceData[i].clear();				//	���֮ǰÿ����¼������
	}

	for(i = 0; i < MAX_FORMATNUMBER; ++i) {
		m_pmap_OutputStyle_Keyword[i].clear();			//	��������ʽ��map
	}

	m_mmap_DataBase_Keyword.clear();					//	������ݿ�ؼ���
	m_mmap_Option.clear();								//	���ѡ������
	m_String_ReferenceTypeVector.clear();				//	����������Ͷ�Ӧ���ַ���
	m_EndNote_ReferenceTypeVector.clear();				//	���EndNote��������
	m_Ris_ReferenceTypeVector.clear();					//	���Ris��������
	m_BibTeX_ReferenceTypeVector.clear();				//	���BibTeX ��������
	m_mmap_Curent_OutputStyle_Keyword.clear();			//	���ʵ�ʲ��õ����map
	m_Source_ReferenceTypeVector.clear();				//	���ת��ǰ�ĵ����׸�ʽ��ʾ�б�
	m_Current_ReferenceTypeVector.clear();				//	���ʵ�ʲ��õĵ����׸�ʽ��ʾ�б�
	m_map_Item_State.clear();							//	��ո����Ƿ���Ҫ�ָ��map

	m_sFirstLibraryWord		= "";						//	������һ����־
}

//	ɾ��������Ч���ַ��������зִ�
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


//	ɾ���ַ�����ͷ�ͽ�βλ�õĿո�~
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

//	ɾ���ַ�����ͷ������
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

//	ɾ���ַ����еĵ�����
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

//	����ַ������Ƿ����Ӣ���ַ�
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
	//	�������ݽ���
	UpdateData(TRUE);

	//	ȷ�����Ŀ¼	
	std::string UserSettingPath(m_UserSettingPath.GetBuffer());
	if( UserSettingPath.empty() ){
		m_FileTitle = m_PathName + m_FileTitle;
	}
	else{
		m_FileTitle = m_UserSettingPath + m_FileTitle;
	}

	//	ȷ�����Ris ���� EndNote ���� BibTeX
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


	//	��ȷ���������������
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

	//	����ǽ��� EndNote Ris����Ҫ���������ָ��
	if ( m_iReferenceType == m_String_ReferenceTypeVector.size() ){
		m_Source_ReferenceTypeVector = m_EndNote_ReferenceTypeVector;
	}
	else if( (m_iReferenceType == m_String_ReferenceTypeVector.size()+1) )	{
		m_Source_ReferenceTypeVector = m_Ris_ReferenceTypeVector;
	}


	return TRUE;
}

//	��ȡ����
BOOL CCvtCNKIDlg::ReadData()
{
	m_nReferenceNumber = -1;
	std::ifstream infile( m_FileName );			//	��ȡ�ļ��� infile	
	std::string	contextWord;					//	�����ַ������ֱ�����ǰ��֮ǰ��ȡ������

	while ( std::getline(infile, contextWord, '\n') ){
		if ( contextWord != "" ){
			ProcessData(contextWord);
		}		
	}

	return TRUE;
}

//	�����������ļ�
BOOL CCvtCNKIDlg::WriteData()
{
	std::string	fmt_space, fmt_endofrefer, fmt_refertype;
	fmt_space		= m_mmap_Curent_OutputStyle_Keyword.find(m_space)->second;
	fmt_endofrefer	= m_mmap_Curent_OutputStyle_Keyword.find(m_endofrefer)->second;
	fmt_refertype	= m_mmap_Curent_OutputStyle_Keyword.find(n00_refertype)->second;

	std::ofstream outfile(m_OutputFileName);		//	����� outfile �ļ����ļ��� result.txt

	if ( !outfile ) {								//	����޷������ļ������˳�		
		MessageBox("��������ļ�ʧ�ܡ�", "����", MB_ICONERROR);
		return FALSE;
	}

	++m_nReferenceNumber;
	for (int i = 0; i < m_nReferenceNumber; ++i)
	{
		StrStrVectorMMap::iterator iter_referdata;
		StrStrMMap::iterator iter_format;
		StrVector::iterator iter_itemstring;

		//	���������������
		if ( m_iReferenceType < m_String_ReferenceTypeVector.size() )		//	���б���ѡ����������
		{
			if( m_iPosOutputStyle == 2 ){		//	��� BibTeX ������
				outfile << "@" << fmt_refertype << m_Current_ReferenceType << "{," << std::endl;
			}
			else{
				outfile << fmt_refertype << fmt_space << m_Current_ReferenceType << std::endl;
			}
		}
		else{													//	�������׸�ʽת��ʱ�����������
			StrStrVectorMMap::iterator iter_referdata_type = m_pmmap_ReferenceData[i].find(n00_refertype);
			std::string	tempstring_refertype = iter_referdata_type->second.front();
			std::string bibtex_refertype;

			// ������ͨ�����͵�ת������Ҫ���⴦��
			if( tempstring_refertype == "Generic" 
				&& (m_Source_ReferenceTypeVector == m_EndNote_ReferenceTypeVector) )	{		//	�� EndNote ת��
				if( m_Current_ReferenceTypeVector == m_Ris_ReferenceTypeVector ){				//	�� EndNote �� Ris
					iter_referdata_type->second.pop_back();
					iter_referdata_type->second.push_back("GEN");
				}
				else if( m_Current_ReferenceTypeVector == m_BibTeX_ReferenceTypeVector ){		//	�� EndNote �� BibTex
					iter_referdata_type->second.pop_back();
					bibtex_refertype = "misc";
				}
			}
			else if( tempstring_refertype == "GEN"												//	�� RiS ת��
				&& (m_Source_ReferenceTypeVector == m_Ris_ReferenceTypeVector) ){		
				if( m_Current_ReferenceTypeVector == m_EndNote_ReferenceTypeVector ){			//	�� Ris �� EndNote
					iter_referdata_type->second.pop_back();
					iter_referdata_type->second.push_back("Generic");
				}
				else if( m_Current_ReferenceTypeVector == m_BibTeX_ReferenceTypeVector ){		//	�� Ris �� BibTex
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

			if( m_iPosOutputStyle == 2 ){		//	��� BibTeX ������
				outfile << "@" << fmt_refertype << bibtex_refertype << "{," << std::endl;
			}
		}

		//	����������е���¼
		for( iter_referdata = m_pmmap_ReferenceData[i].begin(); 
			 iter_referdata != m_pmmap_ReferenceData[i].end();
			 ++iter_referdata )	{

			//	������¼��ÿһ��������
			for( iter_format = m_mmap_Curent_OutputStyle_Keyword.begin(); 
				 iter_format != m_mmap_Curent_OutputStyle_Keyword.end();
				 ++iter_format ){
				if ( (iter_referdata->first) == (iter_format->first) )
				{
					if( m_iPosOutputStyle == 2 ){	//	��� BibTeX �����ʽ�������⴦��
						size_t iTotalNumber = iter_referdata->second.size();
						int index = 1;
						for( iter_itemstring = iter_referdata->second.begin();
							iter_itemstring != iter_referdata->second.end();
							++iter_itemstring, ++index ){
								if (index == 1)	{			//	index == 1��˵����һ�Σ�Ҫ���ǰ��Ĳ���
									if ( iter_format->first == n41_publisher ){
										//	������һ����һ���ر�������һ�£�������Ҫ�����ģ�Ҳ�����������
										if ( (m_Current_ReferenceType == "phdthesis") || (m_Current_ReferenceType == "masterthesis") ) {
											outfile << "school";
										}
										else if( m_Current_ReferenceType == "techreport" ){
											outfile << "insitution";
										}
									}
									else if(iter_format->first == n30_volume){
										//	ѧλ���ļ��𣬶�Ӧ�ľ���Ϊtype
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
								else{					//	index != 1	Ҫ����м䲿��
									outfile << " and " << (*iter_itemstring) ;	
								}

								if ( index == iTotalNumber ){	//	index == iTotalNumber�����Ҫ�����������������
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

		//	����û��Զ����� 
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

			//	���������־
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

			//	���������־
			outfile << fmt_endofrefer << fmt_space << "\n\n" << std::endl;			
		}
	}

	return TRUE;
}

//	�����ȡ�õ���ÿһ������
void CCvtCNKIDlg::ProcessData(string& thestring)
{	
	int temptemptemtp = 0;
	DeleteBlankStringofBeginAndEnd(thestring);							//	ɾ����ʼ��ĩβ�Ŀո�
	DeleteNumberStringofBegin(thestring, "1234567890.,-()[]/ ");		//	ɾ����ʼ������

	if ( thestring.size() == 0 ){	return;	}							//	��� thestring Ϊ0���򲻱ش���

	std::string::size_type pos = 999;	
	std::string::size_type ExactPos = std::string::npos;				//	��¼һ����ȷ��λ��
	std::string::size_type validindex = 6;								//	��Ч������ʼֵ���������
	std::string::size_type targetwordsize = 0;							//	����Ҫ�ȽϵĹؼ��ʵĴ�С
	StrStrMMap::const_iterator citer;
	StrStrMMap::const_iterator Exactciter;								//	��¼��ʱ��λ��
	std::vector<std::string> vStringKeyword;
	std::string tempstring;

	//	�����ҵ���ƥ���λ��
	for( citer = m_mmap_DataBase_Keyword.begin(); citer != m_mmap_DataBase_Keyword.end(); ++citer )
	{
		pos = thestring.find(citer->second);
		if( (targetwordsize < citer->second.size()) && (pos < ExactPos || pos == 0) ){	
			ExactPos = pos;
			Exactciter = citer;	
			tempstring = thestring;
			targetwordsize = citer->second.size();
			DeleteInvalidString(tempstring, vStringKeyword);
			tempstring = vStringKeyword.front();						//	��һ��ò��û�ã�tempstring ���沢û���õ���2007.05.31
		} 
	}

	//	ƥ��ɹ��󣬼����û����Ҫɾ�����ַ���
	std::string::size_type deletewordpos;
	for( citer = m_mmap_Option.begin(); citer != m_mmap_Option.end(); ++citer ){
		deletewordpos = thestring.find(citer->second);
		if ( (deletewordpos != std::string::npos) && (citer->first == o04_delete_word) ){
			std::string strbegin = thestring.substr(0, deletewordpos);	//	��ȡ��Ч���ַ���
			std::string strend = thestring.substr(deletewordpos+1);	
			thestring = strbegin + strend;
		}
	}

	//	��¼����ƥ���λ�ú󣬿�ʼ�������ݴ���
	if (ExactPos != std::string::npos && ExactPos < validindex){
		if ( m_sFirstLibraryWord == "" ){
			m_sFirstLibraryWord = Exactciter->second;
			++m_nReferenceNumber;
		}
		else if( m_sFirstLibraryWord == Exactciter->second )	{
			++m_nReferenceNumber;
		}

		tempstring = thestring.substr(ExactPos+Exactciter->second.size());	//	��ȡ��Ч���ַ���
		DeleteBlankStringofBeginAndEnd(tempstring);							//	ɾ���õ����ַ�������β�հ�
		if( n03_author == Exactciter->first ){
			DeleteNumberString(tempstring, "1234567890-()[]/");
		}

		if( !tempstring.empty() ){
			std::vector<std::string> vString;
			if ( m_iReferenceType < m_String_ReferenceTypeVector.size() ){					//	�����ĸ�ʽת��
				if ( (Exactciter->first == n30_volume) && (m_Current_ReferenceType == "phdthesis") ){		//	�����ѧλ���ģ���Ҫ�ж���˶ʿ���Ļ��ǲ�ʿ����
					std::string::size_type thesispos;
					for( citer = m_mmap_Option.begin(); citer != m_mmap_Option.end(); ++citer ){
						thesispos = tempstring.find(citer->second);
						if ( thesispos != std::string::npos ){
							if ( citer->first == o02_doctor_thesis ){
								tempstring = "��ʿѧλ����";
								break;
							}
							else if( citer->first == o03_master_thesis ){
								tempstring = "˶ʿѧλ����";
								break;
							}
						}
					}
				}

				if ( m_map_Item_State.find(Exactciter->first)->second && !IsEnglishVersion(tempstring) ){		//	�ж��ַ����Ƿ���Ҫ�ֳɶ������ؼ��ʣ�����
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
			else{		//	m_iReferenceType >= m_String_ReferenceTypeVector.size()			//	EndNote Ris ���������ߴ� EndNote Ris ת���� BibTeX
				DeleteNumberStringofBegin(tempstring, ".,-()[] ");		//	��һ����Ҫȥ�� - ����
				StrStrVectorMMap::const_iterator citer_str_strVector = m_pmmap_ReferenceData[m_nReferenceNumber].find(Exactciter->first);
				if( citer_str_strVector == m_pmmap_ReferenceData[m_nReferenceNumber].end() ){
					if ( m_map_Item_State.find(Exactciter->first)->second && !IsEnglishVersion(tempstring) ){		//	�ж��ַ����Ƿ���Ҫ�ֳɶ������ؼ��ʣ�����
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

//	�û�ѡ�����Ŀ¼
void CCvtCNKIDlg::OnBnClickedSetoutputdir()
{	
	char* path = ChooseDirectory("�趨 CvtCNKI ת��������λ��");
	m_UserSettingPath.Format("%s", path);
	m_UserSettingPath = m_UserSettingPath + "\\";				//	�������һ��
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

//	����ÿһ���Ƿ���Ҫ�ָ���״̬, trueΪ��Ҫ����faseΪ����Ҫ����
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

//	���ñ�־�ַ���
void CCvtCNKIDlg::SetFlagWord()
{
	n00_refertype			= "n00_refertype"			+ m_sFlagWord;		//		n00.��������-�Ѿ�����Ҫ�ڴ��ṩ
	n02_title				= "n02_title" 				+ m_sFlagWord;		//		n02.����
	n03_author				= "n03_author" 				+ m_sFlagWord;		//		n03.����
	n04_year				= "n04_year" 				+ m_sFlagWord;		//		n04.��
	n06_placepublished		= "n06_placepublished" 		+ m_sFlagWord;		//		n06.�����
	n09_totalpages			= "n09_totalpages" 			+ m_sFlagWord;		//		n09.ҳ��
	n11_date				= "n11_date" 				+ m_sFlagWord;		//		n11.����
	n12_revieweditem		= "n12_revieweditem" 		+ m_sFlagWord;		//		n12.������Ŀ
	n13_shorttitle			= "n13_shorttitle" 			+ m_sFlagWord;		//		n13.�̱���
	n14_resourcenum			= "n14_resourcenum" 		+ m_sFlagWord;		//		n14.������Դ��
	n15_call				= "n15_call" 				+ m_sFlagWord;		//		n15.�绰
	n16_accessnumber		= "n16_accessnumber" 		+ m_sFlagWord;		//		n16.��ȡ���.DOI
	n17_label				= "n17_label" 				+ m_sFlagWord;		//		n17.��ǩ
	n18_keyword				= "n18_keyword" 			+ m_sFlagWord;		//		n18.�ؼ���
	n19_abstract			= "n19_abstract" 			+ m_sFlagWord;		//		n19.ժҪ
	n20_note				= "n20_note" 				+ m_sFlagWord;		//		n20.ע��
	n21_researchnote		= "n21_researchnote" 		+ m_sFlagWord;		//		n21.�о�ע��
	n22_url					= "n22_url" 				+ m_sFlagWord;		//		n22.��ַ
	n23_address				= "n23_address" 			+ m_sFlagWord;		//		n23.���ߵ�ַ
	n24_caption				= "n24_caption" 			+ m_sFlagWord;		//		n24.caption
	n25_accessdate			= "n25_accessdate" 			+ m_sFlagWord;		//		n25.��ȡ������
	n26_database			= "n26_database" 			+ m_sFlagWord;		//		n26.���ݿ�����
	n27_dbprovider			= "n27_dbprovider" 			+ m_sFlagWord;		//		n27.���ݿ��ṩ��
	n28_language			= "n28_language" 			+ m_sFlagWord;		//		n28.����
	n29_secondarytitle		= "n29_secondarytitle" 		+ m_sFlagWord;		//		n29.�ڶ�����.�ڿ�����.ϵ.������
	n30_volume				= "n30_volume" 				+ m_sFlagWord;		//		n30.��.ѧλ����
	n31_number				= "n31_number" 				+ m_sFlagWord;		//		n31.����.��
	n32_section				= "n32_section" 			+ m_sFlagWord;		//		n32.����,��,��ʼҳ
	n33_issn				= "n33_issn" 				+ m_sFlagWord;		//		n33.issn/isbn
	n34_typeofwork			= "n34_typeofwork" 			+ m_sFlagWord;		//		n34.��������(ѧ��)
	n36_subsidiaryauthor	= "n36_subsidiaryauthor"	+ m_sFlagWord;		//		n36.��Ҫ����
	n37_translatedauthor	= "n37_translatedauthor"	+ m_sFlagWord;		//		n37.������
	n38_originalpublication	= "n38_originalpublication"	+ m_sFlagWord;		//		n38.ԭ������
	n39_secondaryauthor		= "n39_secondaryauthor"		+ m_sFlagWord;		//		n39.�ڶ�����
	n40_reprintedition		= "n40_reprintedition"		+ m_sFlagWord;		//		n40.��ӡ���
	n41_publisher			= "n41_publisher" 			+ m_sFlagWord;		//		n41.������.��ѧ
	n42_numberofvolumes		= "n42_numberofvolumes" 	+ m_sFlagWord;		//		n42.����
	n43_tertiaryauthor		= "n43_tertiaryauthor" 		+ m_sFlagWord;		//		n43.��������.��ʦ
	n44_tertiarytitle		= "n44_tertiarytitle" 		+ m_sFlagWord;		//		n44.��������
	n45_edition				= "n45_edition" 			+ m_sFlagWord;		//		n45.�汾
	n46_alternatetitle		= "n46_alternatetitle" 		+ m_sFlagWord;		//		n46.�滻����(��д)
	n47_translatedtitle		= "n47_translatedtitle" 	+ m_sFlagWord;		//		n47.������Ŀ.Ӣ��ƪ��
	n48_linktopdf			= "n48_linktopdf" 			+ m_sFlagWord;		//		n48.pdf����
	n49_image				= "n49_image" 				+ m_sFlagWord;		//		n49.ͼ��
	n50_lastmodifieddate	= "n50_lastmodifieddate" 	+ m_sFlagWord;		//		n50.����޸�����
	n51_custom1				= "n51_custom1" 			+ m_sFlagWord;		//		n51.�Զ�����1
	n52_custom2				= "n52_custom2" 			+ m_sFlagWord;		//		n52.�Զ�����2
	n53_custom3				= "n53_custom3" 			+ m_sFlagWord;		//		n53.�Զ�����3
	n54_custom4				= "n54_custom4" 			+ m_sFlagWord;		//		n54.�Զ�����4
	n55_custom5				= "n55_custom5" 			+ m_sFlagWord;		//		n55.�Զ�����5
	n56_custom6	 			= "n56_custom6" 			+ m_sFlagWord;		//		n56.�Զ�����6
	n57_custom7	 			= "n57_custom7" 			+ m_sFlagWord;		//		n57.�Զ�����7

	m_space					= "space_flag";
	m_endofrefer			= "endofrefer_flag";

	//	ϵͳѡ��
	o01_tooltip				= "o01_tooltip"				+ m_sFlagWord;		//	Tooltip״̬��ʾ��
	o02_doctor_thesis		= "o02_doctor_thesis"		+ m_sFlagWord;		//	��ʿ���ı�ʾ��
	o03_master_thesis		= "o03_master_thesis"		+ m_sFlagWord;		//	˶ʿ���ı�ʾ��
	o04_delete_word			= "o04_delete_word"			+ m_sFlagWord;		//	��Ҫɾ���Ĵʻ�ı�ʾ��
	o05_outputfile_ext		= "o05_outputfile_ext"		+ m_sFlagWord;		//	��չ����ʾ��
	o06_abstract_split		= "o06_abstract_split"		+ m_sFlagWord;		//	�ض�ժҪ�����ַ���
}


//	���������ʽ
BOOL CCvtCNKIDlg::SetOutputStyleKeyword()
{
	//	EndNote ��ʽ
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

	m_pmap_OutputStyle_Keyword[0].insert( make_pair(m_space,		" ") );				//	a.	���������ǩ��ֵ��ļ������
	m_pmap_OutputStyle_Keyword[0].insert( make_pair(m_endofrefer,		" ") );			//	b.	���һ���ʾ�����ı�־

	//	RiS ��ʽ
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

	m_pmap_OutputStyle_Keyword[1].insert( make_pair(m_space,			"  - ") );		//	a.	���������ǩ��ֵ��ļ������
	m_pmap_OutputStyle_Keyword[1].insert( make_pair(m_endofrefer,		"ER") );		//	b.	���һ���ʾ�����ı�־

	//	BibTeX ��ʽ
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

	m_pmap_OutputStyle_Keyword[2].insert( make_pair(m_space,		" = ") );				//	a.	���������ǩ��ֵ��ļ������
	m_pmap_OutputStyle_Keyword[2].insert( make_pair(m_endofrefer,		"") );			//	b.	���һ���ʾ�����ı�־

	return TRUE;
}
