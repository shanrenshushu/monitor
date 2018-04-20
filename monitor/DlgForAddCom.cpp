#include "stdafx.h"
#include "DlgForAddCom.h"
#include "MainWnd.h"
#include <WinUser.h>
#include "Common.h"

			
DUI_BEGIN_MESSAGE_MAP(DlgForAddCom, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_add", OnBtnAdd )
DUI_END_MESSAGE_MAP()
DlgForAddCom::DlgForAddCom(HWND hWnd)
{
	m_ParentHwnd = hWnd;
	this->Create( hWnd, L"DlgForAddCarType", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForAddCom::~DlgForAddCom(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForAddCom::InitWindow(){
	CenterWindow();
	CComboUI *pCom = static_cast<CComboUI *>(m_PaintManager.FindControl(_T("comb_select_com")));
	vector<wstring> comVe = GetAllCom();
	for (int i=0;i<comVe.size();i++)
	{
		CListLabelElementUI *pElement = new CListLabelElementUI;
		pElement->SetTag(i);
		pElement->SetText(comVe[i].c_str());
		pCom->Add(pElement);
	}
}

void DlgForAddCom::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForAddCom::OnBtnAdd(TNotifyUI& msg){
	CComboUI *pCom = static_cast<CComboUI *>(m_PaintManager.FindControl(_T("comb_select_com")));
	if (pCom->GetCurSel()<0)
	{
		MessageBox(this->GetHWND(),L"请选择串口",L"错误",MB_OK);
		return;
	}
	CDuiString com= pCom->GetText();
	vector<sComInfo*> ve;
	sComInfo cominfo;
	wstring sql = Format(L"SELECT * FROM %s WHERE name='%s'",A2U(cominfo.GetTableName()).c_str(),com.GetData());
	MAN->GetComDB()->Query(ve,U2A(sql));
	if (!ve.empty())
	{
		MessageBox(this->GetHWND(),L"该串口已被添加在列表里面，请重新选择或者返回",L"错误",MB_OK);
		SafeDeleteVector(ve);
		return;
	}
	else
	{
		CEditUI *pEdit = static_cast<CEditUI *>(m_PaintManager.FindControl(_T("edt_com_desc")));
		CDuiString mark = pEdit->GetText();
		sComInfo info;
		info.name = com.GetData();
		info.mark = mark;
		MAN->GetComDB()->Insert(info);
	}

	Close();
}