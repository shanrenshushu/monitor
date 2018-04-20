#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;

class DlgForDelCom :public WindowImplBase
{
public:
	DlgForDelCom( HWND hWnd );

	~DlgForDelCom(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMeterMsgBox");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForDelCom.xml");  }

	CDuiString GetSkinFolder() { return _T("");}

	void InitWindow();

	void OnFinalMessage( HWND hWnd );
private:
	void OnBtnDel(TNotifyUI& msg);
	void Initlist();
	
private:
	HWND      m_ParentHwnd;
	DuiLib::CListUI *mpListView;

public:

	DUI_DECLARE_MESSAGE_MAP()
};

