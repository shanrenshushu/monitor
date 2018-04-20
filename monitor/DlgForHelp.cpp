#include "stdafx.h"
#include "DlgForHelp.h"
#include "MainWnd.h"
#include <WinUser.h>

			
DUI_BEGIN_MESSAGE_MAP(DlgForHelp, WindowImplBase)
	DUI_ON_CLICK_CTRNAME( L"btn_help_ok", OnBtnHelpOk )
DUI_END_MESSAGE_MAP()
DlgForHelp::DlgForHelp(HWND hWnd)
{
	m_ParentHwnd = hWnd;
	this->Create( hWnd, L"DlgForHelp", UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW);
}

DlgForHelp::~DlgForHelp(void){
	//manager()->GetUiUpdater()->delWnd(this->GetHWND());
}
void DlgForHelp::InitWindow(){
	CenterWindow();
}

void DlgForHelp::OnFinalMessage( HWND hWnd ){
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DlgForHelp::OnBtnHelpOk(TNotifyUI& msg){
	
	Close();
}