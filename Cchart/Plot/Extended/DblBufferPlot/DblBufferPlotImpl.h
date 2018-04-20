/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2011 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This is a templated class 

#ifndef __DBLBUFFERPLOTIMPL_H_122333444455555__
#define __DBLBUFFERPLOTIMPL_H_122333444455555__

#include "DblBufferPlot.h"
#include "../../Basics/XYHandler.h"

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//

template <typename XYPlotImplT>
class CDblBufferPlotImpl : public CDblBufferPlot, public XYPlotImplT
{
public:
	CDblBufferPlotImpl()
	{
		m_hWnd = 0;
	}

	CDblBufferPlotImpl(HWND hWnd):m_hWnd(hWnd)
	{
		m_bRectConfinedClient = false;
		CreateVirtualWindow();
	}
	virtual ~CDblBufferPlotImpl()
	{
		ReleaseVirtualWindow();
	}

	void		SetWnd(HWND hWnd)
	{
		m_bRectConfinedClient = false;
		ReleaseVirtualWindow();
		m_hWnd = hWnd;
		CreateVirtualWindow();
	}
	void		SetWnd(HWND hWnd, RECT rectClient)
	{
		m_bRectConfinedClient = true;
		m_rectConfinedClient = rectClient;
		ReleaseVirtualWindow();
		m_hWnd = hWnd;
		CreateVirtualWindow();
	}
	void		SetWnd(RECT rectClient)
	{
		if(!m_hWnd)return;
		SetWnd(m_hWnd, rectClient);
	}

public:
	void		OnDraw(HWND hWnd)
	{
		if( !IsWindow(hWnd) )
		{
			return;
		}

		if(m_hWnd==0)
		{
			SetWnd(hWnd);
		}
		else if(m_hWnd != hWnd)
		{
			SetWnd(hWnd);
		}
		
		HDC hDC = GetDC(m_hWnd);
		RECT destRect;
		if (m_IsCustomRect)
			destRect = m_CustomRect;
		else
			GetClientRect(hWnd, &destRect);

		OnDraw(hDC, destRect);
		ReleaseDC(m_hWnd, hDC);
	}

	void		OnDraw(HDC hDC, RECT destRect)
	{
		HWND hWnd = WindowFromDC(hDC);
		if(!IsWindow(hWnd))
		{
			return;
		}

		if(m_hWnd==0)
		{
			SetWnd(hWnd);
		}
		else if(m_hWnd != hWnd)
		{
			SetWnd(hWnd);
		}

		OnUpdate(destRect);

		//StretchBlt(hDC, 0, 0, m_maxX, m_maxY, m_hMemDC, 0, 0, m_maxX, m_maxY, SRCCOPY);
		StretchBlt(hDC, destRect.left, destRect.top, destRect.right-destRect.left, destRect.bottom-destRect.top, m_hMemDC, destRect.left, destRect.top, destRect.right-destRect.left, destRect.bottom-destRect.top, SRCCOPY);
	}
protected:
	void OnUpdate(RECT destRect)
	{
		HDC hDC = m_hMemDC;
		
		if (XYPlotImplT::IsEmpty())
		{
			XYPlotImplT::EraseBkgnd(hDC, destRect);
			return;
		}
		RECT realRect;
		if(m_bRectConfinedClient)
			realRect = m_rectConfinedClient;
		else
			realRect = destRect;
		SetLastClientRect(realRect);
		XYPlotImplT::OnDraw(hDC, realRect);
	}
};

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//

//Since the CHandler can not be used as a templated class, here c-style define is used like MFC
#define Declare_CDblBufferPlotImplI( CDblBufferPlotImplI, XYPlotImplT, CHandlerT ) \
\
	template < typename XYPlotImplT > \
	class CDblBufferPlotImplI : public CDblBufferPlotImpl< XYPlotImplT >, public CHandlerT< CDblBufferPlotImplI<XYPlotImplT> > \
	{ \
	public:\
		CDblBufferPlotImplI(){}\
		virtual ~CDblBufferPlotImplI(){}\
	public:\
		bool OnLButtonDown(HWND hWnd, POINT point){return CHandlerT<CDblBufferPlotImplI>::OnLButtonDown(hWnd, point);}\
		bool OnLButtonUp(HWND hWnd, POINT point){return CHandlerT<CDblBufferPlotImplI>::OnLButtonUp(hWnd, point);}\
		bool OnLButtonDblClk(HWND hWnd, POINT point){return CHandlerT<CDblBufferPlotImplI>::OnLButtonDblClk(hWnd, point);}\
		bool OnMouseMove(HWND hWnd, POINT point){return CHandlerT<CDblBufferPlotImplI>::OnMouseMove(hWnd, point);}\
		bool OnContextMenu( HMENU hMenu, HWND hWnd, POINT point ){return CHandlerT<CDblBufferPlotImplI>::OnContextMenu(hMenu, hWnd, point);}\
		bool OnKeyDown(HWND hWnd, UINT key){return CHandlerT<CDblBufferPlotImplI>::OnKeyDown(hWnd, key);}\
		void Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)\
		{\
			POINT	point;\
			point.x = LOWORD(lParam);\
			point.y = HIWORD(lParam);\
			UINT	key = wParam;\
			switch(message)\
			{\
				case WM_LBUTTONDOWN:	if(OnLButtonDown(hWnd, point))OnDraw(hWnd);break;\
				case WM_LBUTTONUP:		if(OnLButtonUp(hWnd, point))OnDraw(hWnd);break;\
				case WM_LBUTTONDBLCLK:	if(OnLButtonDblClk(hWnd, point))OnDraw(hWnd);break;\
				case WM_MOUSEMOVE:		if(OnMouseMove(hWnd, point))OnDraw(hWnd);break;\
				case WM_CONTEXTMENU:	if(OnContextMenu(NULL, hWnd, point))OnDraw(hWnd);break;\
				case WM_KEYDOWN:		if(OnKeyDown(hWnd, key))OnDraw(hWnd);break;\
				default:				return;\
			}\
		}\
	};\
\

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

#endif