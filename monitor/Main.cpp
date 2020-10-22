﻿//                            _ooOoo_  
//                           o8888888o  
//                           88" . "88  
//                           (| -_- |)  
//                            O\ = /O  
//                        ____/`---'\____  
//                      .   ' \\| |// `.  
//                       / \\||| : |||// \  
//                     / _||||| -:- |||||- \  
//                       | | \\\ - /// | |  
//                     | \_| ''\---/'' | |  
//                      \ .-\__ `-` ___/-. /  
//                   ___`. .' /--.--\ `. . __  
//                ."" '< `.___\_<|>_/___.' >'"".  
//               | | : `- \`.;`\ _ /`;.`/ - ` : | |  
//                 \ \ `-. \_ __\ /__ _/ .-` / /  
//         ======`-.____`-.___\_____/___.-`____.-'======  
//                            `=---='   
//         .............................................  
//                  佛祖保佑             永无BUG 
//			............................................
#include "stdafx.h"
//#include "YZGMainFrame.h"
#include "MainWnd.h"
#include "Manager.h"
#include "GsmModel.h"
#pragma comment (lib, "Msimg32")
GsmModel              g_GsmModel;


HANDLE mMutexHandle = NULL;
bool CheckSingleApplication()
{
	mMutexHandle = ::CreateMutex(NULL, TRUE, ApplicationWndClassName);
	unsigned long err = ::GetLastError();
	if (err != ERROR_SUCCESS)
	{
		return false;
	}
	if (::FindWindow(ApplicationWndClassName, NULL))
	{
		return false;
	}
	return true;
}

