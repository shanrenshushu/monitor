#include "stdafx.h"
#include "UIButton.h"

namespace DuiLib
{
	CButtonUI::CButtonUI()
		: m_uButtonState(0)
		, m_dwHotTextColor(0)
		, m_dwPushedTextColor(0)
		, m_dwFocusedTextColor(0)
		, m_dwHotBkColor(0)
		, m_dwPushedBkColor(0)
		, m_dwNormalBorderColor(0)
		, m_dwHotBorderColor(0)
		, m_dwPushedBorderColor(0)
		, m_dwDisabledBorderColor(0)
		, m_dwDisabledBkColor(0xffffffff)
	{
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}

	CDuiString CButtonUI::GetClass() const
	{
		return _T("ButtonUI");
	}

	LPVOID CButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_BUTTON) == 0 ) return static_cast<CButtonUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CButtonUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	void CButtonUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled()) {
				if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
					Activate();
					return;
				}
			}
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
				else m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			if( IsContextMenuUsed() ) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			// return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			// return;
		}
		if( event.Type == UIEVENT_SETCURSOR ) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		CLabelUI::DoEvent(event);
	}

	bool CButtonUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		return true;
	}

	void CButtonUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	//************************************
	// Method:    SetHotBkColor
	// FullName:  CButtonUI::SetHotBkColor
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DWORD dwColor
	// Note:	  
	//************************************
	void CButtonUI::SetHotBkColor( DWORD dwColor )
	{
		m_dwHotBkColor = dwColor;
	}

    //************************************
    // Method:    GetHotBkColor
    // FullName:  CButtonUI::GetHotBkColor
    // Access:    public
    // Returns:   DWORD
    // Qualifier: const
    // Note:
    //************************************
    DWORD CButtonUI::GetHotBkColor() const
    {
        return m_dwHotBkColor;
    }

    void CButtonUI::SetNormalBkColor(DWORD dwColor)
    {
        m_dwNormalBkColor = dwColor;
    }

    DWORD CButtonUI::GetNormalBkColor() const
    {
        return m_dwNormalBkColor;
    }

    void CButtonUI::SetPushedBkColor(DWORD dwColor)
    {
        m_dwPushedBkColor = dwColor;
    }

    DWORD CButtonUI::GetPushedBkColor() const
    {
        return m_dwPushedBkColor;
    }

    void CButtonUI::SetDisabledBkColor(DWORD dwColor)
    {
        m_dwDisabledBkColor = dwColor;
    }

    DWORD CButtonUI::GetDisabledBkColor() const
    {
        return m_dwDisabledBkColor;
    }

	void CButtonUI::SetHotTextColor(DWORD dwColor)
	{
		m_dwHotTextColor = dwColor;
	}

	DWORD CButtonUI::GetHotTextColor() const
	{
		return m_dwHotTextColor;
	}

	void CButtonUI::SetPushedTextColor(DWORD dwColor)
	{
		m_dwPushedTextColor = dwColor;
	}

	DWORD CButtonUI::GetPushedTextColor() const
	{
		return m_dwPushedTextColor;
	}

	void CButtonUI::SetFocusedTextColor(DWORD dwColor)
	{
		m_dwFocusedTextColor = dwColor;
	}

	DWORD CButtonUI::GetFocusedTextColor() const
	{
		return m_dwFocusedTextColor;
	}

	void CButtonUI::SetNormalBorderColor(DWORD dwColor)
	{
		m_dwNormalBorderColor = dwColor;
	}

	DWORD CButtonUI::GetNormalBorderColor() const
	{
		return m_dwNormalBorderColor;
	}
	void CButtonUI::SetHotBorderColor(DWORD dwColor)
	{
		m_dwHotBorderColor = dwColor;
	}

	DWORD CButtonUI::GetHotBorderColor() const
	{
		return m_dwHotBorderColor;
	}

	void CButtonUI::SetPushedBorderColor(DWORD dwColor)
	{
		m_dwPushedBorderColor = dwColor;
	}

	DWORD CButtonUI::GetPushedBorderColor() const
	{
		return m_dwPushedBorderColor;
	}

	void CButtonUI::SetDisabledBorderColor(DWORD dwColor)
	{
		m_dwDisabledBorderColor = dwColor;
	}

	DWORD CButtonUI::GetDisabledBorderColor() const
	{
		return m_dwDisabledBorderColor;
	}

	LPCTSTR CButtonUI::GetNormalImage()
	{
		return m_sNormalImage;
	}

	void CButtonUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetHotImage()
	{
		return m_sHotImage;
	}

	void CButtonUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetPushedImage()
	{
		return m_sPushedImage;
	}

	void CButtonUI::SetPushedImage(LPCTSTR pStrImage)
	{
		m_sPushedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetFocusedImage()
	{
		return m_sFocusedImage;
	}

	void CButtonUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CButtonUI::GetDisabledImage()
	{
		return m_sDisabledImage;
	}

	void CButtonUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	//************************************
	// Method:    GetForeImage
	// FullName:  CButtonUI::GetForeImage
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier:
	// Note:	  
	//************************************
	LPCTSTR CButtonUI::GetForeImage()
	{
		return m_sForeImage;
	}

	//************************************
	// Method:    SetForeImage
	// FullName:  CButtonUI::SetForeImage
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pStrImage
	// Note:	  
	//************************************
	void CButtonUI::SetForeImage( LPCTSTR pStrImage )
	{
		m_sForeImage = pStrImage;
		Invalidate();
	}

	//************************************
	// Method:    GetHotForeImage
	// FullName:  CButtonUI::GetHotForeImage
	// Access:    public 
	// Returns:   LPCTSTR
	// Qualifier:
	// Note:	  
	//************************************
	LPCTSTR CButtonUI::GetHotForeImage()
	{
		return m_sHotForeImage;
	}

	//************************************
	// Method:    SetHotForeImage
	// FullName:  CButtonUI::SetHotForeImage
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: LPCTSTR pStrImage
	// Note:	  
	//************************************
	void CButtonUI::SetHotForeImage( LPCTSTR pStrImage )
	{
		m_sHotForeImage = pStrImage;
		Invalidate();
	}

	SIZE CButtonUI::EstimateSize(SIZE szAvailable)
	{
		if( m_cxyFixed.cy == 0 ) return CSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 8);
		return CControlUI::EstimateSize(szAvailable);
	}

    void CButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if( _tcscmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
        else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
        else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
        else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
        else if( _tcscmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
        else if( _tcscmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
        else if( _tcscmp(pstrName, _T("hotforeimage")) == 0 ) SetHotForeImage(pstrValue);
        else if( _tcscmp(pstrName, _T("hotbkcolor")) == 0 )
        {
            if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotBkColor(clrColor);
        }
        else if( _tcscmp(pstrName, _T("hottextcolor")) == 0 )
        {
            if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotTextColor(clrColor);
        }
        else if( _tcscmp(pstrName, _T("pushedtextcolor")) == 0 )
        {
            if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetPushedTextColor(clrColor);
        }
        else if( _tcscmp(pstrName, _T("focusedtextcolor")) == 0 )
        {
            if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetFocusedTextColor(clrColor);
        }
        else if( _tcscmp(pstrName, _T("normalbkcolor")) == 0 )
        {
            if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetNormalBkColor(clrColor);
        }
        else if( _tcscmp(pstrName, _T("pushedbkcolor")) == 0 )
        {
            if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetPushedBkColor(clrColor);
        }
        else if( _tcscmp(pstrName, _T("disabledbkcolor")) == 0 )
        {
            if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetDisabledBkColor(clrColor);
        }
		else if( _tcscmp(pstrName, _T("normalbordercolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNormalBorderColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("hotbordercolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBorderColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("pushedbordercolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedBorderColor(clrColor);
		}
		else if( _tcscmp(pstrName, _T("disabledbordercolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledBorderColor(clrColor);
		}
        else CLabelUI::SetAttribute(pstrName, pstrValue);
    }

	void CButtonUI::PaintText(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		if( m_sText.IsEmpty() ) return;
		int nLinks = 0;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

		DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;

		if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
			clrColor = GetPushedTextColor();
		else if( ((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0) )
			clrColor = GetHotTextColor();
		else if( ((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0) )
			clrColor = GetFocusedTextColor();

		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, clrColor, \
			NULL, NULL, nLinks, m_uTextStyle);
		else
		{
			if(((m_uButtonState & UISTATE_HOT) != 0) && m_iHotFont > -1) 
				CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, clrColor, m_iHotFont, m_uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, clrColor, m_iFont, m_uTextStyle);
		}
	}

	void CButtonUI::PaintStatusImage(HDC hDC)
	{
		if(_tcsstr(m_sBkImage, _T(".gif"))|| _tcsstr(m_sBkImage, _T(".GIF")))
		{
			CLabelUI::PaintStatusImage(hDC);
			return;
		}
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

        if( (m_uButtonState & UISTATE_DISABLED) != 0 )
        {
			if(m_dwDisabledBkColor != 0)
			{
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwDisabledBkColor));
			}
            if( !m_sDisabledImage.IsEmpty() )
            {
                if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) m_sDisabledImage.Empty();
                else goto Label_ForeImage;
            }
        }
        else if( (m_uButtonState & UISTATE_PUSHED) != 0 )
        {
			if(m_dwPushedBkColor != 0)
			{
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwPushedBkColor));
			}
            if( !m_sPushedImage.IsEmpty() )
            {
                if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) )
                {
                    m_sPushedImage.Empty();
                }
                if( !m_sPushedForeImage.IsEmpty() )
                {
                    if( !DrawImage(hDC, (LPCTSTR)m_sPushedForeImage) )
                        m_sPushedForeImage.Empty();
                    return;
                }
                else goto Label_ForeImage;
            }
        }
        else if( (m_uButtonState & UISTATE_HOT) != 0 )
        {
			if(m_dwHotBkColor != 0)
			{
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
			}
            if( !m_sHotImage.IsEmpty() )
            {
                if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) )
                {
                    m_sHotImage.Empty();
                }
                if( !m_sHotForeImage.IsEmpty() )
                {
                    if( !DrawImage(hDC, (LPCTSTR)m_sHotForeImage) )
                        m_sHotForeImage.Empty();
                    return;
                }
                else goto Label_ForeImage;
            }
        }
        else if( (m_uButtonState & UISTATE_FOCUSED) != 0 )
        {
            if( !m_sFocusedImage.IsEmpty() )
            {
                if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) m_sFocusedImage.Empty();
                else goto Label_ForeImage;
            }
        }

		if( !m_sNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) m_sNormalImage.Empty();
			else goto Label_ForeImage;
		}

		if(!m_sForeImage.IsEmpty() )
			goto Label_ForeImage;

		return;

