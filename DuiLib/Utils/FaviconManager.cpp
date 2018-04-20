#include "stdafx.h"
#include <queue>
#include <winhttp.h>
#include "FaviconManager.h"
#include "UIDelegate.h"
#include "Core/UIManager.h"
#include "Utils/UIDelegate.h"
#include "UIlib.h"
#pragma comment(lib,"Winhttp.lib")

using namespace DuiLib;

cFaviconManager* cFaviconManager::Instance = NULL;
cFaviconManager::cFaviconManager(void)
	:mShareHandle(NULL)
	,mIsExit(false)
{
	if (!Instance)
	{
		Instance = this;
		InitializeCriticalSection(&mCs);//EnterCriticalSection(&mCs);LeaveCriticalSection(&mCs);
	}
	else
	{
		ASSERT(0);
	}
}

cFaviconManager::~cFaviconManager(void)
{
	Exit();
	DeleteCriticalSection(&mCs);
}

void cFaviconManager::Begin()
{
	DWORD threadid;
	mShareHandle=CreateThread(NULL,0,run,(LPVOID)this,0,&threadid);
}
bool cFaviconManager::Exit()
{
	mIsExit = true;
	if (WaitForSingleObject(mShareHandle, 1000) != WAIT_OBJECT_0)
	{
		DWORD exitCode;
		GetExitCodeThread(mShareHandle, &exitCode);
		if (exitCode == STILL_ACTIVE)
		{
			TerminateThread(mShareHandle, 0);
		}
	}
	CloseHandle(mShareHandle);
	mShareHandle=0;
	return true;
}


void cFaviconManager::End()
{
	CloseHandle(mShareHandle);
	mShareHandle=0;
}


bool cFaviconManager::IsEmpty()
{
	return mTaskQueue.empty();
}

void cFaviconManager::PushTask( sFaviconTask &task )
{
	EnterCriticalSection(&mCs);
	mTaskQueue.push(task);
	LeaveCriticalSection(&mCs);

	if (!mShareHandle)
	{
		Begin();
	}
}

bool cFaviconManager::GetTask( sFaviconTask &task )
{
	bool success = false;
	EnterCriticalSection(&mCs);
	if (!mTaskQueue.empty())
	{
		task = mTaskQueue.front();  
		mTaskQueue.pop(); 
		if (*task.isDel == 1)
		{
			success = true;
		}
	}
	LeaveCriticalSection(&mCs);
	return success;
}

DWORD WINAPI cFaviconManager::run( LPVOID param )
{
	cFaviconManager* pFavMan = (cFaviconManager*)param;
	bool IsFirst = true;
	while(!pFavMan->mIsExit && !pFavMan->IsEmpty())//空的话就结束线程
	{
		sFaviconTask task;
		if (pFavMan->GetTask(task))
		{
			pFavMan->FaviconTaskRun(task);//下载
			pFavMan->Invok(task);//回调
		}

		Sleep(50);
	}
	pFavMan->End();
	return 0;
}

void cFaviconManager::Invok( sFaviconTask &task )
{
	EnterCriticalSection(&mCs);
	try
	{
		if (*task.isDel  == 1)//如果是1说明还没销毁，其他值都认为销毁
		{
			if (task.onEvent && *task.onEvent)
			{
				(*task.onEvent)(const_cast<wchar_t *>(task.path.c_str()));
			}
		}
	}
	catch(...)
	{

	}
	LeaveCriticalSection(&mCs);
	
}


