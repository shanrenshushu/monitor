#include "StdAfx.h"
#include "ChartUI.h"
#include  <mmsystem.h>
#pragma comment(lib,"winmm.lib")
CCharTUI::CCharTUI()
{
	m_funPaintVideo =NULL;
} 

CCharTUI::~CCharTUI()
{
	m_pManager->KillTimer(this, VIDEO_TIMER_ID);
}

void CCharTUI::DoPaint( HDC hDC, const RECT& rcPaint )
{
	//__super::DoPaint(hDC,rcPaint);
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;

	// »æÖÆÑ­Ðò£º±³¾°ÑÕÉ«->±³¾°Í¼->×´Ì¬Í¼->ÎÄ±¾->±ß¿ò
	
	PaintBkColor(hDC);
	PaintBkImage(hDC);
	//PaintStatusImage(hDC);
	PaintVideo(hDC);
	PaintText(hDC);
	PaintBorder(hDC);
	
}

void CCharTUI::SetTime()
{
	m_pManager->SetTimer(this, VIDEO_TIMER_ID, 100);
}

void CCharTUI::PaintVideo( HDC hDC )
{
	//m_rcItem
	if (m_funPaintVideo)
	{
		m_funPaintVideo(hDC,m_rcItem);
	}
}

void CCharTUI::SetFun( FUN_PAINTVIDEO fun )
{
	m_funPaintVideo = fun;
}

void CLabelUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_TIMER && event.wParam == VIDEO_TIMER_ID )
	{
		Invalidate();
		return;
	}
	
	CControlUI::DoEvent(event);
}