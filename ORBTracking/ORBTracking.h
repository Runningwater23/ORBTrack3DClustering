
// ORBTracking.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CORBTrackingApp: 
// �йش����ʵ�֣������ ORBTracking.cpp
//

class CORBTrackingApp : public CWinApp
{
public:
	CORBTrackingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CORBTrackingApp theApp;