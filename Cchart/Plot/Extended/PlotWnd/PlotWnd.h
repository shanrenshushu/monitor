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

#ifndef __PLOTWND_H_122333444455555__
#define __PLOTWND_H_122333444455555__

#include <windows.h>
#include <tchar.h>

#define	PLOT_CLASS_NAME		_TEXT("PlotWnd_Defined_by_Yang_Guojun")

static void *pThis;

//////////////////////////////////////////////////////////////////////////////////////////////
// This is a templated class, but a glabal value pThis must be used. it may cause trouble
//

template<typename PlotImplT>
class CPlotWnd
{
public:
	CPlotWnd();
	CPlotWnd(HINSTANCE hInst, HWND hWndParent);
	virtual	~CPlotWnd();
public:
	HWND	m_hWnd;
private:
	bool	m_bCreated;
	UINT	m_uintOldWndProc;

	virtual	bool		RegisterWndClass(HINSTANCE hInstance);
	virtual	HWND		CreatePlotWnd(	HWND hWndParent, HINSTANCE hInstance,
										DWORD dwStyle, int x, int y, int cx, int cy);
	static	LRESULT	CALLBACK	PlotWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	PlotImplT	*GetPlot();

	bool	Create(HINSTANCE hInst, HWND hWndParent);
	void	Release();
	bool	Attach(HWND hWnd);
	bool	Detach();	
};

//////////////////////////////////////////////////////////////////////////////////////////////
//

template<typename PlotImplT>
CPlotWnd<PlotImplT>::CPlotWnd( )
{
	m_hWnd = 0;
	m_bCreated = false;
	m_uintOldWndProc = 0;

	pThis = this;
}

template<typename PlotImplT>
CPlotWnd<PlotImplT>::CPlotWnd(HINSTANCE hInst, HWND	hWndParent)
{
	Create(hInst, hWndParent);

	pThis = this;
}

template<typename PlotImplT>
CPlotWnd<PlotImplT>::~CPlotWnd()
{
	Release();
	if(IsWindow(m_hWnd) && m_bCreated)
	{
		DestroyWindow(m_hWnd);
	}
}

template<typename PlotImplT>
bool	CPlotWnd<PlotImplT>::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	if(::GetClassInfo(hInstance, PLOT_CLASS_NAME, &wc))
		return true;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;//(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = PlotWindowProc;
	wc.lpszClassName = PLOT_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	if(!RegisterClass(&wc)) {
		return false;
	}
	return true;
}

template<typename PlotImplT>
HWND	CPlotWnd<PlotImplT>::CreatePlotWnd( HWND hWndParent, HINSTANCE hInstance, 
								  DWORD dwStyle, int x, int y, int cx, int cy)
{
	HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, PLOT_CLASS_NAME, NULL, 
								dwStyle | WS_BORDER | WS_TABSTOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS , 
								x, y, cx, cy, hWndParent, NULL, hInstance, NULL);
	if(hWnd)
	{
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
		SetFocus(hWnd);
		
		PlotImplT *pPlot = new PlotImplT;
		pPlot->SetWnd( hWnd );
		SetWindowLong(hWnd, GWL_USERDATA, (long)pPlot);
	}
	return hWnd;
}

template<typename PlotImplT>
PlotImplT	*CPlotWnd<PlotImplT>::GetPlot()
{
	PlotImplT* pPlot = (PlotImplT*)::GetWindowLong(m_hWnd, GWL_USERDATA);
	return pPlot;
}

template<typename PlotImplT>
LRESULT	CALLBACK	CPlotWnd<PlotImplT>::PlotWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PlotImplT *pPlot = (PlotImplT*)GetWindowLong(hWnd, GWL_USERDATA);
	CPlotWnd<PlotImplT>* pT= (CPlotWnd<PlotImplT>*)pThis;

	if(pPlot)
	{
		switch(msg)
		{
		case WM_PAINT:
			pPlot->OnDraw(hWnd);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		case WM_CONTEXTMENU:
		case WM_KEYDOWN:
			pPlot->Interactive(hWnd, msg, wParam, lParam);
			break;
		case WM_ERASEBKGND:
			return 0;
		case WM_SIZE:
			if(!pT->m_bCreated)
				return CallWindowProc((WNDPROC)pT->m_uintOldWndProc, hWnd, msg, wParam, lParam);
			else
			{
				HWND hWndParent = GetParent(hWnd);
				RECT rt;
				GetClientRect(hWndParent, &rt);
				MoveWindow(hWnd, rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top, TRUE);
				break;
			}
		case WM_DESTROY:
			if(pPlot)
				delete pPlot;
			SetWindowLong(hWnd, GWL_USERDATA, 0);
			if(!pT->m_bCreated)
			{
				SetWindowLong(hWnd, GWL_WNDPROC, pT->m_uintOldWndProc);
				CallWindowProc((WNDPROC)pT->m_uintOldWndProc, hWnd, msg, wParam, lParam);
			}
			break;
		case WM_NCCALCSIZE:
			if(pT->m_bCreated)
				return CallWindowProc((WNDPROC)pT->m_uintOldWndProc, hWnd, msg, wParam, lParam);
			break;
		default:
			if(!pT->m_bCreated)
				return CallWindowProc((WNDPROC)pT->m_uintOldWndProc, hWnd, msg, wParam, lParam);
			break;
		}
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

template<typename PlotImplT>
void	CPlotWnd<PlotImplT>::Release( )
{
	if(IsWindow(m_hWnd))
	{
		PlotImplT* pPlot = (PlotImplT*)::GetWindowLong(m_hWnd, GWL_USERDATA);
		if(pPlot)delete pPlot;

		if(m_bCreated)
		{
			DestroyWindow(m_hWnd);
			m_bCreated = false;
		}
		else
		{
			SetWindowLong(m_hWnd, GWL_WNDPROC, m_uintOldWndProc);
			m_hWnd = 0;
			m_bCreated = false;
		}
	}
	
}

template<typename PlotImplT>
bool	CPlotWnd<PlotImplT>::Create(HINSTANCE hInst, HWND hWndParent)
{
	RegisterWndClass(hInst);
	Release();
	m_hWnd = CreatePlotWnd(hWndParent, hInst, WS_VISIBLE | WS_CHILD, 0,0,0,0);
	m_bCreated = true;
	m_uintOldWndProc = 0;

	return true;
}

template<typename PlotImplT>
bool	CPlotWnd<PlotImplT>::Attach(HWND hWnd)
{
	// If the given hwnd is not a window, return
	if(!IsWindow(hWnd))
		return false;

	PlotImplT *pPlot = 0;
	if(IsWindow(m_hWnd))
	{
		// Store the plot pointer
		pPlot=(PlotImplT*)GetWindowLong(hWnd, GWL_USERDATA);
		if( m_bCreated )
		{
			DestroyWindow(m_hWnd);
			m_bCreated = false;
		}
	}
	
	if(!pPlot) pPlot = new PlotImplT;

	m_hWnd = hWnd;
	SetWindowLong(m_hWnd, GWL_USERDATA, (long)pPlot);
	m_uintOldWndProc = SetWindowLong(m_hWnd, GWL_WNDPROC, (long)PlotWindowProc);
	return true;
}

template<typename PlotImplT>
bool	CPlotWnd<PlotImplT>::Detach()
{
	if(!IsWindow(m_hWnd))return true;
	if(m_bCreated)return true;

	Release();
	return true;
}

#endif