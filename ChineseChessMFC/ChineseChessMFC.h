
// ChineseChessMFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChineseChessMFCApp:
// �йش����ʵ�֣������ ChineseChessMFC.cpp
//

class CChineseChessMFCApp : public CWinApp
{
public:
	CChineseChessMFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChineseChessMFCApp theApp;