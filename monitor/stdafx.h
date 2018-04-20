// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#include <thread>
#include <mutex>
#include <functional>
#include <winsock2.h>
#include <mysql.h> //������ͷ�ļ���һЩ��Ҫ�ṹ�������  
#include <errmsg.h>//������Ϣ�Ĵ��  
#include <mysql_version.h>  //������ǰmysql�İ汾��Ϣ  

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <objbase.h>
#include <vector>
#include <map>
#include <commctrl.h>
#include <shlwapi.h>
#include <list>
#include <string>

 

#include "UIlib.h"
using namespace DuiLib;
#include "Logger.h"
/*#include "UtilityLib.h"
#include "ClientHttpLib.h"*/
//#include "ApiHelperLib.h"
//#include "DataSynchLib.h"

#include "..\Cchart\Plot\PlotInterface.h"


#include "Manager.h"

using namespace std;
#ifndef NO_USING_DUILIB_NAMESPACE
using namespace DuiLib;
#endif



////////////////////
#define ApplicationWndClassName _T("YZG40_MainWindow")

#define SafeDelete(name) if(name){delete name;name=NULL;}
template<typename T>
void SafeDeleteVector(T& V)
{
	for (T::iterator i = V.begin(); i!=V.end();++i)
		SafeDelete(*i);
	V.clear();
}

//
#define GetControlByName(classname,pControl,controlname) \
	pControl = static_cast<classname *>(m_PaintManager.FindControl(controlname)); \
	ASSERT(pControl); 


////////////////
#define STRMAXLONG 1024
int ConvertToUnicode(const char* str,wchar_t* out,UINT codepage = CP_ACP);
int ConvertToAscii(const wchar_t* uniStr, char* out,UINT codepage = CP_ACP);

string WStringToString(const wstring &str);
wstring StringToWString(const string &str) ;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#define WM_CUSTOM_CHANGEUESR		WM_USER+100
#define WM_RECVDATA_MSG				WM_USER+101
