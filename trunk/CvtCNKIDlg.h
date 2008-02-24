// CvtCNKIDlg.h : ͷ�ļ�
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
#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;
using std::map;
using std::vector;

typedef std::vector<string> StrVector;
typedef std::multimap<string, string> StrStrMMap;
typedef std::multimap<string, StrVector> StrStrVectorMMap;
typedef std::map<string, bool> StrBoolMap;


// CCvtCNKIDlg �Ի���
class CCvtCNKIDlg : public CDialog
{
// ����
public:
	CCvtCNKIDlg(CWnd* pParent = NULL);	// ��׼���캯��


// �Ի�������
	enum { IDD = IDD_CVTCNKI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON			m_hIcon;
	CToolTipCtrl	m_tooltip;

	string		n00_refertype;			//		n00.��������-�Ѿ�����Ҫ�ڴ��ṩ
	string		n02_title;				//		n02.����
	string		n03_author;				//		n03.����
	string		n04_year;				//		n04.��
	string		n06_placepublished;		//		n06.�����
	string		n09_totalpages;			//		n09.ҳ��
	string		n11_date;				//		n11.����
	string		n12_revieweditem;		//		n12.������Ŀ
	string		n13_shorttitle;			//		n13.�̱���
	string		n14_resourcenum;		//		n14.������Դ��
	string		n15_call;				//		n15.�绰
	string		n16_accessnumber;		//		n16.��ȡ���.DOI
	string		n17_label;				//		n17.��ǩ
	string		n18_keyword;			//		n18.�ؼ���
	string		n19_abstract;			//		n19.ժҪ
	string		n20_note;				//		n20.ע��
	string		n21_researchnote;		//		n21.�о�ע��
	string		n22_url;				//		n22.��ַ
	string		n23_address;			//		n23.���ߵ�ַ
	string		n24_caption;			//		n24.caption
	string		n25_accessdate;			//		n25.��ȡ������
	string		n26_database;			//		n26.���ݿ�����
	string		n27_dbprovider;			//		n27.���ݿ��ṩ��
	string		n28_language;			//		n28.����
	string		n29_secondarytitle;		//		n29.�ڶ�����.�ڿ�����.ϵ.������
	string		n30_volume;				//		n30.��.ѧλ����
	string		n31_number;				//		n31.����.��
	string		n32_section;			//		n32.����,��,��ʼҳ
	string		n33_issn;				//		n33.issn/isbn
	string		n34_typeofwork;			//		n34.��������(ѧ��)
	string		n36_subsidiaryauthor;	//		n36.��Ҫ����
	string		n37_translatedauthor;	//		n37.������
	string		n38_originalpublication;//		n38.ԭ������
	string		n39_secondaryauthor;	//		n39.�ڶ�����
	string		n40_reprintedition;		//		n40.��ӡ���
	string		n41_publisher;			//		n41.������.��ѧ
	string		n42_numberofvolumes;	//		n42.����
	string		n43_tertiaryauthor;		//		n43.��������.��ʦ
	string		n44_tertiarytitle;		//		n44.��������
	string		n45_edition;			//		n45.�汾
	string		n46_alternatetitle;		//		n46.�滻����(��д)
	string		n47_translatedtitle;	//		n47.������Ŀ.Ӣ��ƪ��
	string		n48_linktopdf;			//		n48.pdf����
	string		n49_image;				//		n49.ͼ��
	string		n50_lastmodifieddate;	//		n50.����޸�����
	string		n51_custom1;			//		n51.�Զ�����1
	string		n52_custom2;			//		n52.�Զ�����2
	string		n53_custom3;			//		n53.�Զ�����3
	string		n54_custom4;			//		n54.�Զ�����4
	string		n55_custom5;			//		n55.�Զ�����5
	string		n56_custom6;			//		n56.�Զ�����6
	string		n57_custom7;			//		n57.�Զ�����7

	string		m_space, m_endofrefer;	//	������ͽ�����־��
	string		m_sFirstLibraryWord;	//	ÿ�����ݿ�ĵ�һ����ʾ��

	CString		m_CurrentPath;			//	�����������·��
	CString		m_UserSettingPath;		//	�û��趨��·��
	CString		m_PathName;				//	�������򿪵��ļ�·��
	CString		m_FileName;				//	�������򿪵��ļ�����������չ��
	CString		m_FileTitle;			//	�������򿪵��ļ���	
	CString		m_OutputFileName;		//	���ڱ������������ļ���	
	int			m_nReferenceFrom;		//	��¼�����Ǵ��ļ����ǴӼ��������
	int			m_nMaxInforChar;		//	ÿ����Ϣ����ַ���
	
	StrStrVectorMMap	*m_pmmap_ReferenceData;				//	��¼ָ��
	int			m_nReferenceNumber;
	StrStrMMap	m_mmap_DataBase_Keyword;				//	���ݿ�ؼ���map
	StrStrMMap	*m_pmap_OutputStyle_Keyword;			//	�����ʽmap
	StrStrMMap	m_mmap_Curent_OutputStyle_Keyword;		//	ʵ�ʲ��õ����map

