#include "stdafx.h"
#include "MessageDialog.h"

using namespace DuiLib;
const TCHAR kWndBackGround[] =		_T("background");
CMessageDialog :: CMessageDialog() :
    m_pButtonOK(NULL),
    m_pButtonCancel(NULL),
	mpCheckBox(NULL),
	m_pIconImg(NULL),
	result(0),
	uType(0)
{
}

CMessageDialog::~CMessageDialog()
{

}

LPCTSTR CMessageDialog::GetWindowClassName() const
{
    return _T("UIMessageDialog");
}

UINT CMessageDialog::GetClassStyle() const
{
    return CS_DBLCLKS;
}


void CMessageDialog::OnFinalMessage(HWND /*hWnd*/)
{
	return;
}


DWORD CMessageDialog::GetBkColor() const
{
    CControlUI *background = m_pm.FindControl(kWndBackGround);
    if (background != NULL)
        return background->GetBkColor();
    return 0;
}

void CMessageDialog::SetBkColor(DWORD dwBackColor)
{
    CControlUI *background = m_pm.FindControl(kWndBackGround);
    if (background != NULL)
    {
        background->SetBkImage(_T(""));
        background->SetBkColor(dwBackColor);
        background->NeedUpdate();
    }

}

void CMessageDialog::SetBkImage(CDuiString strBkImage)
{
    CControlUI *background = m_pm.FindControl(kWndBackGround);
    if (background != NULL)
    {
        TCHAR szBuf[MAX_PATH] = {0};
        _stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,1000,30'"), strBkImage.GetData());
        background->SetBkImage(szBuf);
    }
}

LPCTSTR CMessageDialog::GetBkImage()
{
    CControlUI *background = m_pm.FindControl(kWndBackGround);
    if (background != NULL)
        return background->GetBkImage();
    return _T("");
}

void CMessageDialog::Init()
{
    m_pButtonOK = static_cast<CButtonUI *>(m_pm.FindControl(_T("btnOK")));
    m_pButtonCancel = static_cast<CButtonUI *>(m_pm.FindControl(_T("btnCancle")));
	m_pIconImg = static_cast<CLabelUI *>(m_pm.FindControl(_T("IconImg")));
	m_pRichEdit = static_cast<CRichEditUI *>(m_pm.FindControl(_T("TbMessage")));
	m_pTopContainer=static_cast<CVerticalLayoutUI *>(m_pm.FindControl(_T("topVert")));
	if(m_pTopContainer)
	{
		m_pTopContainer->SetBkColor(0xFFFFFFFF);
	}
	mpCheckBox = static_cast<CCheckBoxUI *>(m_pm.FindControl(_T("rememberCheckBox")));
}

void CMessageDialog::Notify(TNotifyUI &msg)
{
    if( msg.sType == _T("click") )
    {
        if( msg.pSender->GetName() == _T("btnOK"))
        {
			switch (m_uType)
			{
				case MB_OK:
					//Close(IDOK);
					OnKeyDown(0,VK_RETURN,0);
					return;
				case MB_YESNO:
					//Close(IDYES);
					OnKeyDown(0,VK_RETURN,0);
					return;
				case MB_OKCANCEL:
					OnKeyDown(0,VK_RETURN,0);
					//Close(IDOK);
					return;
			}
        }
        else if( msg.pSender->GetName() == _T("btnCancle"))
        {
			switch (m_uType)
			{
			case MB_YESNO:
				//Close(IDNO);
				OnKeyDown(0,VK_ESCAPE,0);
				return;
			case MB_OKCANCEL:
				//Close(IDCANCEL);
				OnKeyDown(0,VK_ESCAPE,0);
				return;
			}
        }
		else if( msg.pSender->GetName() == _T("btnClose"))
		{
			//this->Close(IDCLOSE);
			OnKeyDown(0,VK_ESCAPE,0);
		}
    }
}

