
#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;

class DlgForAddCom :public WindowImplBase
{
public:
	DlgForAddCom( HWND hWnd );

	~DlgForAddCom(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMeterMsgBox");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForAddCom.xml");  }

	CDuiString GetSkinFolder() { return _T("");}

	void InitWindow();

	void OnFinalMessage( HWND hWnd );
private:
	void OnBtnAdd(TNotifyUI& msg);
	
private:
	HWND      m_ParentHwnd;


public:

	DUI_DECLARE_MESSAGE_MAP()
};

