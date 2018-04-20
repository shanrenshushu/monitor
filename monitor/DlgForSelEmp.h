#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;

class DlgForSelEmp :public WindowImplBase
{
public:
	DlgForSelEmp( HWND hWnd );

	~DlgForSelEmp(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMeterMsgBox");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForSelEmp.xml");  }

	CDuiString GetSkinFolder() { return _T("");}

	void InitWindow();

	void OnFinalMessage( HWND hWnd );
private:
	void OnBtnSel(TNotifyUI& msg);
	void Initlist();
private:
	HWND      m_ParentHwnd;
	DuiLib::CListUI *mpListView;

public:

	DUI_DECLARE_MESSAGE_MAP()
};

