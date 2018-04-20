#include "StdAfx.h"
#include "UIMessageBox.h"

namespace DuiLib
{
	UIMessageBox::UIMessageBox()
		:mStyle(MB_OK)
	{
	
	}

	UIMessageBox::~UIMessageBox()
	{

	}
	LRESULT UIMessageBox::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )
	{
		m_pm.Init(m_hWnd);

		CDialogBuilder builder;

		CDuiString m_strXMLPath = L"ResMain\\MessageBox.xml";
		CControlUI *pRoot = builder.Create(m_strXMLPath.GetData(), (UINT)0,  0, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddMessageFilter(this);
		CenterWindow();
		OnInit();
		return 0;
	}

	void UIMessageBox::OnInit()
	{
		mpIcon = static_cast<CLabelUI *>(m_pm.FindControl(_T("IconImg")));
		mpCaption = static_cast<CLabelUI *>(m_pm.FindControl(_T("caption")));
		mpClose = static_cast<CButtonUI *>(m_pm.FindControl(_T("close")));
		mpClose->OnNotify += MakeDelegate(this, &UIMessageBox::EventHandle);
		mpLeftBtn= static_cast<CButtonUI *>(m_pm.FindControl(_T("leftbtn")));
		mpLeftBtn->OnNotify += MakeDelegate(this, &UIMessageBox::EventHandle);
		mpRightBtn = static_cast<CButtonUI *>(m_pm.FindControl(_T("rightbtn")));
		mpRightBtn->OnNotify += MakeDelegate(this, &UIMessageBox::EventHandle);
		mpRichEdit = static_cast<CRichEditUI *>(m_pm.FindControl(_T("richedit")));
	}

	LRESULT UIMessageBox::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
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

	LPCTSTR UIMessageBox::GetWindowClassName()const
	{
		return _T("UIMessageBox");
	}

	LRESULT UIMessageBox::MessageHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled )
	{
		switch( uMsg ) 
		{
		case WM_KEYDOWN:
			{
				if( wParam == VK_ESCAPE) 
				{
					KeyAndClick(mpClose);
					return 1;
				}
				else if (wParam == VK_RETURN)
				{
					KeyAndClick(mpRightBtn);
				}
			}
			break;
		}
		return 0;
	}

	bool UIMessageBox::EventHandle( void *message )
	{
		TNotifyUI * msg = (TNotifyUI *)message;
		if( msg->sType == _T("click") )
		{
			KeyAndClick(msg->pSender);
		}
	
		return true;
	}

	void UIMessageBox::KeyAndClick( CControlUI *pSender )
	{
		switch (mStyle)
		{
		case MB_OK:
			if( pSender == mpClose)
			{
				Close(IDCANCEL);
			}
			else if (pSender == mpRightBtn)
			{
				Close(IDOK);
			}
			break;
		case MB_OKCANCEL:
			if( pSender == mpClose)
			{
				Close(IDCANCEL);
			}
			else if (pSender == mpLeftBtn)
			{
				Close(IDOK);
			}
			else if (pSender == mpRightBtn)
			{
				Close(IDCANCEL);
			}
			break;
		case MB_YESNO:
			if( pSender == mpClose)
			{
				Close(IDCANCEL);
			}
			else if (pSender == mpLeftBtn)
			{
				Close(IDYES);
			}
			else if (pSender == mpRightBtn)
			{
				Close(IDNO);
			}
			break;
		default:
			break;
		}
	}

	void UIMessageBox::SetAttribute( WhichControl which,LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		switch (which)
		{
		case eLeftBtn:
			mpLeftBtn->SetAttribute(pstrName,pstrValue);
			break;
		case eRightBtn:
			mpRightBtn->SetAttribute(pstrName,pstrValue);
			break;
		case eEdit:
			mpRichEdit->SetAttribute(pstrName,pstrValue);
			break;
		default:
			break;
		}
	}

	void UIMessageBox::Create( HWND parent,LPCTSTR caption,LPCTSTR text/*=L"提示"*/,UINT style /*= MB_OK*/,IconStyle icon /*=eDui*/)
	{
		__super::Create(parent, _T(""),WS_VISIBLE | WS_POPUP, WS_EX_TOOLWINDOW );

		mStyle = style;
		if (caption)
			mpRichEdit->SetText(caption);
		if(text)
			mpCaption->SetText(text);
		switch (icon)
		{
		case DuiLib::UIMessageBox::eCuo:
			mpIcon->SetBkImage(_T("ResMain/cuo.png"));
			break;
		case DuiLib::UIMessageBox::eDui:
			mpIcon->SetBkImage(_T("ResMain/dui.png"));
			break;
		case DuiLib::UIMessageBox::eTan:
			mpIcon->SetBkImage(_T("ResMain/tan.png"));
			break;
		case DuiLib::UIMessageBox::eWen:
			mpIcon->SetBkImage(_T("ResMain/wen.png"));
			break;
		default:
			break;
		}

		switch (style)
		{
		case MB_OK:
			mpLeftBtn->SetVisible(false);
			mpRightBtn->SetText(L"确认");
			break;
		case MB_OKCANCEL:
			mpLeftBtn->SetText(L"确认");
			mpRightBtn->SetText(L"取消");
			break;
		case MB_YESNO:
			mpLeftBtn->SetText(L"是");
			mpRightBtn->SetText(L"否");
			break;
		default:
			break;
		}
	}


}