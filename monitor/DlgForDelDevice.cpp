#include "stdafx.h"
#include "DlgForDelDevice.h"
#include "MainWnd.h"
#include <WinUser.h>

			
DUI_BEGIN_MESSAGE_MAP(DlgForDelDevice, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_del", OnBtnDel )
DUI_END_MESSAGE_MAP()
DlgForDelDevice::DlgForDelDevice(MainWnd* hWnd)
{
	m_ParentHwnd = hWnd;
	this->Create( hWnd->GetHWND(), L"DlgForDelDevice", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForDelDevice::~DlgForDelDevice(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForDelDevice::InitWindow(){
	GetControlByName(DuiLib::CListUI,mpListView,L"setting_meter_list");
	Initlist();
	CenterWindow();
}

void DlgForDelDevice::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForDelDevice::OnBtnDel(TNotifyUI& msg){
	int count = mpListView->GetCount();
	for (int i=0;i<count;i++)
	{
		CListContainerElementUI *pListItem = (CListContainerElementUI *)mpListView->GetItemAt(i);
		CCheckBoxUI * pCheckbox=(CCheckBoxUI *)pListItem->FindSubControl(L"checkbox");
		if (pCheckbox->IsSelected())
		{
			int id = pCheckbox->GetTag();
			MAN->GetDeviceDB()->Delete(id);
			sDeviceInfo info;
			info.deviceID=pListItem->GetTag();
			info.com=pCheckbox->GetUserData();
			m_ParentHwnd->DelDevice(info);
		}
	}
	Close();
}

void DlgForDelDevice::Initlist()
{
	vector<sDeviceInfo> ve;
	MAN->GetDeviceDB()->GetAllDevice(ve);
	for (int i=0;i<ve.size();i++)
	{
		CDialogBuilder builder;
		CListContainerElementUI *pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("listitem/ListItemForDelDevice.xml"), 0, NULL, &m_PaintManager));
		if(pListItem)
		{
			mpListView->Add(pListItem);
			pListItem->SetTag(ve[i].deviceID);
			CLabelUI* deviceid = (CLabelUI *)pListItem->FindSubControl(L"deviceId");
			deviceid->SetText(uint2str(ve[i].deviceID).c_str());
			CLabelUI* pName = (CLabelUI *)pListItem->FindSubControl(L"name");
			pName->SetText(ve[i].name.c_str());
			CLabelUI* pMark = (CLabelUI *)pListItem->FindSubControl(L"mark");
			pMark->SetText(ve[i].mark.c_str());
			CCheckBoxUI * pCheckbox=(CCheckBoxUI *)pListItem->FindSubControl(L"checkbox");
			pCheckbox->SetTag(ve[i].ID);
			pCheckbox->SetUserData(ve[i].com.c_str());
		}
	}
}
