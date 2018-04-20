#include "StdAfx.h"
#include "Setting.h"

SettingBox::SettingBox()
	:mpSetingInfo(NULL)
{
	
}

SettingBox::~SettingBox()
{

}
LRESULT SettingBox::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
{
	m_pm.Init(m_hWnd);

	CDialogBuilder builder;

	CDuiString m_strXMLPath = L"SettingBox.xml";
	CControlUI *pRoot = builder.Create(m_strXMLPath.GetData(), (UINT)0,  0, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	//m_pm.AddMessageFilter(this);
	CenterWindow();
	OnInit();
	return 0;
}

void SettingBox::OnInit()
{
	mpIcon = static_cast<CLabelUI *>(m_pm.FindControl(_T("IconImg")));
	mpCaption = static_cast<CLabelUI *>(m_pm.FindControl(_T("caption")));
	mpClose = static_cast<CButtonUI *>(m_pm.FindControl(_T("close")));
	mpClose->OnNotify += MakeDelegate(this, &SettingBox::EventHandle);
	mpEnter= static_cast<CButtonUI *>(m_pm.FindControl(_T("enter")));
	mpEnter->OnNotify += MakeDelegate(this, &SettingBox::EventHandle);
	mpCancel = static_cast<CButtonUI *>(m_pm.FindControl(_T("cancel")));
	mpCancel->OnNotify += MakeDelegate(this, &SettingBox::EventHandle);
}

LRESULT SettingBox::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
	case WM_CREATE: 
		lRes = OnCreate(uMsg, wParam, lParam, bHandled);
		break;
	default: bHandled = FALSE; break;
	}
	if( bHandled )
		return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )
		return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR SettingBox::GetWindowClassName()const
{
	return _T("SettingBox");
}

bool SettingBox::EventHandle( void *message )
{
	TNotifyUI * msg = (TNotifyUI *)message;
	if( msg->sType == _T("click") )
	{
		if( msg->pSender == mpClose || msg->pSender == mpCancel)
		{
			Close(IDCANCEL);
		}
		else if (msg->pSender == mpEnter)
		{
			SaveSettingInfo();
			Close(IDOK);
		}
	}
	
	return true;
}



void SettingBox::Create( HWND parent,LPCTSTR caption/*=L"ÉèÖÃ"*/,psDeviceInfo settinginfo/*=NULL*/)
{
	__super::Create(parent, _T(""),WS_VISIBLE | WS_POPUP, WS_EX_TOOLWINDOW );

	mpCaption->SetText(caption);

	mpSetingInfo = settinginfo;
	if (mpSetingInfo)
	{
		CEditUI *pName = static_cast<CEditUI *>(m_pm.FindControl(_T("name")));
		pName->SetText(mpSetingInfo->name.c_str());

		CComboUI *pCom = static_cast<CComboUI *>(m_pm.FindControl(_T("com")));
		pCom->SetText(mpSetingInfo->com.c_str());

		CComboUI *pBaud = static_cast<CComboUI *>(m_pm.FindControl(_T("baud")));
		wchar_t buf[64] = {0};
		swprintf(buf,64,L"%d",mpSetingInfo->baud);
		pBaud->SetText(buf);

		CComboUI *pType = static_cast<CComboUI *>(m_pm.FindControl(_T("type")));
		pType->SetText(mpSetingInfo->type.c_str());

		CEditUI *pXWarning = static_cast<CEditUI *>(m_pm.FindControl(_T("XWarning")));
		memset(buf,0,sizeof(wchar_t)*64);
		swprintf(buf,64,L"%.2f",mpSetingInfo->XWarning);
		pXWarning->SetText(buf);

		CEditUI *pZWarning = static_cast<CEditUI *>(m_pm.FindControl(_T("ZWarning")));
		memset(buf,0,sizeof(wchar_t)*64);
		swprintf(buf,64,L"%.2f",mpSetingInfo->ZWarning);
		pZWarning->SetText(buf);

		CEditUI *pTWarning = static_cast<CEditUI *>(m_pm.FindControl(_T("TWarning")));
		memset(buf,0,sizeof(wchar_t)*64);
		swprintf(buf,64,L"%.2f",mpSetingInfo->TWarning);
		pTWarning->SetText(buf);

	}
}

void SettingBox::SaveSettingInfo()
{
	if (mpSetingInfo)
	{
		CEditUI *pName = static_cast<CEditUI *>(m_pm.FindControl(_T("name")));
		mpSetingInfo->name = pName->GetText();

		CComboUI *pCom = static_cast<CComboUI *>(m_pm.FindControl(_T("com")));
		mpSetingInfo->com=pCom->GetText();

		CComboUI *pBaud = static_cast<CComboUI *>(m_pm.FindControl(_T("baud")));
		mpSetingInfo->baud = _wtoi(pBaud->GetText());

		CComboUI *pType = static_cast<CComboUI *>(m_pm.FindControl(_T("type")));
		mpSetingInfo->type=pType->GetText();

		CEditUI *pXWarning = static_cast<CEditUI *>(m_pm.FindControl(_T("XWarning")));
		mpSetingInfo->XWarning = _wtof(pXWarning->GetText());

		CEditUI *pZWarning = static_cast<CEditUI *>(m_pm.FindControl(_T("ZWarning")));
		mpSetingInfo->ZWarning = _wtof(pZWarning->GetText());

		CEditUI *pTWarning = static_cast<CEditUI *>(m_pm.FindControl(_T("TWarning")));
		mpSetingInfo->TWarning = _wtof(pTWarning->GetText());
	}
}
