// CvtCNKIDlg.h : 头文件
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


// CCvtCNKIDlg 对话框
class CCvtCNKIDlg : public CDialog
{
// 构造
public:
	CCvtCNKIDlg(CWnd* pParent = NULL);	// 标准构造函数


// 对话框数据
	enum { IDD = IDD_CVTCNKI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON			m_hIcon;
	CToolTipCtrl	m_tooltip;

	string		n00_refertype;			//		n00.文献类型-已经不需要在此提供
	string		n02_title;				//		n02.标题
	string		n03_author;				//		n03.作者
	string		n04_year;				//		n04.年
	string		n06_placepublished;		//		n06.出版地
	string		n09_totalpages;			//		n09.页数
	string		n11_date;				//		n11.日期
	string		n12_revieweditem;		//		n12.评价条目
	string		n13_shorttitle;			//		n13.短标题
	string		n14_resourcenum;		//		n14.电子资源号
	string		n15_call;				//		n15.电话
	string		n16_accessnumber;		//		n16.获取序号.DOI
	string		n17_label;				//		n17.标签
	string		n18_keyword;			//		n18.关键词
	string		n19_abstract;			//		n19.摘要
	string		n20_note;				//		n20.注释
	string		n21_researchnote;		//		n21.研究注释
	string		n22_url;				//		n22.网址
	string		n23_address;			//		n23.作者地址
	string		n24_caption;			//		n24.caption
	string		n25_accessdate;			//		n25.获取的日期
	string		n26_database;			//		n26.数据库名字
	string		n27_dbprovider;			//		n27.数据库提供商
	string		n28_language;			//		n28.语言
	string		n29_secondarytitle;		//		n29.第二标题.期刊名称.系.从书名
	string		n30_volume;				//		n30.卷.学位级别
	string		n31_number;				//		n31.数字.期
	string		n32_section;			//		n32.部分,项,开始页
	string		n33_issn;				//		n33.issn/isbn
	string		n34_typeofwork;			//		n34.著作种类(学科)
	string		n36_subsidiaryauthor;	//		n36.次要作者
	string		n37_translatedauthor;	//		n37.翻译者
	string		n38_originalpublication;//		n38.原出版社
	string		n39_secondaryauthor;	//		n39.第二作者
	string		n40_reprintedition;		//		n40.重印版次
	string		n41_publisher;			//		n41.出版社.大学
	string		n42_numberofvolumes;	//		n42.卷数
	string		n43_tertiaryauthor;		//		n43.第三作者.导师
	string		n44_tertiarytitle;		//		n44.第四作者
	string		n45_edition;			//		n45.版本
	string		n46_alternatetitle;		//		n46.替换标题(缩写)
	string		n47_translatedtitle;	//		n47.翻译题目.英文篇名
	string		n48_linktopdf;			//		n48.pdf链接
	string		n49_image;				//		n49.图像
	string		n50_lastmodifieddate;	//		n50.最后修改日期
	string		n51_custom1;			//		n51.自定义项1
	string		n52_custom2;			//		n52.自定义项2
	string		n53_custom3;			//		n53.自定义项3
	string		n54_custom4;			//		n54.自定义项4
	string		n55_custom5;			//		n55.自定义项5
	string		n56_custom6;			//		n56.自定义项6
	string		n57_custom7;			//		n57.自定义项7

	string		m_space, m_endofrefer;	//	间隔符和结束标志符
	string		m_sFirstLibraryWord;	//	每个数据库的第一个标示词

	CString		m_CurrentPath;			//	保存程序所在路径
	CString		m_UserSettingPath;		//	用户设定的路径
	CString		m_PathName;				//	保存所打开的文件路径
	CString		m_FileName;				//	保存所打开的文件名，包括扩展名
	CString		m_FileTitle;			//	保存所打开的文件名	
	CString		m_OutputFileName;		//	用于保存输出结果的文件名	
	int			m_nReferenceFrom;		//	记录文献是从文件还是从剪贴板汇入
	int			m_nMaxInforChar;		//	每个信息最长的字符数
	
