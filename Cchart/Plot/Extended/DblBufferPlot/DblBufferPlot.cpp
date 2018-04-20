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

//#include "stdafx.h"
#include "DblBufferPlot.h"

/////////////////////////////////////////////////////////////////////////
// Class CPlot

CDblBufferPlot::CDblBufferPlot()
{
	m_hWnd = NULL;
	m_hMemDC = NULL;
	SetDefaults();
	
}

CDblBufferPlot::~CDblBufferPlot()
{
}

void	CDblBufferPlot::SetDefaults()
{
	m_bRectConfinedClient = false;
	ZeroMemory(&m_rectConfinedClient, sizeof(RECT));
	
	m_maxX=GetSystemMetrics(SM_CXSCREEN);
	m_maxY=GetSystemMetrics(SM_CYSCREEN);

	m_bForCtrl = false;
}

// Copy the settings from one chart to another
void	CDblBufferPlot::CopySettings( const CDblBufferPlot *plot )
{
	// Then CDblBufferPlot
	m_bRectConfinedClient = plot->m_bRectConfinedClient;
	m_rectConfinedClient = plot->m_rectConfinedClient;
}

void	CDblBufferPlot::CreateVirtualWindow()
{
	// code for virtual window
	// initialize vitual window
	HDC hDC=GetDC(m_hWnd);
	HBITMAP hMembmp;
	
	int w = m_bForCtrl?(m_rectConfinedClient.right-m_rectConfinedClient.left):m_maxX;
	int h = m_bForCtrl?(m_rectConfinedClient.bottom-m_rectConfinedClient.top):m_maxY;

	hMembmp=CreateCompatibleBitmap(hDC,w,h);
	m_hMemDC=CreateCompatibleDC(hDC); 
	SelectObject(m_hMemDC,hMembmp); 
	RECT rect; 
	SetRect(&rect,0,0,w,h);
	HBRUSH hBrush;
	hBrush = CreateSolidBrush(RGB(255,255,255)); 
	SelectObject(m_hMemDC,hBrush); 
	FillRect(m_hMemDC,&rect,hBrush);
	DeleteObject(hBrush);
	DeleteObject(hMembmp);
	
	ReleaseDC(m_hWnd,hDC);
	// code for virtual window ended
}

void	CDblBufferPlot::ReleaseVirtualWindow()
{
	// code for virtual window
	if(m_hMemDC)
	{
		DeleteDC(m_hMemDC);
		m_hMemDC = 0;
	}
	// code for virtual window ended
}

