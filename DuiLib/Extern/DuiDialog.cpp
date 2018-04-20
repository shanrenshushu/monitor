// DuiDialog.cpp: implementation of the CDuiDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DuiDialog.h"
#include "MessageDialog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace DuiLib;
CDuiDialog::CDuiDialog() {

}

CDuiDialog::~CDuiDialog() {

}

HWND CDuiDialog::Create( LPCTSTR lpszUISkin, IDialogBuilderCallback *pCallback ) {
    //DUI__Trace(L"CDuiDialog::Create lpszUISkin=%s",lpszUISkin);
    m_pCallback = pCallback;
    m_strUISkin = lpszUISkin;
    return CWindowWnd::Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0);	  //UI_WNDSTYLE_DIALOG
}

HWND CDuiDialog::Create( LPCTSTR lpszUISkin, IDialogBuilderCallback *pCallback, DWORD dwStyle , DWORD dwExStyle /*=0*/) {
    m_pCallback = pCallback;
    m_strUISkin = lpszUISkin;
    return CWindowWnd::Create(NULL, NULL, dwStyle, 0);
}

LRESULT CDuiDialog::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch( uMsg ) {
    case WM_CREATE:
        lRes = OnCreate(uMsg, wParam, lParam, bHandled);
        break;
    case WM_DESTROY:
        lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
        break;
    case WM_NCACTIVATE:
        lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
        break;
    case WM_NCCALCSIZE:
        lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
        break;
    case WM_NCPAINT:
        lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
        break;
    case WM_NCHITTEST:
        lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
        break;
    case WM_SIZE:
        lRes = OnSize(uMsg, wParam, lParam, bHandled);
        break;
    default:
        bHandled = FALSE;
    }
    if( bHandled ) return lRes;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CDuiDialog::OnInit() {

}

void CDuiDialog::OnNotify( TNotifyUI &msg ) {
    if( _tcsicmp (msg.sType, _T("click")) == 0 ) {
        OnClick(msg.pSender);
    } else if (_tcsicmp (msg.sType , _T ("selectchanged")) == 0) {
        OnSelChanged (msg.pSender);
    } else if (_tcsicmp (msg.sType , _T ("killfocus")) == 0) {
        OnKillFocus(msg.pSender);
    }
}

void CDuiDialog::OnClick( CControlUI *pSender ) {

}

LRESULT CDuiDialog::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled ) {
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    styleValue &= ~WS_CAPTION;
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

    m_pm.Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI *pRoot = builder.Create(m_strUISkin.GetData(), NULL, m_pCallback, &m_pm);
    ASSERT(pRoot && "Failed to parse XML");
    m_pm.AttachDialog(pRoot);
    m_pm.AddNotifier(this);

    OnInit();
    return 0;
}

LRESULT CDuiDialog::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled ) {
    bHandled = FALSE;
    return 0;
}

LRESULT CDuiDialog::OnNcActivate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled ) {
    if( ::IsIconic(*this) ) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CDuiDialog::OnNcCalcSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled ) {
    bHandled = FALSE;
    return 0;
}

LRESULT CDuiDialog::OnNcPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled ) {
    bHandled = FALSE;
    return 0;
}

LRESULT CDuiDialog::OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled ) {
    POINT pt;
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);

    RECT rcCaption = m_pm.GetCaptionRect();
    if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
        CControlUI *pControl = static_cast<CControlUI *>(m_pm.FindControl(pt));
        if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
            return HTCAPTION;
    }

    return HTCLIENT;
}

LRESULT CDuiDialog::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled ) {
    bHandled = FALSE;
    return 0;
}

void CDuiDialog::Notify( TNotifyUI &msg ) {
    OnNotify(msg);
}

void CDuiDialog::OnSelChanged( CControlUI *pSender ) {
    //	throw std::exception("The method or operation is not implemented.");
}

void CDuiDialog::OnKillFocus( CControlUI *pSender ) {

}

UINT CDuiDialog::MessageBox( LPCTSTR lpText, UINT uIcon/* = MB_ICONWARNING */, LPCTSTR lpCaption /*= _T("��ʾ")*/, UINT uType /*= MB_OK */,LPCTSTR lpBtnYesText/*=_T("��")*/,LPCTSTR lpBtnNoText/*=_T("��")*/) {
    return CMessageDialog::DoMadol(m_hWnd, lpText, lpCaption, uType, uIcon,lpBtnYesText,lpBtnNoText);
}

DWORD CDuiDialog::GetColor(LPCTSTR pszColorValue)
{
    while( *pszColorValue > _T('\0') && *pszColorValue <= _T(' ') )
		pszColorValue = ::CharNext(pszColorValue);
    if( *pszColorValue == _T('#'))
		pszColorValue = ::CharNext(pszColorValue);
    LPTSTR pstr = NULL;
    DWORD clrColor = _tcstoul(pszColorValue, &pstr, 16);
    return clrColor;
}
