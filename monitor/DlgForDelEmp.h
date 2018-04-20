#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;

class DlgForDelEmp :public WindowImplBase
{
public:
	DlgForDelEmp( HWND hWnd );

	~DlgForDelEmp(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMeterMsgBox");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForDelEmp.xml");  }

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

