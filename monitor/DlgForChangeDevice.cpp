#include "stdafx.h"
#include "DlgForChangeDevice.h"
#include "MainWnd.h"
#include <WinUser.h>

			
DUI_BEGIN_MESSAGE_MAP(DlgForChangeDevice, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_change", OnBtnChange )
DUI_END_MESSAGE_MAP()
DlgForChangeDevice::DlgForChangeDevice(MainWnd* paren,int deviceid)
{
	mDeviceId = deviceid;
	m_ParentHwnd = paren;
	this->Create( paren->GetHWND(), L"DlgForChangeDevice", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForChangeDevice::~DlgForChangeDevice(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForChangeDevice::InitWindow(){
	GetControlByName(CEditUI,mpDeviceID,L"edt_device_id");
	GetControlByName(CEditUI,mpName,L"edt_device_name");
	GetControlByName(CComboBoxUI,mpType,L"comb_device_type");
	GetControlByName(CEditUI,mpMark,L"edt_device_remark");
	GetControlByName(CComboBoxUI,mpCom,L"comb_select_com");
	GetControlByName(CEditUI,mpAddress,L"edt_select_add");
	GetControlByName(CEditUI,mpXWarning,L"edt_warm_x");
	GetControlByName(CEditUI,mpZWarning,L"edt_warm_y");
	GetControlByName(CEditUI,mpTWarning,L"edt_warm_tmp");

	sDeviceInfo info = MAN->GetDeviceDB()->GetDeviceByDeviceID(mDeviceId);
	mpDeviceID->SetText(Format(L"%d",info.deviceID).c_str());
	mpDeviceID->SetEnabled(false);
	mpName->SetText(info.name.c_str());
	mpType->SetText(info.type.c_str());
	mpMark->SetText(info.mark.c_str());
	mpAddress->SetText(Format(L"%d",info.address).c_str());
	mpXWarning->SetText(Format(L"%d",info.XWarning).c_str());
	mpZWarning->SetText(Format(L"%d",info.ZWarning).c_str());
	mpTWarning->SetText(Format(L"%d",info.TWarning).c_str());

	vector<wstring> comVe = GetAllCom();
	int index=0;
	for (int i=0;i<comVe.size();i++)
	{
		CListLabelElementUI *pElement = new CListLabelElementUI;
		pElement->SetTag(i);
		pElement->SetText(comVe[i].c_str());
		if (info.com == comVe[i].c_str())
		{
			index = i;
		}
		mpCom->Add(pElement);
	}
	mpCom->SelectItem(index);
	CenterWindow();
}

void DlgForChangeDevice::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForChangeDevice::OnBtnChange(TNotifyUI& msg){
	sDeviceInfo info;

	
	wstring temp = mpType->GetText();
	if (temp.empty())
	{
		MessageBox(this->GetHWND(),L"设备类型不能为空",L"错误",MB_OK);
		return ;
	}
	info.deviceID=_wtoi( mpDeviceID->GetText());
	info.type = temp;
	info.name = mpName->GetText();
	info.mark = mpMark->GetText();
	info.com = mpCom->GetText();
	info.address = _wtoi(mpAddress->GetText().GetData());
	info.XWarning = _wtof(mpXWarning->GetText());
	info.ZWarning = _wtof(mpZWarning->GetText());
	info.TWarning = _wtof(mpTWarning->GetText());

	if (!info.com.empty() && info.address!=0)
	{
		vector<psDeviceInfo>ve;
		string sql = Format("SELECT * FROM %s where deviceID<>%d and com='%s' and address=%d",info.GetTableName().c_str(),info.deviceID,U2A(info.com).c_str(),info.address);
		MAN->GetDeviceDB()->Query(ve,sql);
		if (!ve.empty())
		{
			SafeDeleteVector(ve);
			MessageBox(this->GetHWND(),L"该串口下的串口地址已被占用",L"错误",MB_OK);
			return ;
		}
	}

	MAN->GetDeviceDB()->Update(info);
	Close();
}