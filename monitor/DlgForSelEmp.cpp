#include "stdafx.h"
#include "DlgForSelEmp.h"
#include "MainWnd.h"
#include <WinUser.h>

			
DUI_BEGIN_MESSAGE_MAP(DlgForSelEmp, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_sel", OnBtnSel )
DUI_END_MESSAGE_MAP()
DlgForSelEmp::DlgForSelEmp(HWND hWnd)
{
	m_ParentHwnd = hWnd;
	this->Create( hWnd, L"DlgForSelEmp", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForSelEmp::~DlgForSelEmp(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForSelEmp::InitWindow(){
	GetControlByName(DuiLib::CListUI,mpListView,L"setting_meter_list");
	Initlist();
	CenterWindow();
}

void DlgForSelEmp::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForSelEmp::OnBtnSel(TNotifyUI& msg){
	int count = mpListView->GetCount();
	for (int i=0;i<count;i++)
	{
		CListContainerElementUI *pListItem = (CListContainerElementUI *)mpListView->GetItemAt(i);
		CCheckBoxUI * pCheckbox=(CCheckBoxUI *)pListItem->FindSubControl(L"checkbox");
		if (pCheckbox->IsSelected())
		{
			int id = pCheckbox->GetTag();
			MAN->GetPersonDB()->SelectPersonByID(id);
		}
	}
	Close();
}

void DlgForSelEmp::Initlist()
{
	vector<sPersonInfo> ve;
	MAN->GetPersonDB()->GetAllPerson(ve);
	for (int i=0;i<ve.size();i++)
	{
		if (ve[i].checked==1)
		{
			continue;
		}
		
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