	StrVector	m_Ris_ReferenceTypeVector;				//	Ris��ʽ�����׸�ʽ��ʾ�б�
	StrVector	m_EndNote_ReferenceTypeVector;			//	EndNote��ʽ�����׸�ʽ��ʾ�б�
	StrVector	m_BibTeX_ReferenceTypeVector;			//	BibTeX��ʽ�����׸�ʽ��ʾ�б�
	StrVector	m_String_ReferenceTypeVector;			//	���׸�ʽ��Ӧ��˵�������б�
	StrVector	m_Source_ReferenceTypeVector;			//	ת��ǰ�ĸ�ʽ������ǲ�ͬ���׸�ʽת������Ҫ���
	StrVector	m_Current_ReferenceTypeVector;			//	ʵ�ʲ��õĵ����׸�ʽ��ʾ�б�
	string		m_Current_ReferenceType;				//	ʵ�ʲ��õĵ����׸�ʽ
	int			m_iPosOutputStyle;						//	ʵ������ĵ����׸�ʽ - ��Ӧ������
	int			m_iReferenceType;						//	��������

	StrBoolMap	m_map_Item_State;						//	���ø����Ƿ���Ҫ�ָ�

	//	ѡ��
	string		o01_tooltip;							//	Tooltip״̬��ʾ��
	string		o02_doctor_thesis;						//	��ʿ���ı�ʾ��
	string		o03_master_thesis;						//	˶ʿ���ı�ʾ��
	string		o04_delete_word;						//	��Ҫɾ���Ĵʻ�ı�ʾ��
	string		o05_outputfile_ext;						//	��չ����ʾ��
	string		o06_abstract_split;						//	�ض�ժҪ�����ַ���
	StrStrMMap	m_mmap_Option;							//	ѡ��map

	

private:
	BOOL	InitializeSetting();			//	��ʼ������
	void	ClearData();					//	��ո�������
	void	SetFlagWord();					//	���ñ�־��
	BOOL	GetDataBaseKeyword();			//	���ļ���ȡ���ݿ�ؼ���
	void	ExtractDataBaseKeyword(std::string thestring);	//	��ȡ�ؼ���
	BOOL	SetOutputStyleKeyword();		//	��������ĸ�ʽ Ris �� EndNote �� BibTeX
	BOOL	SetItemState();					//	���ÿ���ÿ����Ŀ�Ƿ���Ҫ�����״̬

	BOOL	SetCurrentMap();				//	���ô˴�ʹ�õĿ�ؼ���

	BOOL	ReadData();						//	����¼�ļ�
	void	CreateTempFile();				//	������ʱ�ļ�
	BOOL	WriteData();					//	д��¼���������ļ�	
	void	ProcessData(string& thestring);	//	����¼���ݽ��д���
	BOOL	IsEnglishVersion(std::string thestring);	//	����ַ����ǲ��ǰ���Ӣ��
	char*	ChooseDirectory(char *szTitle);				//	����Ŀ¼
	string	ConvertUtf8ToGBK(std::string thestrUtf8);	//	��UTF8��GBK����ת����

	void	DeleteInvalidString(string& thestring, std::vector<std::string>& vResultString, string filter = ",; ");		//	ɾ����Ч���ַ�
	void	DeleteBlankStringofBeginAndEnd(string& thestring);					//	ɾ������β�Ŀհ��ַ��������Ʊ��
	void	DeleteNumberStringofBegin(string& thestring, string filter);		//	ɾ�����׵������ַ�
	void	DeleteNumberString(string& thestring, string filter);				//	ɾ�������ַ�

	void	AddToolTip();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnDestroy( );
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
public:
	afx_msg void OnBnClickedFile();
	afx_msg void OnBnClickedMemory();
	afx_msg void OnBnClickedConvert();

private:
	//	�Ի����ϵ��ĸ��༭���Ӧ�ı���
	CString		m_sPages;			//	���׵�ҳ��
	CString		m_sDatabaseName;	//	���ݿ������
	CString		m_sAnnotation;		//	�Զ����ע��
	CString		m_sLang;			//	����
	std::string m_sFlagWord;		//	��־�ʣ���������Ǹ��ؼ���
	afx_msg void OnBnClickedSetoutputdir();
	afx_msg void OnBnClickedOpenoutputfile();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedPopupmenu();
	afx_msg void OnHelpme();
	afx_msg void OnLicense();
	afx_msg void OnDbcfgutf8();
	afx_msg void OnDbcfgOpenplan();
	afx_msg void OnProjecthomepage();
	afx_msg void OnDownloadhomepage();
	afx_msg void OnServiceshomepage();
	afx_msg void OnBnClickedSponsor1();
	afx_msg void OnBnClickedSponsor2();
	afx_msg void OnNoteexpressWebsite();
	afx_msg void OnNoteexpressForum();
	afx_msg void OnNoteexpressCmc();
	afx_msg void OnNoteexpressCtc();
	afx_msg void OnNoteexpressTuangou();
	afx_msg void OnPaangoodWebsite();
	afx_msg void OnPaangoodForum();
	afx_msg void OnPaangoodWeblog();
	afx_msg void OnPaangoodAgency();
	afx_msg void OnBnClickedWebsite();
};
