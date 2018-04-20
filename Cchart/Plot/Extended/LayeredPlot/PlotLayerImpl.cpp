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
#include "PlotLayerImpl.h"
#include "LayeredPlotImpl.h"

CPlotLayerImpl::CPlotLayerImpl(CLayeredPlot *pParent) : CPlotLayer(pParent)
{
}

CPlotLayerImpl::~CPlotLayerImpl()
{
	
}

CPlotLayerImpl& CPlotLayerImpl::operator=(const CPlotLayerImpl& rhs)
{
	CPlot::CopySettings(&rhs);
	CXYPlot::CopyAll(&rhs);
	return *this;
}

int		CPlotLayerImpl::RegionIdentify( HDC hDC, POINT point )
{
	CAxis *pAxis=0;
	RECT theRect, plotRect;
	
	CLayeredPlotImpl *pP=(CLayeredPlotImpl *)m_pParent;
	plotRect = pP->GetPlotRect(hDC,pP->GetLastClientRect() ); 
	//data region
	theRect= plotRect;
	if( PtInRect(&theRect,point) )
		return kXYRegionData;
	//title region
	theRect=pP->GetTitleRect(hDC,pP->GetLastClientRect(), plotRect);
	if( PtInRect(&theRect,point) )
		return kXYRegionTitle;
	//axis region
	CAxis *axis;
	SIZE pAddingSize;
	for(int i=0;i<GetAxisCount();i++)
	{
		axis=GetAxis(i);
		theRect=((CAxisImpl*)axis)->GetPlotRect(hDC, GetLastClientRect(), GetPseudoPlotRect(hDC,pP->GetLastClientRect() ));
		//Modify the extended region
		pAddingSize = ((CAxisImpl*)axis)->GetPaddingSize(hDC);
		switch(axis->GetLocation())
		{
		case kLocationLeft:
		case kLocationRight:
			theRect.top = plotRect.top - pAddingSize.cx;
			theRect.bottom = plotRect.bottom + pAddingSize.cy;
			break;
		case kLocationTop:
		case kLocationBottom:
			theRect.left = plotRect.left - pAddingSize.cx;
			theRect.right = plotRect.right + pAddingSize.cy;
			break;
		}
		if( PtInRect(&theRect,point) )
		{
			switch(axis->GetLocation())
			{
			case kLocationLeft:return kXYRegionLeft;
			case kLocationRight:return kXYRegionRight;
			case kLocationBottom:return kXYRegionBottom;
			case kLocationTop:return kXYRegionTop;
			}
		}
	}
	return kXYRegionOut;
}

SIZE	CPlotLayerImpl::GetAxisSize( HDC hDC, int location )
{
	SIZE size={0,0};
	CAxis		*axis=GetAxisByLocation(location);
	if(axis==NULL)return size;
	
	SIZE		axisDims, paddingDims;
	axisDims = ((CAxisImpl *)axis)->GetMinDisplaySize( hDC );
	paddingDims = ((CAxisImpl *)axis)->GetPaddingSize( hDC );
	size=axisDims;
	
	switch(location)
	{
	case kLocationBottom:		
	case kLocationTop:
		size.cx+=(paddingDims.cx+paddingDims.cy);
		size.cy=axisDims.cy;
		break;
	case kLocationLeft:
	case kLocationRight:
		size.cy+=(paddingDims.cx+paddingDims.cy);
		size.cx=axisDims.cx;
		break;
	default: break;
	}
	
	return size;
}

void	CPlotLayerImpl::OnDraw( HWND hWnd )
{
	HDC hDC = GetDC(hWnd);
	RECT destRect;
	GetClientRect(hWnd,&destRect);
	
	OnDraw( hDC, destRect );
	
	ReleaseDC(hWnd, hDC);
}

