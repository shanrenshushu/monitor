#include "stdafx.h"
#include "DlgForAddDevice.h"
#include "MainWnd.h"
#include <WinUser.h>

			
DUI_BEGIN_MESSAGE_MAP(DlgForAddDevice, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_add", OnBtnAdd )
DUI_END_MESSAGE_MAP()
DlgForAddDevice::DlgForAddDevice(MainWnd* hWnd)
{
	m_ParentHwnd = hWnd;
	this->Create( hWnd->GetHWND(), L"DlgForAddCarType", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForAddDevice::~DlgForAddDevice(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForAddDevice::InitWindow(){
	GetControlByName(CEditUI,mpDeviceID,L"edt_device_id");
	GetControlByName(CEditUI,mpName,L"edt_device_name");
	GetControlByName(CComboBoxUI,mpType,L"comb_device_type");
	GetControlByName(CEditUI,mpMark,L"edt_device_remark");
	GetControlByName(CComboBoxUI,mpCom,L"comb_select_com");
	GetControlByName(CEditUI,mpAddress,L"edt_select_add");
	GetControlByName(CEditUI,mpXWarning,L"edt_warm_x");
	GetControlByName(CEditUI,mpZWarning,L"edt_warm_y");
	GetControlByName(CEditUI,mpTWarning,L"edt_warm_tmp");

	vector<wstring> comVe = GetAllCom();
	for (int i=0;i<comVe.size();i++)
	{
		CListLabelElementUI *pElement = new CListLabelElementUI;
		pElement->SetTag(i);
		pElement->SetText(comVe[i].c_str());
		mpCom->Add(pElement);
	}

	CenterWindow();
}

void DlgForAddDevice::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForAddDevice::OnBtnAdd(TNotifyUI& msg){

	sDeviceInfo info;
	wstring temp = mpDeviceID->GetText();
	if (temp.empty())
	{
		MessageBox(this->GetHWND(),L"设备编号不能为空",L"错误",MB_OK);
		return ;
	}

	info.deviceID=_wtoi(temp.c_str());
	temp = mpType->GetText();
	if (temp.empty())
	{
		MessageBox(this->GetHWND(),L"设备类型不能为空",L"错误",MB_OK);
		return ;
	}
	info.type = temp;

	info.name = mpName->GetText();
	info.mark = mpMark->GetText();
	info.com = mpCom->GetText();
	info.address = _wtoi(mpAddress->GetText().GetData());
	info.XWarning = _wtof(mpXWarning->GetText());
	info.ZWarning = _wtof(mpZWarning->GetText());
	info.TWarning = _wtof(mpTWarning->GetText());
	
	sDeviceInfo find= MAN->GetDeviceDB()->GetDeviceByDeviceID(info.deviceID);
	if (find.deviceID != 0)
	{
		MessageBox(this->GetHWND(),L"该设备已经被添加",L"错误",MB_OK);
		return ;
	}
	if (!info.com.empty() && info.address!=0)
	{
		vector<psDeviceInfo>ve;
		string sql = Format("SELECT * FROM %s where com='%s' and address=%d",find.GetTableName().c_str(),U2A(info.com).c_str(),info.address);
		MAN->GetDeviceDB()->Query(ve,sql);
		if (!ve.empty())
		{
			SafeDeleteVector(ve);
			MessageBox(this->GetHWND(),L"该串口下的串口地址已被占用",L"错误",MB_OK);
			return ;
		}
	}
	MAN->GetDeviceDB()->Insert(info);
	//if (info.address !=0 && !info.com.empty())
	{
	m_ParentHwnd->AddDevice(info);
	}
	
	Close();
}