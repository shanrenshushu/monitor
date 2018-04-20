
#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;
class MainWnd;

class DlgForAddDevice :public WindowImplBase
{
public:
	DlgForAddDevice( MainWnd* hWnd );

	~DlgForAddDevice(void);

	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMeterMsgBox");  }

	virtual CDuiString GetSkinFile()                {   return _T("DlgForAddDevice.xml");  }

	CDuiString GetSkinFolder() { return _T("");}

	void InitWindow();
	void OnFinalMessage( HWND hWnd );
private:
	void OnBtnAdd(TNotifyUI& msg);
	
private:
	MainWnd*      m_ParentHwnd;
	
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

