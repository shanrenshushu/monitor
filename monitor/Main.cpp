//                            _ooOoo_  
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
//                  ���汣��             ����BUG 
//			............................................
#include "stdafx.h"
//#include "YZGMainFrame.h"
#include "MainWnd.h"
#include "Manager.h"
#include "GsmModel.h"

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

 
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//if (!CheckSingleApplication())
	//{
	//	LogInfo(L"ʵ���Ѿ�����");
	//	return 0;
	//}
	LogInfo(L"����");

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
	SetInfo.name=L"���ü������";
	SetInfo.com=L"com2";
	SetInfo.baud=4800;
	SetInfo.type=L"С���豸";
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
	//g_GsmModel.StartGsm( 99/*���ں�*/ );  
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

	MainWnd * mainwnd= new MainWnd();
	mainwnd->Create(NULL, _T("�񶯼��ϵͳ"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	mainwnd->CenterWindow();
	mainwnd->ShowWindow();
#endif	
	CPaintManagerUI::MessageLoop();
	LogInfo(L"MessageLoop�˳���");



	///�������˳�������
	Manager::ReleaseInstance();
	ReleaseMutex(mMutexHandle);
	//ApiHelper::ExitSQLiteDB();
	::OleUninitialize();
	LogInfo(L"===============�˳�================\n");
	return 1;
}