LRESULT CMessageDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    //	styleValue &= ~WS_CAPTION;
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

    m_pm.Init(m_hWnd);
    CDialogBuilder builder;

    CControlUI *pRoot = builder.Create(m_strXMLPath.GetData(), (UINT)0,  0, &m_pm);
    ASSERT(pRoot && "Failed to parse XML");
    m_pm.AttachDialog(pRoot);
    m_pm.AddNotifier(this);

    Init();
    return 0;
}

LRESULT CMessageDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT CMessageDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT CMessageDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    if( ::IsIconic(*this) ) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMessageDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return 0;
}

LRESULT CMessageDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return 0;
}

LRESULT CMessageDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    POINT pt;
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);

    RECT rcCaption = m_pm.GetCaptionRect();
    if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom )
    {
        CControlUI *pControl = static_cast<CControlUI *>(m_pm.FindControl(pt));
        if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
                _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
            return HTCAPTION;
    }

    return HTCLIENT;
}

LRESULT CMessageDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    SIZE szRoundCorner = m_pm.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
    if( !::IsIconic(*this) /*&& (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)*/ )
    {
        CDuiRect rcWnd;
        ::GetWindowRect(*this, &rcWnd);
        rcWnd.Offset(-rcWnd.left, -rcWnd.top);
        rcWnd.right++;
        rcWnd.bottom++;
        HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
        ::SetWindowRgn(*this, hRgn, TRUE);
        ::DeleteObject(hRgn);
    }
#endif
    bHandled = FALSE;
    return 0;
}

LRESULT CMessageDialog::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    CDuiRect rcWork = oMonitor.rcWork;
    rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

    LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
    lpMMI->ptMaxPosition.x	= rcWork.left;
    lpMMI->ptMaxPosition.y	= rcWork.top;
    lpMMI->ptMaxSize.x		= rcWork.right;
    lpMMI->ptMaxSize.y		= rcWork.bottom;

    bHandled = FALSE;
    return 0;
}

LRESULT CMessageDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
    if( wParam == SC_CLOSE )
    {
        bHandled = TRUE;
        SendMessage(WM_CLOSE);
        return 0;
    }
    BOOL bZoomed = ::IsZoomed(*this);
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    if( ::IsZoomed(*this) != bZoomed )
    {
    }
    return lRes;
}

LRESULT CMessageDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch( uMsg )
    {
    case WM_CREATE:
        lRes = OnCreate(uMsg, wParam, lParam, bHandled);
        break;
    case WM_CLOSE:
        lRes = OnClose(uMsg, wParam, lParam, bHandled);
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
    case WM_GETMINMAXINFO:
        lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
        break;
    case WM_SYSCOMMAND:
        lRes = OnSysCommand(uMsg, wParam, lParam, bHandled);
        break;
	case WM_KEYDOWN:
		lRes = OnKeyDown(uMsg, wParam, lParam);
		break;
    default:
        bHandled = FALSE;
    }
    if( bHandled ) return lRes;
    if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

