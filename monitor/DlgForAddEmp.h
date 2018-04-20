
#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;

class DlgForAddEmp :public WindowImplBase
{
public:
	DlgForAddEmp( HWND hWnd );

	~DlgForAddEmp(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMeterMsgBox");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForAddEmp.xml");  }

	CDuiString GetSkinFolder() { return _T("");}

	void InitWindow();

	void OnFinalMessage( HWND hWnd );
private:
	void OnBtnAdd(TNotifyUI& msg);
private:
	HWND      m_ParentHwnd;
	CEditUI* mpJobnum;
	CEditUI* mpName;
	CEditUI* mpPhone;

public:

	DUI_DECLARE_MESSAGE_MAP()
};