bool cFaviconManager::FaviconTaskRun( sFaviconTask &task )
{
#if 0
	EnterCriticalSection(&mCs);
	URL_COMPONENTS urlCom;
	memset(&urlCom, 0, sizeof(urlCom));
	urlCom.dwStructSize = sizeof(urlCom);
	WCHAR wszScheme[64] = {0};
	urlCom.lpszScheme = wszScheme;
	urlCom.dwSchemeLength = ARRAYSIZE(wszScheme);
	WCHAR wszHostName[1024] = {0};
	urlCom.lpszHostName = wszHostName;
	urlCom.dwHostNameLength = ARRAYSIZE(wszHostName);
	WCHAR wszUrlPath[1024] = {0};
	urlCom.lpszUrlPath = wszUrlPath;
	urlCom.dwUrlPathLength = ARRAYSIZE(wszUrlPath);
	WCHAR wszExtraInfo[1024] = {0};
	urlCom.lpszExtraInfo = wszExtraInfo;
	urlCom.dwExtraInfoLength = ARRAYSIZE(wszExtraInfo);

	wstring downUrl = task.url;
	if (string::npos == downUrl.find(L"http://"))
	{
		if (string::npos == downUrl.find(L"https://"))
		{
			wstring str = L"http://";//默认使用http
			if (string::npos != downUrl.find(L"//"))
			{
				str= L"http:";
			}
			downUrl = str + downUrl;
		}
	}
	if ( FALSE == WinHttpCrackUrl(downUrl.c_str(), downUrl.length(), ICU_ESCAPE, &urlCom) ) 
	{
		//ASSERT(0);
		LeaveCriticalSection(&mCs);
		return false;
	}
	

	if (task.directDown)
	{
		task.path = L"ResIcon\\";
		task.path += urlCom.lpszHostName;
		
		int pos;
		if ((pos=downUrl.find_last_of(L".")) != wstring::npos)
		{
			wstring ex= downUrl.substr(pos,downUrl.length()-pos);
			if (ex == L".png" ||ex == L".ico" || ex == L".bmp"|| ex == L".jpeg" )
			{
				task.path += ex;
			}
			else
			{
				LeaveCriticalSection(&mCs);
				return false;
			}
		}
		else
		{
			LeaveCriticalSection(&mCs);
			return false;
		}
	}
	else
	{
		downUrl = urlCom.lpszScheme;
		downUrl += L"://";
		wstring host = urlCom.lpszHostName;
		int pos;
		if ((pos=host.find_first_of(L"?")) != wstring::npos)
		{
			host= host.substr(0,pos);
		}
		downUrl += host;
		downUrl += L"/favicon.ico";

		task.path = L"ResIcon\\";
		task.path += host;
		task.path += L".ico";
	}

	wstring filePath = DuiLib::CPaintManagerUI::GetInstancePath().GetData();
	filePath += task.path;
	if (_waccess(filePath.c_str(), 0) == 0)
	{
		LeaveCriticalSection(&mCs);
		return true;
	}
	LeaveCriticalSection(&mCs);

	//CWinHttpClient winhttp(downUrl);
	//winhttp.SetTimeOut(3000,3000,3000,4000);
	//winhttp.SetDownloadPath(filePath);
	//winhttp.SetAgent("User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
	//winhttp.AddHeader("Accept","*/*");
	//winhttp.SetHeader("Accept:application/x-ms-application, image/jpeg, application/xaml+xml, image/gif, image/pjpeg,*/*");
	//string str;
	//winhttp.GetReturnData(str);

	bool isimg=false;
	char *outstr = NULL;
	do 
	{
		if (winhttp.IsSuccessful() == TRUE)
		{
			long l = WideCharToMultiByte(CP_ACP, 0, filePath.c_str(), -1, NULL, 0, NULL, 0);
			outstr = new char[l];
			memset(outstr,0,l*sizeof(char));
			WideCharToMultiByte(CP_ACP, 0, filePath.c_str(), -1, outstr, l, NULL, 0);
			outstr[l - 1] = 0;

			FILE* pInfile;
			if (fopen_s(&pInfile, outstr, "rb") == 0)
			{
				fseek(pInfile, 0, SEEK_END);
				int mSize = ftell(pInfile);
				if (mSize == 0)
				{
					fclose(pInfile);
					break;
				}
				if (mSize >10)
				{
					mSize=10;
				}
				fseek(pInfile, 0, SEEK_SET);
				char *buf = new char[mSize+1];
				memset(buf,0,mSize+1);
				fread(buf, 1, mSize, pInfile);
				buf[mSize]=0;

				const char *p = buf;
				for (int i=0;i<mSize;i++)
				{
					if ( *p=='\r' || *p=='\n' || *p==' '|| (*p>=33 && *p<=126))
					{
						isimg = false;
					}
					else
					{
						isimg = true;
						break;
					}
				}
				delete []buf;
				fclose(pInfile);
			}
		}
		
	} while (0);
	if (outstr)
	{
		delete[] outstr;
		outstr = NULL;
	}
	
	if (!isimg)
	{
		DeleteFile(filePath.c_str());
		task.path=L"";
		return false;
	}
	//return winhttp.IsSuccessful() == TRUE;
#endif
	return true;
}


