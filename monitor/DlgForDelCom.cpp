#include "stdafx.h"
#include "DlgForDelCom.h"
#include "MainWnd.h"
#include <WinUser.h>

			
DUI_BEGIN_MESSAGE_MAP(DlgForDelCom, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_del", OnBtnDel )
DUI_END_MESSAGE_MAP()
DlgForDelCom::DlgForDelCom(HWND hWnd)
{
	m_ParentHwnd = hWnd;
	this->Create( hWnd, L"DlgForDelCom", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForDelCom::~DlgForDelCom(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForDelCom::InitWindow(){
	GetControlByName(DuiLib::CListUI,mpListView,L"setting_meter_list");
	Initlist();
	CenterWindow();
}

void DlgForDelCom::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForDelCom::OnBtnDel(TNotifyUI& msg){
	int count = mpListView->GetCount();
	for (int i=0;i<count;i++)
	{
		CListContainerElementUI *pListItem = (CListContainerElementUI *)mpListView->GetItemAt(i);
		CCheckBoxUI * pCheckbox=(CCheckBoxUI *)pListItem->FindSubControl(L"checkbox");
		if (pCheckbox->IsSelected())
		{
			int id = pCheckbox->GetTag();
			MAN->GetComDB()->Delete(id);
		}
	}
	Close();

}

void DlgForDelCom::Initlist()
{
	vector<sComInfo> ve;
	MAN->GetComDB()->GetAllCom(ve);
	for (int i=0;i<ve.size();i++)
	{
		CDialogBuilder builder;
		CListContainerElementUI *pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("listitem/ListItemForDelCom.xml"), 0, NULL, &m_PaintManager));
		if(pListItem)
		{
			mpListView->Add(pListItem);
			pListItem->SetTag(ve[i].ID);
			CLabelUI* pName = (CLabelUI *)pListItem->FindSubControl(L"name");
			pName->SetText(ve[i].name.c_str());
			CLabelUI* pMark = (CLabelUI *)pListItem->FindSubControl(L"mark");
			pMark->SetText(ve[i].mark.c_str());
			CCheckBoxUI * pCheckbox=(CCheckBoxUI *)pListItem->FindSubControl(L"checkbox");
			pCheckbox->SetTag(ve[i].ID);
		}
	}
	
}
