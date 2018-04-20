#include "StdAfx.h"
#include "ChartUI.h"
#include  <mmsystem.h>
#pragma comment(lib,"winmm.lib")
CCharTUI::CCharTUI(): 
	mpChart(NULL)
{} 

void CCharTUI::SetInternVisible( bool bVisible /*= true*/ )
{
	__super::SetInternVisible(bVisible);  
	//::ShowWindow(m_hWnd, bVisible);  
}

void CCharTUI::SetPos( RECT rc )
{
	__super::SetPos(rc);  
	//::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE/*SWP_NOZORDER | SWP_NOACTIVATE*/);  
	if(mpChart)
	 mpChart->SetCustomRect(rc);
}

BOOL CCharTUI::Attach(HWND hwnd,Chart *pchart )
{
	//if (! ::IsWindow(hWndNew))  
	//{  
	//	ASSERT(0);
	//	return FALSE;  
	//}  
	m_hWnd = hwnd;
	mpChart = pchart; 
	return TRUE;
}

HWND CCharTUI::Detach()
{
	if (mpChart)
	{
		delete mpChart;
		mpChart = NULL;
	}
	return m_hWnd;  
}

void CCharTUI::DoPaint( HDC hDC, const RECT& rcPaint )
{
	//__super::DoPaint(hDC,rcPaint);
	if (mpChart)
	{
		RECT rc = GetPos();
		long t1= timeGetTime();
		mpChart->OnDraw(hDC,rc);
		long t2 = timeGetTime();
		//LogInfo(L"t2-t1=%u",t2-t1);
	}
}
//LRESULT CCharTUI::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
//{
//	LRESULT lRes = 0;
//	switch( uMsg ) 
//	{
//	case WM_GETMINMAXINFO:
//		//lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
//		break;
//	case WM_LBUTTONDOWN:													  ////
//	case WM_LBUTTONUP:														  ////
//	case WM_LBUTTONDBLCLK:													  ////
//	case WM_MOUSEMOVE:														  ////
//	case WM_CONTEXTMENU:													  ////
//	case WM_KEYDOWN:
//		{
//			XYChartI * pxy= (XYChartI *)GetChart();
//
//			pxy->Interactive(m_hWnd, uMsg, wParam, lParam);
//		}
//		break;
//	}
//	return lRes;
//}
