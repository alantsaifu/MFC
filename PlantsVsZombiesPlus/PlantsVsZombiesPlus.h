
// PlantsVsZombiesPlus.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPlantsVsZombiesPlusApp:
// �йش����ʵ�֣������ PlantsVsZombiesPlus.cpp
//

class CPlantsVsZombiesPlusApp : public CWinApp
{
public:
	CPlantsVsZombiesPlusApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPlantsVsZombiesPlusApp theApp;