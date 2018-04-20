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

//#include "StdAfx.h"
#include "LayeredPlotImpl.h"
#include "../../Basics/PlotUtility.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//

CLayeredPlotImpl::CLayeredPlotImpl()
{
}

CLayeredPlotImpl::~CLayeredPlotImpl()
{
	ClearLayers();
}

bool	CLayeredPlotImpl::IsEmpty()
{
	if(m_vpLayers.size()<=0)return true;
	for(int i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CPlotLayerImplI*)m_vpLayers[i])->IsEmpty())return false;
	}
	return true;
}

bool	CLayeredPlotImpl::NeedDrawing()
{
	if(IsEmpty())return false;
	for(int i=0;i<(int)m_vpLayers.size();i++)
	{
		if(((CPlotLayerImplI*)m_vpLayers[i])->NeedDrawing())return true;
	}
	return false;
}

int		CLayeredPlotImpl::AddCurve(double *pX, double *pY, int nLen, int nLayerIdx)
{
	if(nLayerIdx < 0 || nLayerIdx >= GetLayersCount() )return -1;
	int dataID = ((CPlotLayerImplI*)m_vpLayers[nLayerIdx])->AddCurve(pX,pY,nLen);
	
	double xRange[2],yRange[2];
	m_vpLayers[nLayerIdx]->GetPlotRange(xRange,yRange);

	m_vpLayers[nLayerIdx]->UniteToColor(colorTableForPlot[nLayerIdx%NUMOFCOLORS]);
	
	return dataID;
}
/*
int		CLayeredPlotImpl::AddLayer()
{
	CPlotLayerImplI *tmp = new CPlotLayerImplI(this);
	tmp->GetLayerIndex() = (int)m_vpLayers.size();
	m_vpLayers.push_back(tmp);	
	SetTop((int)m_vpLayers.size()-1, true);
	return m_vpLayers.size();
}
*/
SIZE	CLayeredPlotImpl::GetTitleSize( HDC hDC )
{
	SIZE size={0,0};
	tstring strTitle;
	int top = GetTop();
	if(top < 0)
		strTitle=GetTitle();
	else
		strTitle=GetLayer(top)->GetTitle();
	if( strTitle != _TEXT("") )
	{
		HFONT		hOldFont, hFont;
		hFont = CreateFontIndirect(&GetTitleFont());
		hOldFont = (HFONT)SelectObject(hDC, hFont);
		//size = pDC->GetTextExtent( strTitle );
		GetTextExtentPoint( hDC, strTitle.c_str(), strTitle.length(), &size);
		SelectObject( hDC, hOldFont );
		DeleteObject(hFont);
	}
	
	return size;
}

RECT	CLayeredPlotImpl::GetTitleRect( HDC hDC, RECT clientRect, RECT chartRect )
{
	int top = GetTop();
	if(top < 0)
		return CPlotImpl::GetTitleRect(hDC, clientRect, chartRect);
	else
		return GetLayer(top)->GetTitleRect(hDC, clientRect, chartRect);
}

RECT	CLayeredPlotImpl::GetPlotRect( HDC hDC, RECT clientRect)
{
	//////////////////////////////////////////////////////////////
	// Get Plot Rect
	int i;
	
	RECT plotRect=clientRect;
	SIZE sizeRect={0,0},sizeTmp={0,0};
	
	// First title
	sizeRect=GetTitleSize(hDC);
	plotRect.top+=sizeRect.cy;
	
	// then top axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CPlotLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		sizeTmp=((CPlotLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationTop );
		sizeRect.cy+=sizeTmp.cy;
	}
	plotRect.top+=sizeRect.cy;
	
	// then bottom axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CPlotLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		sizeTmp=((CPlotLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationBottom );
		sizeRect.cy+=sizeTmp.cy;
	}
	plotRect.bottom-=sizeRect.cy;
	
	// then left axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CPlotLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		sizeTmp=((CPlotLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationLeft );
		sizeRect.cx+=sizeTmp.cx;
	}
	plotRect.left+=sizeRect.cx;
	
	// then right axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CPlotLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		sizeTmp=((CPlotLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationRight );
		sizeRect.cx+=sizeTmp.cx;
	}
	plotRect.right-=sizeRect.cx;
	
	// Add spacing
	InflateRect( &plotRect, -10, -10 );
	
	return plotRect;
}

void	CLayeredPlotImpl::SwitchSelection()
{
	if(GetLayersCount()<=0)return;
	int top = GetTop(), num = GetLayersCount();
	if(!m_bSwitchReverse)
	{
		if(top < 0)
		{
			top = 0;
			SetTop(top, true);
			return;
		}
		top++;
		if(top >= num)
			top = 0;
	}
	else
	{
		if(top > num)
		{
			top = num-1;
			SetTop(top, true);
			return;
		}
		top--;
		if(top < 0)
			top = num-1;
	}
	SetTop(top, true);
}

void	CLayeredPlotImpl::OnDraw( HWND hWnd )
{
	HDC hDC = GetDC(hWnd);
	RECT destRect;
	GetClientRect(hWnd,&destRect);
	
	OnDraw( hDC, destRect );
	
	ReleaseDC(hWnd, hDC);
}

void	CLayeredPlotImpl::OnDraw( HDC hDC, RECT destRect)
{
	//if there is no data, doing nothing
	if(!NeedDrawing())
	{
		EraseBkgnd( hDC, destRect );
		return;
	}

	int i;

	// get plot rect
	RECT plotRect=GetPlotRect(hDC,destRect);

	// Erase the background using parent function
	EraseBkgnd( hDC, destRect );

	// First Draw Frame
	DrawFrame(hDC, plotRect);

	// Draw the grids using the parent function
	//DrawGrids( hDC, plotRect );

	// The draw title
	int top=GetTop();
	if(top<0)
	{
		RECT titleRect = GetTitleRect( hDC, destRect, plotRect);
		DrawTitle(hDC, titleRect);
	}

	// Draw each layer
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CPlotLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		((CPlotLayerImplI *)m_vpLayers[i])->OnDraw( hDC, plotRect );
		((CPlotLayerImplI *)m_vpLayers[i])->SetLastClientRect(destRect);
	}

	SetLastClientRect(destRect);
}