	StrStrVectorMMap	*m_pmmap_ReferenceData;				//	题录指针
	int			m_nReferenceNumber;
	StrStrMMap	m_mmap_DataBase_Keyword;				//	数据库关键词map
	StrStrMMap	*m_pmap_OutputStyle_Keyword;			//	输出格式map
	StrStrMMap	m_mmap_Curent_OutputStyle_Keyword;		//	实际采用的输出map

	StrVector	m_Ris_ReferenceTypeVector;				//	Ris格式的文献格式表示列表
	StrVector	m_EndNote_ReferenceTypeVector;			//	EndNote格式的文献格式表示列表
	StrVector	m_BibTeX_ReferenceTypeVector;			//	BibTeX格式的文献格式表示列表
	StrVector	m_String_ReferenceTypeVector;			//	文献格式对应的说明文字列表
	StrVector	m_Source_ReferenceTypeVector;			//	转换前的格式，如果是不同文献格式转换，需要这个
	StrVector	m_Current_ReferenceTypeVector;			//	实际采用的的文献格式表示列表
	string		m_Current_ReferenceType;				//	实际采用的的文献格式
	int			m_iPosOutputStyle;						//	实际输出的的文献格式 - 对应的数字
	int			m_iReferenceType;						//	文献类型

	StrBoolMap	m_map_Item_State;						//	设置各项是否需要分割

	//	选项
	string		o01_tooltip;							//	Tooltip状态表示词
	string		o02_doctor_thesis;						//	博士论文标示词
	string		o03_master_thesis;						//	硕士论文标示词
	string		o04_delete_word;						//	需要删除的词汇的标示词
	string		o05_outputfile_ext;						//	扩展名表示词
	string		o06_abstract_split;						//	截断摘要保留字符数
	StrStrMMap	m_mmap_Option;							//	选项map

	

private:
	BOOL	InitializeSetting();			//	初始化设置
	void	ClearData();					//	清空各项数据
	void	SetFlagWord();					//	设置标志词
	BOOL	GetDataBaseKeyword();			//	从文件读取数据库关键词
	void	ExtractDataBaseKeyword(std::string thestring);	//	提取关键词
	BOOL	SetOutputStyleKeyword();		//	设置输出的格式 Ris 或 EndNote 或 BibTeX
	BOOL	SetItemState();					//	设置库内每个条目是否需要处理的状态

	BOOL	SetCurrentMap();				//	设置此处使用的库关键词

	BOOL	ReadData();						//	读题录文件
	void	CreateTempFile();				//	创建临时文件
	BOOL	WriteData();					//	写题录处理结果到文件	
	void	ProcessData(string& thestring);	//	对题录数据进行处理
	BOOL	IsEnglishVersion(std::string thestring);	//	检查字符串是不是包含英文
	char*	ChooseDirectory(char *szTitle);				//	设置目录
	string	ConvertUtf8ToGBK(std::string thestrUtf8);	//	从UTF8到GBK进行转换。

	void	DeleteInvalidString(string& thestring, std::vector<std::string>& vResultString, string filter = ",; ");		//	删除无效的字符
	void	DeleteBlankStringofBeginAndEnd(string& thestring);					//	删除行首尾的空白字符，包括制表符
	void	DeleteNumberStringofBegin(string& thestring, string filter);		//	删除行首的数字字符
	void	DeleteNumberString(string& thestring, string filter);				//	删除数字字符

	void	AddToolTip();

	// 生成的消息映射函数
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
	//	对话框上的四个编辑框对应的变量
	CString		m_sPages;			//	文献的页码
	CString		m_sDatabaseName;	//	数据库的名字
	CString		m_sAnnotation;		//	自定义的注释
	CString		m_sLang;			//	语言
	std::string m_sFlagWord;		//	标志词，表明这个是个关键词
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
