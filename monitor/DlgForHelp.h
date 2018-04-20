#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;

class DlgForHelp:public WindowImplBase
{
public:
	DlgForHelp( HWND hWnd );

	~DlgForHelp(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DlgForHelp");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForHelp.xml");  }

	CDuiString GetSkinFolder() { return _T("");}

	void InitWindow();

	void OnFinalMessage( HWND hWnd );
private:
	void OnBtnHelpOk(TNotifyUI& msg);
private:
	HWND      m_ParentHwnd;

public:

	DUI_DECLARE_MESSAGE_MAP()
};

