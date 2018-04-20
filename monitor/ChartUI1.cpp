#include "StdAfx.h"
#include "ChartUI.h"

CCharTUI::CCharTUI(): 
	m_hWnd(NULL)
{} 

void CCharTUI::SetInternVisible( bool bVisible /*= true*/ )
{
	__super::SetInternVisible(bVisible);  
	::ShowWindow(m_hWnd, bVisible);  
}

void CCharTUI::SetPos( RECT rc )
{
	__super::SetPos(rc);  
	::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,SWP_NOZORDER | SWP_NOACTIVATE);  

}

BOOL CCharTUI::Attach(HWND hwnd)
{
	if (! ::IsWindow(hwnd))  
	{  
		ASSERT(0);
		return FALSE;  
	}  
	m_hWnd = hwnd;
	return TRUE;
}

HWND CCharTUI::Detach()
{
	m_hWnd = NULL;
	return m_hWnd;  
}

