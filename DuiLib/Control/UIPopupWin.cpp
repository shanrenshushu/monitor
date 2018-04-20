#include "stdafx.h"
#include "UIPopupWin.h"


void DuiLib::CPopupWin::Init( HWND hWndParent,const RECT &rect )
{
	RECT rc = rect;
	::MapWindowRect(hWndParent, HWND_DESKTOP, &rc);

	Create(hWndParent, NULL, WS_POPUP, WS_EX_TOOLWINDOW, rc);

	::ShowWindow(m_hWnd, SW_SHOW);

	//HWND hWndParent = m_hWnd;
	//while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
	//::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

LPCTSTR DuiLib::CPopupWin::GetWindowClassName() const
{
	return _T("CPopupWin");
}

void DuiLib::CPopupWin::OnFinalMessage( HWND hWnd )
{
	delete this;
}

LRESULT DuiLib::CPopupWin::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	default:				bHandled = FALSE; break;
	}
	if (bHandled)
	{
		return lRes;
	}
	else
	{
		lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
		if (bHandled)
		{
			return lRes;
		}
	}
	
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) )
	{
		return lRes;
	}
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT DuiLib::CPopupWin::OnKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT DuiLib::CPopupWin::OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	PostMessage(WM_CLOSE);
	return lRes;
}

LRESULT DuiLib::CPopupWin::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT DuiLib::CPopupWin::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;//默认不做处理
	return 0;
}
