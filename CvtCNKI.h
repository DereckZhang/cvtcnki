// CvtCNKI.h : PROJECT_NAME 应用程序的主头文件
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
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCvtCNKIApp:
// 有关此类的实现，请参阅 CvtCNKI.cpp
//

class CCvtCNKIApp : public CWinApp
{
public:
	CCvtCNKIApp();


public:
// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCvtCNKIApp theApp;