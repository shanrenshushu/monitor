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
	caption�����⣬
	text:���ݣ�
	style�����ͣ�ȡֵ��MB_OK��ȷ�ϣ���MB_OKCANCEL��ȡ����ȷ�ϣ���MB_YESNO �����ǣ�
	����ֵ��
	IDOK = 1����������������//��ȷ����ť
	IDCANCEL = 2������������//��ȡ����ť
	IDYES = 6�������������� //���ǰ�ť
	IDNO = 7����������������//����ť
	*******************/
	void Create(HWND parent,LPCTSTR caption=L"����",psDeviceInfo settinginfo=NULL);

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
