
#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;
#include "MainWnd.h"

class DlgForChangeDevice :public WindowImplBase
{
public:
	DlgForChangeDevice( MainWnd* paren ,int deviceid);

	~DlgForChangeDevice(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMeterMsgBox");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForChangeDevice.xml");  }

	CDuiString GetSkinFolder() { return _T("");}

	void InitWindow();

	void OnFinalMessage( HWND hWnd );
private:
	void OnBtnChange(TNotifyUI& msg);
	
private:
	MainWnd*      m_ParentHwnd;
	int		mDeviceId;

	CEditUI *mpDeviceID;
	CEditUI *mpName;
	CComboBoxUI *mpCom;
	CComboBoxUI *mpType;
	CEditUI *mpBaud;
	CEditUI *mpXWarning;
	CEditUI *mpZWarning;
	CEditUI *mpTWarning;
	CEditUI *mpAddress;
	CEditUI *mpMark;
public:

	DUI_DECLARE_MESSAGE_MAP()
};

