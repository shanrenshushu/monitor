#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;
class MainWnd;

class DlgForDelDevice :public WindowImplBase
{
public:
	DlgForDelDevice( MainWnd* hWnd );

	~DlgForDelDevice(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMeterMsgBox");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForDelDevice.xml");  }

	CDuiString GetSkinFolder() { return _T("");}

	void InitWindow();

	void OnFinalMessage( HWND hWnd );
private:
	void OnBtnDel(TNotifyUI& msg);
	void Initlist();
private:
	MainWnd*      m_ParentHwnd;
	DuiLib::CListUI *mpListView;

public:

	DUI_DECLARE_MESSAGE_MAP()
};

