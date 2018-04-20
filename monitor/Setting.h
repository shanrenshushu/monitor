#pragma once
#include "Data.h"

class SettingBox : public CWindowWnd
{
public:
	SettingBox();
	~SettingBox();

public:
	virtual LPCTSTR GetWindowClassName()const;
	virtual void	OnFinalMessage(HWND hWnd){delete this;}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	
	virtual void OnInit();

	bool EventHandle(void *msg);

	/*******************
	caption：标题，
	text:内容，
	style：类型，取值：MB_OK（确认），MB_OKCANCEL（取消，确认），MB_YESNO （否，是）
	返回值：
	IDOK = 1　　　　　　　　//按确定按钮
	IDCANCEL = 2　　　　　　//按取消按钮
	IDYES = 6　　　　　　　 //按是按钮
	IDNO = 7　　　　　　　　//按否按钮
	*******************/
	void Create(HWND parent,LPCTSTR caption=L"设置",psDeviceInfo settinginfo=NULL);

private:
	void SaveSettingInfo();

private:
	CPaintManagerUI	 m_pm;
	psDeviceInfo mpSetingInfo;

	CLabelUI *mpCaption;
	CLabelUI *mpIcon;
	CButtonUI *mpClose;
	CButtonUI *mpEnter;
	CButtonUI *mpCancel;
};