void SetIcon(HWND hWnd)
{
	wstring TitleIcoPath = CPaintManagerUI::GetInstancePath()+L"ico.ico";
	wstring FormIcoPath = CPaintManagerUI::GetInstancePath()+L"ico.ico";

	//HICON hIconTitle = static_cast<HICON>(::LoadImage(NULL, TitleIcoPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	//if (hIconTitle)
	{
		//::SendMessage(hWnd, WM_SETICON, FALSE, reinterpret_cast<LPARAM>(hIconTitle));
	}
	
	HICON hIconForm = static_cast<HICON>(::LoadImage(NULL, FormIcoPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	if (hIconForm)
	{
		::SendMessage(hWnd, WM_SETICON, false, reinterpret_cast<LPARAM>(hIconForm));
	}
	
}

#pragma comment (lib, "Msimg32")
int Testmain()
{
	HDC lWndDC= GetWindowDC(NULL);
	RECT  lrtRect;
	lrtRect.left = 0;
	lrtRect.top = 0;
	lrtRect.right = 500;
	lrtRect.bottom = 500;
	COLORREF col_from = RGB(255, 0,0);
	COLORREF col_to = RGB(0, 255,0);
	COLORREF col_a = RGB(0, 0,255);

	BOOL vert_grad=true;
	TRIVERTEX        vert[3];
	GRADIENT_RECT    mesh;
	mesh.UpperLeft  = 0;
	mesh.LowerRight = 1;
	/*_GRADIENT_TRIANGLE mesh;
	mesh.Vertex1  = 0;
	mesh.Vertex2 = 1;
	mesh.Vertex3 = 2;*/
	vert[0].x      = lrtRect.left;
	vert[0].y      = lrtRect.top;
	vert[0].Alpha  = 0x0000;
	vert[0].Blue   = GetBValue(col_from)<<8;
	vert[0].Green  = GetGValue(col_from)<<8;
	vert[0].Red    = GetRValue(col_from)<<8;
	vert[1].x      = lrtRect.right;
	vert[1].y      = lrtRect.bottom; 
	vert[1].Alpha  = 0x0000;
	vert[1].Blue   = GetBValue(col_to) <<8;
	vert[1].Green  = GetGValue(col_to) <<8;
	vert[1].Red    = GetRValue(col_to) <<8;

	vert[2].x      = lrtRect.left;
	vert[2].y      = lrtRect.bottom; 
	vert[2].Alpha  = 0x0000;
	vert[2].Blue   = GetBValue(col_a) <<8;
	vert[2].Green  = GetGValue(col_a) <<8;
	vert[2].Red    = GetRValue(col_a) <<8;
	

	GradientFill( lWndDC, vert, 2, &mesh, 1, GRADIENT_FILL_RECT_H);

	DeleteDC(lWndDC);

	Sleep(2000);

	return 0;
}

#include "shadow.h"
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	return Testmain();
	//if (!CheckSingleApplication())
	//{
	//	LogInfo(L"实例已经运行");
	//	return 0;
	//}
	LogInfo(L"启动");

	//ApiHelper::InitSQLiteDB();

	::OleInitialize(NULL);
	CPaintManagerUI::SetInstance(hInstance);
	CDuiString path(CPaintManagerUI::GetInstancePath() + _T("skin\\"));
	CPaintManagerUI::SetResourcePath(path);
	Manager::CreateInstance();
	MAN->CheckTable();
	int id = MAN->GetDeviceDB()->GetMaxDeviceID();
	
	unsigned char Xbuf[8]={0x01,0x03,0x09,0x92,0x00,0x0A,0x67,0xBC};
	bool b = CRC_Check(Xbuf,8);
	AUTOCRC16(Xbuf,8);

	/*sSettingInfo SetInfo;
	SetInfo.deviceID = 3;
	SetInfo.createTime = Utf2U(GetCurTime());
	SetInfo.createTimeNum = time(0);
	SetInfo.name=L"看得见法理的";
	SetInfo.com=L"com2";
	SetInfo.baud=4800;
	SetInfo.type=L"小型设备";
	SetInfo.XWarning=1.02;
	SetInfo.ZWarning=0.25;
	SetInfo.TWarning = 12.12;*/

	//MAN->GetDbMysql()->InsertRecord(SetInfo);
	//vector<sSettingInfo> DbMysql = MAN->GetDbMysql()->QueryRecord("select * from table1");
	/*vector<int> id = MAN->GetSettingDB()->GetAllDeviceID();
	for (int i=0;i<id.size();i++)
	{
	sSettingInfo s = MAN->GetSettingDB()->GetRecordByDeviceID(id[i]);
	}*/
	

	//Gsm
	//g_GsmModel.StartGsm( 99/*串口号*/ );  
	//g_GsmModel.InsertOneMsg( Tool::WStringToString(vecAllPhone[i]),Tool::WStringToString(wstrAlarmInfo) );

#if 0
	CYZGMainFrameWin *pMainFrameWin = new CYZGMainFrameWin();
	if(pMainFrameWin == NULL) return 0;
		
	//HWND hwnd = pMainFrameWin->Create(0, _T(""),WS_OVERLAPPEDWINDOW,0);// WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX) & ~(WS_SIZEBOX)
	HWND hwnd = pMainFrameWin->Create(0, _T(""),UI_WNDSTYLE_FRAME,0);
	pMainFrameWin->CenterWindow();
	//pMainFrameWin->ShowWindow();
	SetIcon(hwnd);
	::ShowWindow(hwnd, SW_MAXIMIZE);
	::SetForegroundWindow(hwnd);
	::UpdateWindow(hwnd);
#else	

	CWndShadow::Initialize(hInstance);
	MainWnd * mainwnd= new MainWnd();
	mainwnd->Create(NULL, _T("振动检测系统"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	mainwnd->CenterWindow();
	mainwnd->ShowWindow();
#endif	
	CPaintManagerUI::MessageLoop();
	LogInfo(L"MessageLoop退出！");



	///以下是退出清理工作
	Manager::ReleaseInstance();
	ReleaseMutex(mMutexHandle);
	//ApiHelper::ExitSQLiteDB();
	::OleUninitialize();
	LogInfo(L"===============退出================\n");
	return 1;
}
