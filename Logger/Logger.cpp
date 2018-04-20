#include "stdafx.h"
#include "Logger.h"
#include <time.h>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


wstring Logger::GetCurrWorkingDir() {
	wstring strPath;
	TCHAR szFull[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	::GetModuleFileName(NULL, szFull, sizeof(szFull) / sizeof(TCHAR));
	_tsplitpath(szFull, szDrive, szDir, NULL, NULL);
	//_wsplitpath_s(szFull,szDrive,_MAX_DIR,szDir,_MAX_DIR,NULL,0,NULL,0);
	//_tcscpy(szFull, szDrive);
	wcscpy_s(szFull,szDrive);
	//_tcscat(szFull, szDir);
	_tcscat_s(szFull, szDir);
	strPath = wstring(szFull);
	if(strPath.at(strPath.length()-1)!='\\')
		strPath.append(L"\\");
	return strPath;
}
void  Logger::WriteLog(LPCTSTR path,LPCTSTR content,eLoggerType type = eInfo)
{
	try
	{
		wstring currentpath;
		if (path)
		{
			currentpath = path;
			if (currentpath.empty())
			{
				currentpath = GetCurrWorkingDir();
			}
		}
		else
		{
			currentpath = GetCurrWorkingDir();
		}

		currentpath.append(_T("Log\\"));
		_wmkdir(currentpath.c_str());

        time_t tt = time(NULL);
        tm *t = localtime(&tt);

        //wchar_t monthName[5];
        //wcsftime(monthName, 5, _T("%y%m"), t);
        //currentpath.append(monthName);
        //currentpath.append(_T("\\"));
        //_wmkdir(currentpath.c_str());

        //wchar_t dayName[3];
       // wcsftime(dayName, 3, _T("%d"), t);
		//currentpath.append(dayName);
		//currentpath.append(_T("\\"));
		//_wmkdir(currentpath.c_str());

		switch (type)
		{
		case Logger::eInfo:
			currentpath.append(_T("info.txt"));
			break;
		case Logger::eWarning:
			currentpath.append(_T("warning.txt"));
			break;
		case Logger::eError:
			currentpath.append(_T("error.txt"));
			break;
		default:
			break;
		}

        wchar_t date[20];
        wcsftime(date, 20, _T("%Y-%m-%d %H:%M:%S"), t);

		wofstream file;
		static bool isfrist = true;
		if (isfrist)
		{
			file.open(currentpath,ios::out|ios::ate);//ios::app
		}
		else
		{
			file.open(currentpath,ios::out|ios::app);
		}
		
		if(file.is_open())
		{
			file.imbue(std::locale("chs"));
			file<<date<<_T(" ")<<(content?content:_T(""))<<endl;
			file.close();
		}
		isfrist = false;
	}
	catch(...)
	{
    }
}


void inline Logger::Trace(LPCTSTR path,eLoggerType type,LPCTSTR pstrFormat, ...)
{
	if(pstrFormat==NULL) return;

	va_list args;
	va_start(args, pstrFormat);
	TCHAR* buf;
	int len = _vscwprintf(pstrFormat, args) + 1;
	buf = (TCHAR*)malloc(len * sizeof(TCHAR));
	vswprintf_s(buf, len, pstrFormat, args);
	va_end(args);
	WriteLog(path,buf,type);
	free(buf);
}

void Logger::Trace(LPCTSTR path,eLoggerType type, const char* fileName,size_t line,LPCTSTR pstrFormat, ... )
{
	if(pstrFormat==NULL) return;

	va_list args;
	va_start(args, pstrFormat);
	TCHAR* buf;
	TCHAR tmp[32]={0,};
	TCHAR *wfileName=0;
	wstring str;
	if (fileName)
	{
		size_t templen = MultiByteToWideChar(CP_ACP, 0, fileName, -1, NULL, 0);
		wfileName = (TCHAR*)malloc(templen * sizeof(TCHAR));
		MultiByteToWideChar(CP_ACP, 0, fileName, -1, &wfileName[0], templen);

		str = wfileName;
		str += L"(";
		swprintf(tmp,32,L"%d",line);
		str += tmp;
		str += L") :";
		free(wfileName);
	}
	size_t len = _vscwprintf(pstrFormat, args) +1 + str.length();
	buf = (TCHAR*)malloc(len * sizeof(TCHAR));
	
	memcpy(buf,str.c_str(),str.length()*sizeof(TCHAR));
	vswprintf_s(&buf[str.length()], len-str.length(), pstrFormat, args);
	va_end(args);
	WriteLog(path,buf,type);
	str.clear();
	free(buf);
}

///////////////////////////////////////////
void YZG_LOG DebugPrintA(const char* fileName,size_t line,const char* pfmt, ...)
{
	if (pfmt == 0)
		return;

	va_list vl;
	va_start(vl, pfmt);

	char* buf;
	int len = _vscprintf(pfmt, vl) + 1;
	buf = (char*)malloc(len * sizeof(char));
	vsprintf_s(buf, len, pfmt, vl);

	va_end(vl);

	char tmp[16];
	_itoa(line,tmp,10);
	OutputDebugStringA(fileName);
	OutputDebugStringA("(");
	OutputDebugStringA(tmp);
	OutputDebugStringA(") :");
	OutputDebugStringA(buf);
	OutputDebugStringA("\n");

	free(buf);
}

void YZG_LOG DebugPrintW(const char* fileName,size_t line,const wchar_t* pfmt, ...)
{
	if (pfmt == 0)
		return;

	va_list vl;
	va_start(vl, pfmt);

	wchar_t* buf;

	int len = _vscwprintf(pfmt, vl) + 1;
	buf = (wchar_t*)malloc(len * sizeof(wchar_t));
	vswprintf_s(buf, len, pfmt, vl);
	va_end(vl);

	char tmp[16];
	_itoa(line,tmp,10);
	OutputDebugStringA(fileName);
	OutputDebugStringA("(");
	OutputDebugStringA(tmp);
	OutputDebugStringA(") :");

	OutputDebugStringW(buf);
	OutputDebugStringW(L"\n");

	free(buf);
}

//////////////////////////////////
HWND g_hScannerWnd=NULL;
DWORD LastTime =0;
#define  PerTime  50
#define  MaxTime  300
extern HINSTANCE g_hin;

#pragma data_seg("ScannerBuf")
HHOOK g_hScannerKeyHook=NULL;
char g_ScannerBuf[32]={0};
bool g_HookEnable = true;
volatile int g_Index=0;
#pragma data_seg()
#pragma comment(linker,"/SECTION:ScannerBuf,RWS")


inline void ClearBuf()
{
	g_Index = 0;
	memset(g_ScannerBuf,0,sizeof(g_ScannerBuf));
}
inline void AddNumToBuf(int num)
{
	if (g_Index>=0 && g_Index<32)
	{
		//wchar_t buf[4]={0};
		//swprintf_s(&g_ScannerBuf[g_Index],32, L"%d", num);
		g_ScannerBuf[g_Index]=num;
		++g_Index;
		//DBG_PNTA(g_ScannerBuf);
	}
}
inline void SentBuf()
{
	if (strlen(g_ScannerBuf)<2)
	{
		ClearBuf();
	}
	else
	{
		SendMessage(g_hScannerWnd,WM_SCANNER_MESSAGE,0,(LPARAM)g_ScannerBuf);
	}
}

LRESULT CALLBACK ScannerKeyProc(
	int code,       // hook code
	WPARAM wParam,  // virtual-key code
	LPARAM lParam   // keystroke-message information
	)
{
	if (g_HookEnable && code == HC_ACTION && wParam == WM_KEYUP)
	{
		KBDLLHOOKSTRUCT* pStruct = (KBDLLHOOKSTRUCT*)lParam;
		char ch = pStruct->vkCode;
		if(ch == VK_F8)
		{
			SendMessage(g_hScannerWnd,WM_SCANNER_MESSAGE,0,(LPARAM)g_ScannerBuf);
			return 1;
		}
		if(ch == VK_F7)
			SendMessage(g_hScannerWnd,WM_SCANNER_MESSAGE,0,(LPARAM)0);
		//DBG_PNT(L"wParam=%d",ch);
		//if (0x30<=ch && ch <=0x39 || ch == VK_RETURN)//5E : reserved
		//{
		//	DWORD nowtime = timeGetTime();
		//	if (LastTime==0 || (nowtime >= LastTime && nowtime -LastTime>MaxTime))//第一次或者字符之间超过MaxTime认为重新输入
		//	{
		//		LastTime = nowtime;
		//		ClearBuf();
		//		AddNumToBuf(ch);
		//	}
		//	else if(nowtime >= LastTime && nowtime -LastTime<PerTime)//每个字符之间时间间隔为小于PerTime
		//	{
		//		LastTime = nowtime;
		//		if (ch != VK_RETURN)
		//		{
		//			AddNumToBuf(ch);
		//		}
		//		else
		//		{
		//			SentBuf();
		//			LastTime = nowtime + MaxTime;//MaxTime内不再接收
		//			ClearBuf();
		//		}
		//	}
		//}
		if (ch == VK_RETURN)
		{
			pStruct->vkCode = 0x5E;//0x5E : reserved
			pStruct->dwExtraInfo = 0x12345678;
		}
	}
	return CallNextHookEx(g_hScannerKeyHook, code, wParam, lParam); 
}

void YZG_LOG SetScannerHook(HWND hwnd)
{
	g_hScannerWnd=hwnd;
	g_hScannerKeyHook=SetWindowsHookEx(WH_KEYBOARD_LL,ScannerKeyProc,g_hin/*GetModuleHandle(L"Logger")*/,0);
}
void YZG_LOG SetHookEnable(bool enable)
{
	g_HookEnable = enable;
}

void YZG_LOG UnhookScanner()
{
	if (g_hScannerKeyHook)
	{
		UnhookWindowsHookEx(g_hScannerKeyHook);
		g_hScannerKeyHook = NULL;
	}
}

//////////////////////////////////