Label_ForeImage:
		if(!m_sForeImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sForeImage) ) m_sForeImage.Empty();
		}
	}

	void CButtonUI::PaintBorder(HDC hDC)
	{
		if(m_nBorderSize > 0 || m_rcBorderSize.left > 0 || m_rcBorderSize.top > 0 || m_rcBorderSize.right > 0 || m_rcBorderSize.bottom > 0)
		{
			if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
			else m_uButtonState &= ~ UISTATE_FOCUSED;
			if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
			else m_uButtonState &= ~ UISTATE_DISABLED;

			if( (m_uButtonState & UISTATE_DISABLED) != 0 )
			{
				if(m_dwDisabledBorderColor != 0)
					m_dwBorderColor = m_dwDisabledBorderColor;
			}
			else if( (m_uButtonState & (UISTATE_PUSHED | UISTATE_SELECTED) ) != 0 )
			{
				if(m_dwPushedBorderColor != 0)
					m_dwBorderColor = m_dwPushedBorderColor;
			}
			else if( (m_uButtonState & UISTATE_HOT) != 0 )
			{
				if(m_dwHotBorderColor != 0)
					m_dwBorderColor = m_dwHotBorderColor;
			}
			else 
			{
				if(m_dwNormalBorderColor != 0)
					m_dwBorderColor = m_dwNormalBorderColor;
			}

			if(m_dwBorderColor != 0)
			{
				if(m_nBorderSize > 0 && ( m_cxyBorderRound.cx > 0 || m_cxyBorderRound.cy > 0 ))	//»­Ô²½Ç±ß¿ò
				{
					CRenderEngine::DrawRoundRect(hDC, m_rcItem, m_nBorderSize, m_cxyBorderRound.cx, m_cxyBorderRound.cy, GetAdjustColor(m_dwBorderColor));
				}
				else
				{
					if(m_rcBorderSize.left > 0 || m_rcBorderSize.top > 0 || m_rcBorderSize.right > 0 || m_rcBorderSize.bottom > 0)
					{
						RECT rcBorder;

						if(m_rcBorderSize.left > 0){
							rcBorder		= m_rcItem;
							rcBorder.right	= m_rcItem.left;
							CRenderEngine::DrawLine(hDC,rcBorder,m_rcBorderSize.left,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
						}
						if(m_rcBorderSize.top > 0){
							rcBorder		= m_rcItem;
							rcBorder.bottom	= m_rcItem.top;
							CRenderEngine::DrawLine(hDC,rcBorder,m_rcBorderSize.top,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
						}
						if(m_rcBorderSize.right > 0){
							rcBorder		= m_rcItem;
							rcBorder.left	= m_rcItem.right;
							CRenderEngine::DrawLine(hDC,rcBorder,m_rcBorderSize.right,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
						}
						if(m_rcBorderSize.bottom > 0){
							rcBorder		= m_rcItem;
							rcBorder.top	= m_rcItem.bottom;
							CRenderEngine::DrawLine(hDC,rcBorder,m_rcBorderSize.bottom,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
						}
					}
					else if(m_nBorderSize > 0)
						CRenderEngine::DrawRect(hDC, m_rcItem, m_nBorderSize, GetAdjustColor(m_dwBorderColor));
				}
			}
		}
	}

	void CButtonUI::PaintBkImage( HDC hDC )
	{
		if(_tcsstr(m_sBkImage, _T(".gif"))|| _tcsstr(m_sBkImage, _T(".GIF")))
		{
			return;
		}
		else
		{
			__super::PaintBkImage(hDC);
		}
	}

}