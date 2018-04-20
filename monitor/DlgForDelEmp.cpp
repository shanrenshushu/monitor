#include "stdafx.h"
#include "DlgForDelEmp.h"
#include "MainWnd.h"
#include <WinUser.h>

			
DUI_BEGIN_MESSAGE_MAP(DlgForDelEmp, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_del", OnBtnDel )
DUI_END_MESSAGE_MAP()
DlgForDelEmp::DlgForDelEmp(HWND hWnd)
{
	m_ParentHwnd = hWnd;
	this->Create( hWnd, L"DlgForSelEmp", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForDelEmp::~DlgForDelEmp(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForDelEmp::InitWindow(){
	GetControlByName(DuiLib::CListUI,mpListView,L"setting_meter_list");
	Initlist();
	CenterWindow();
}

void DlgForDelEmp::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForDelEmp::OnBtnDel(TNotifyUI& msg){
	int count = mpListView->GetCount();
	for (int i=0;i<count;i++)
	{
		CListContainerElementUI *pListItem = (CListContainerElementUI *)mpListView->GetItemAt(i);
		CCheckBoxUI * pCheckbox=(CCheckBoxUI *)pListItem->FindSubControl(L"checkbox");
		if (pCheckbox->IsSelected())
		{
			int id = pCheckbox->GetTag();
			MAN->GetPersonDB()->Delete(id);
		}
	}
	Close();
}

void DlgForDelEmp::Initlist()
{
	vector<sPersonInfo> ve;
	MAN->GetPersonDB()->GetAllPerson(ve);
	for (int i=0;i<ve.size();i++)
	{
		CDialogBuilder builder;
		CListContainerElementUI *pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("listitem/ListItemForSelEmp.xml"), 0, NULL, &m_PaintManager));
		if(pListItem)
		{
			mpListView->Add(pListItem);
			//pListItem->SetTag(ve[i].ID);
			CLabelUI* pjobnum = (CLabelUI *)pListItem->FindSubControl(L"jobnum");
			pjobnum->SetText(ve[i].jobNum.c_str());
			CLabelUI* pName = (CLabelUI *)pListItem->FindSubControl(L"name");
			pName->SetText(ve[i].name.c_str());
			CLabelUI* pPhone = (CLabelUI *)pListItem->FindSubControl(L"phone");
			pPhone->SetText(ve[i].phone.c_str());
			CCheckBoxUI * pCheckbox=(CCheckBoxUI *)pListItem->FindSubControl(L"checkbox");
			pCheckbox->SetTag(ve[i].ID);
		}
	}
}
