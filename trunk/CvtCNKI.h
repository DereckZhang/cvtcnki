// CvtCNKI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
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

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCvtCNKIApp:
// �йش����ʵ�֣������ CvtCNKI.cpp
//

class CCvtCNKIApp : public CWinApp
{
public:
	CCvtCNKIApp();


public:
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCvtCNKIApp theApp;