void	CPlotLayerImpl::OnDraw( HDC hDC, RECT destRect )
{
	if(m_vpXYDataInfo.size()<=0)
	{
		EraseBkgnd( hDC, destRect );
		return;
	}
	
	double	xRange[] = {1e30, -1e30}, yRange[]={1e30,-1e30};
	GetPlotRange( xRange, yRange );

	// Draw title
	RECT titleRect;
	if( IsTop() )
	{
		titleRect = ((CLayeredPlotImpl*)m_pParent)->GetTitleRect(hDC, destRect, destRect);
		DrawTitle(hDC, titleRect);
	}

	// Draw the subtitle
	RECT subtitleRect;
	if(m_bSubtitleShow && IsTop() )
	{
		subtitleRect = GetSubtitleRect(hDC, destRect);
		DrawSubtitle(hDC, subtitleRect );
	}

	// Draw the grids
	if(IsTop())
		DrawGrids(hDC, destRect);
	
	// Draw Data Sets
	DrawDataSets( hDC, destRect, xRange, yRange );

	// Draw Axes
	CAxis *axis,*axisOther;
	RECT psPlotRect;
	SIZE sizeAdd={0,0},sizeTmp;
	int location;
	for( int i = 0; i < GetAxisCount(); i++ )
	{
		axis = GetAxis(i);
		if(!axis->IsVisible())continue;
		
		psPlotRect = destRect;
		location = axis->GetLocation();

		sizeAdd.cx = sizeAdd.cy = 0;
		for(int j=0; j<m_nLayerIndex; j++)
		{
			axisOther = m_pParent->GetLayer(j)->GetAxisByLocation(location);
			if(!axisOther)continue;
			sizeTmp = ((CAxisImpl *)axisOther)->GetSize(hDC);
			sizeAdd.cx += sizeTmp.cx;
			sizeAdd.cy += sizeTmp.cy;
		}
		switch(location)
		{
		case kLocationLeft:
			psPlotRect.left -= sizeAdd.cx;
			break;
		case kLocationRight:
			psPlotRect.right += sizeAdd.cx;
			break;
		case kLocationTop:
			psPlotRect.top -= sizeAdd.cy;
			break;
		case kLocationBottom:
			psPlotRect.bottom += sizeAdd.cy;
			break;
		default:
			break;
		}
		((CAxisImpl*)axis)->OnDraw( hDC, destRect, psPlotRect );
	}
	
	// Draw Legend
	if(IsTop())
		if(m_bUseLegend)m_pLegend->OnDraw(hDC,destRect);

	SetLastClientRect(destRect);
	m_LastPlotRect = destRect;
}

void	CPlotLayerImpl::DataToLP( HDC hDC, double *data, LPPOINT point )
{
	RECT		plotRect, tempRect;
	double		xRange[2], yRange[2];
	double		scaleX, scaleY;
	
	if( point == NULL || hDC == NULL ) return;
	
	tempRect = GetLastClientRect() ;
	//NormalizeRect(tempRect);
	if( IsRectEmpty(&tempRect) ) return;
	
	//plotRect = GetPlotRect( pDC, m_rctLastClientRect  );
	plotRect = m_LastPlotRect;
	
	GetRange( xRange, yRange );
	
	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);
	
	if( scaleX == 0. || scaleY == 0. ) return;
	
	point->x = (long)((data[0]-xRange[0]) / scaleX) + plotRect.left;
	point->y = -(long)((data[1]-yRange[0]) / scaleY) + plotRect.bottom;
}

void	CPlotLayerImpl::LPToData( HDC hDC, LPPOINT point, double *data )
{
	RECT		plotRect, tempRect;
	double		xRange[2], yRange[2];
	double		scaleX, scaleY;
	
	if( point == NULL || hDC == NULL ) return;
	
	tempRect = GetLastClientRect() ;
	//tempRect.NormalizeRect();
	if( IsRectEmpty( &tempRect) ) return;
	
	//plotRect = GetPlotRect( pDC, m_rctLastClientRect  );
	plotRect = m_LastPlotRect;
	
	GetRange( xRange, yRange );
	
	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);
	
	if( scaleX == 0. || scaleY == 0. ) return;
	
	data[0] = (point->x-plotRect.left) * scaleX + xRange[0];
	data[1] = -(point->y-plotRect.bottom) * scaleY + yRange[0];
}

RECT	CPlotLayerImpl::GetPseudoPlotRect( HDC hDC, RECT clientRect )
{
	CAxis *axis;
	RECT psPlotRect;
	SIZE sizeAdd;

	psPlotRect = ((CLayeredPlotImpl *)m_pParent)->GetPlotRect( hDC, clientRect);
	for(int j=0; j<m_nLayerIndex; j++)
	{
		for(int i=0; i<m_pParent->GetLayer(j)->GetAxisCount(); i++)
		{
			axis = m_pParent->GetLayer(j)->GetAxis(i);
			if(!axis->IsVisible())continue;
			sizeAdd = ((CAxisImpl *)axis)->GetSize(hDC);
			switch(axis->GetLocation())
			{
			case kLocationLeft:
				psPlotRect.left-=sizeAdd.cx;
				break;
			case kLocationRight:
				psPlotRect.right+=sizeAdd.cx;
				break;
			case kLocationTop:
				psPlotRect.top-=sizeAdd.cy;
				break;
			case kLocationBottom:
				psPlotRect.bottom+=sizeAdd.cy;
				break;
			default:
				break;
			}
		}
		
	}
	return psPlotRect;
}