UINT CMessageDialog::DuiMessageBox(HWND pWnd, LPCTSTR lpText, LPCTSTR lpCaption , UINT uType, UINT uIcon,LPCTSTR lpBtnYesText/*=_T("是")*/,LPCTSTR lpBtnNoText/*=_T("否")*/,CDuiString CheckBox/*=L""*/)
{
	m_uType = uType;
    m_strXMLPath = _T("ResMain\\MessageBoxDialog.xml");
	Create(pWnd, _T(""), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);

    if ( uType == MB_OK)
    {
        if ( m_pButtonCancel != NULL )
        {
            m_pButtonCancel->SetVisible(false);
        }
		if(m_pButtonOK != NULL)
		{
			SIZE cXY = {115, 10};
			m_pButtonOK->SetFixedXY(cXY);
			m_pButtonOK->SetText(lpBtnYesText);
		}
    }
    else if( uType == MB_OKCANCEL )
    {
        //保持默认xml配置
    }
    else if(uType == MB_YESNO)
	{
		if(m_pButtonOK != NULL)
		{
			//m_pButtonOK->SetText(_T("是"));
			m_pButtonOK->SetText(lpBtnYesText);
		}
		if(m_pButtonCancel != NULL)
		{
//			m_pButtonCancel->SetText(_T("否"));
			m_pButtonCancel->SetText(lpBtnNoText);

		}
	} 
    if (m_pIconImg != NULL)
    {
        switch(uIcon)
        {
		case MB_ICONERROR:
			m_pIconImg->SetBkImage(_T("ResImage\\tang.png"));
			break;
		case MB_ICONEXCLAMATION:
			m_pIconImg->SetBkImage(_T("ResImage\\gou.png"));
			break;
		case MB_ICONQUESTION:
			m_pIconImg->SetBkImage(_T("ResImage\\weng.png"));
			break;
		case MB_ICONINFORMATION:
			break;
		case MB_ICON_ERROR2:
			m_pIconImg->SetBkImage(_T("ResImage\\error2.png"));
			break;
        default:           /*MB_ICONASTERISK  MB_ICONWARNING*/ 
			m_pIconImg->SetBkImage(_T("ResImage\\tang.png"));
			break;
        }
    }

    CTextUI *pCaption_control = static_cast<CTextUI *>(m_pm.FindControl( _T("caption")));
    if (pCaption_control != NULL)
    {
        pCaption_control->SetText(lpCaption);
    }
    if (m_pRichEdit != NULL)
    {
		m_pRichEdit->SetText(lpText);
    }
	if (!CheckBox.IsEmpty())
	{
		ShowCheckBox(CheckBox);
	}
    CenterWindow();
    UINT uRet = ShowModal();
	if (mpCheckBox && mpCheckBox->IsVisible())
	{
		if (mpCheckBox->IsSelected())
		{
			result |= eType_Check;
			uRet|= eType_Check;
		}
		else
		{
			result = result &~eType_Check;
			uRet = uRet &~eType_Check;
		}
	}
	if (result!=0)           //判断是否获得keydown值
	{
		return result;
	}
    return uRet;
}

UINT CMessageDialog::DoMadol(HWND pWnd, LPCTSTR lpText, LPCTSTR lpCaption /* = _T("提示") */, UINT uType /* = MB_OK */, UINT uIcon /* = MB_ICONWARNING */,LPCTSTR lpBtnYesText/* =_T("是") */,LPCTSTR lpBtnNoText/* =_T("否") */,CDuiString CheckBox/* =L"" */)
{
	CMessageDialog box;
	return box.DuiMessageBox(pWnd,lpText,lpCaption,uType,uIcon,lpBtnYesText,lpBtnNoText,CheckBox);
}
void CMessageDialog::ShowCheckBox(const CDuiString &text)
{
	if(mpCheckBox)
	{
		mpCheckBox->SetVisible(true);
		mpCheckBox->SetText(text.GetData());
	}
}

LRESULT CMessageDialog::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam==VK_RETURN)
	{
		switch (m_uType)
		{
 		case MB_YESNO:
			result = IDYES;
				break;
		case MB_OKCANCEL:
		case MB_OK:
		default:
			result = IDOK;
			break;
		}
		Close(result);
	}
	else if (wParam==VK_ESCAPE)
	{
		switch (m_uType)
		{
		case MB_YESNO:
			result = IDNO;
			break;
		case MB_OKCANCEL:
			result = IDCANCEL;
			break;
		case MB_OK:
		default:
			result = IDCLOSE;
			break;
		}
		Close(result);
	}
	return false;
}
//void CMessageDialog::DuiAboutBox(CWindowWnd *pMainWnd, LPCTSTR lpXMLPath)
//{
//    if (lpXMLPath == NULL)
//        return;
//    m_pMainWnd = pMainWnd;
//    m_strXMLPath = lpXMLPath;
//
//    Create(m_pMainWnd->GetHWND(), _T("关于此程序"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
//    CenterWindow();
//    ShowModal();
//
//}