// CvtCNKI.cpp : ����Ӧ�ó��������Ϊ��
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCvtCNKIApp

BEGIN_MESSAGE_MAP(CCvtCNKIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCvtCNKIApp ����

CCvtCNKIApp::CCvtCNKIApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCvtCNKIApp ����

CCvtCNKIApp theApp;


// CCvtCNKIApp ��ʼ��

BOOL CCvtCNKIApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CCvtCNKIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
