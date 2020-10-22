#pragma once
#ifdef _DEBUG
#pragma comment(lib, "DuiLib_d.lib")
#else
#pragma comment(lib, "DuiLib.lib")
#endif 

//class SerialDeviceAdmin ;
#include "SerialDeviceAdmin.h"
#include "../DuiLib/UIlib.h"
#include "Control.h"
#include "ChartUI.h"
using namespace DuiLib;
#include "shadow.h"

enum eAxis
{
	XAxis=1,
	ZAxis=2,
	TAxis=3
};

class CWndShadow;
class MainWnd:  public WindowImplBase
{
public:
	MainWnd();
	~MainWnd();
    virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMainFrame");  }
    virtual CDuiString GetSkinFile()                {   return _T("MainWnd.xml");  }
    virtual CDuiString GetSkinFolder()              {   return _T("");  }
	void InitWindow();
	void Init();
	void Notify(TNotifyUI& msg);

	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);  
	virtual LRESULT OnDestroy( UINT , WPARAM , LPARAM , BOOL& bHandled );
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//static bool mgExit;
	//static HWND mgHwnd;
	//static DWORD WINAPI RealTimeMonitoring(LPVOID pvoid);
	void AddDevice(const sDeviceInfo &info);
	void DelDevice(const sDeviceInfo &info);
private:
	CWndShadow m_WndShadow;
	CButtonUI* m_pBtnAddDevice;
	CButtonUI* m_pBtnDelDevice;
	CButtonUI* m_pBtnChangeDevice;

	CButtonUI* m_pBtnAddEmp;
	CButtonUI* m_pBtnDelEmp;
	CButtonUI* m_pBtnSelEmp;

	CButtonUI* m_pBtnAddCom;
	CButtonUI* m_pBtnDelCom;
	CButtonUI* m_pBtnSelCom;

	CButtonUI* m_pBtndetail;

	CCharTUI  *mpUI1;
	CCharTUI  *mpUI2;
	CCharTUI  *mpUI3;
	CCharTUI  *mpUI4;
	CCharTUI  *mpUI5;
	//CTreeViewUI *mpTreeView;
	CTabLayoutUI *mpClientTabLayout;
	DuiLib::CListUI *mpListView;
	CButtonUI* mpBtnCompareX;
	CButtonUI* mpBtnCompareZ;
	CButtonUI* mpBtnCompareT;
	CButtonUI* mpBtnReturn;
	CDateTimeUI*	mpStartTime;
	CDateTimeUI*	mpEndTime;

	HANDLE *mHandleArr;
	int mSelectDeviceID;
	int mHandleNum;
	bool mInCurrent;
	time_t mTodayEnd;

	SerialDeviceAdmin mMAN;

	vector<wstring> lightArr;
	void RefreshListView(int state,const sRecordInfo &info);
	
	bool HandleEvent(void* param);//Ä¬ÈÏÏûÏ¢Ó³Éä

	void AddData( sRecordInfo *pData );
	void RefreshChartUI( vector<sRecordInfo*>& infoArr );
	void CompareChartUI( eAxis xzt );

	void InitDevice( vector<sDeviceInfo> ve );
	void InitChartUI();
	void InitPrimaryMenuPage();
	bool OnBtnPrimaryMenu( void *param );
	void ChangePage(int page);

	void OnBtnHelp(TNotifyUI& msg);

	void ShowDlgForChangeDevice();
	void ShowDlgForAddDevice();
	void ShowDlgForDelDevice();

	void ShowDlgForAddEmp();
	void ShowDlgForDelEmp();
	void ShowDlgForSelEmp();

	void ShowDlgForAddCom();
	void ShowDlgForDelCom();
	

	DUI_DECLARE_MESSAGE_MAP()
};