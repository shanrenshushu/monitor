#include "stdafx.h"
#include "DlgForAddEmp.h"
#include "MainWnd.h"
#include <WinUser.h>

			
DUI_BEGIN_MESSAGE_MAP(DlgForAddEmp, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_add", OnBtnAdd )
DUI_END_MESSAGE_MAP()
DlgForAddEmp::DlgForAddEmp(HWND hWnd)
{
	m_ParentHwnd = hWnd;
	this->Create( hWnd, L"DlgForAddCarType", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForAddEmp::~DlgForAddEmp(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForAddEmp::InitWindow(){
	
	GetControlByName(CEditUI,mpJobnum,L"edt_employee_id");
	GetControlByName(CEditUI,mpName,L"edt_employee_name");
	GetControlByName(CEditUI,mpPhone,L"edt_employee_phone");
	CenterWindow();
}

void DlgForAddEmp::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForAddEmp::OnBtnAdd(TNotifyUI& msg){
	if (mpJobnum->GetText().IsEmpty())
	{
		MessageBox(this->GetHWND(),L"工号不能为空",L"错误",MB_OK);
		return ;
	}
	if (mpName->GetText().IsEmpty())
	{
		MessageBox(this->GetHWND(),L"名字不能为空",L"错误",MB_OK);
		return ;
	}
	if (mpPhone->GetText().IsEmpty())
	{
		MessageBox(this->GetHWND(),L"电话不能为空",L"错误",MB_OK);
		return ;
	}
	sPersonInfo info;
	info.jobNum = mpJobnum->GetText();
	info.name = mpName->GetText();
	info.phone = mpPhone->GetText();
	string sql = Format("SELECT * FROM %s WHERE jobNum='%s'",info.GetTableName().c_str(),U2A(info.jobNum).c_str());
	vector<sPersonInfo*> ve;
	MAN->GetPersonDB()->Query(ve,sql);
	if (!ve.empty())
	{
		SafeDeleteVector(ve);
		MessageBox(this->GetHWND(),L"该学号已经被添加",L"错误",MB_OK);
		return ;
	}
	MAN->GetPersonDB()->Insert(info);
	Close();
}

