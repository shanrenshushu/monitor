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
#include "ShareXPlotImpl.h"
#include "ShareXLegendImpl.h"
#include "ShareXLayerImpl.h"
#include "../../Basics/PlotUtility.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//

CShareXPlotImpl::CShareXPlotImpl()
{
	if(m_pLegend)
	{
		DeleteLegend();
	}
	m_pLegend = new CShareXLegendImpl(this);

	CAxis *axis;
	axis = CPlot::AddAxis(kLocationBottom);
	axis = CPlot::AddAxis(kLocationLeft);
	axis->SetVisible(false);
}

CShareXPlotImpl::~CShareXPlotImpl()
{
	ClearLayers();
}

bool	CShareXPlotImpl::IsEmpty()
{
	if(m_vpLayers.size()<=0)return true;
	for(int i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CShareXLayerImplI*)m_vpLayers[i])->IsEmpty())return false;
	}
	return true;
}

bool	CShareXPlotImpl::NeedDrawing()
{
	if(IsEmpty())return false;
	for(int i=0;i<(int)m_vpLayers.size();i++)
	{
		if(((CShareXLayerImplI*)m_vpLayers[i])->NeedDrawing())return true;
	}
	return false;
}

CAxis*	CShareXPlotImpl::AddAxis( int nLayerIdx, int location )
{
	if( nLayerIdx<0 || nLayerIdx>=this->GetLayersCount() )return NULL;
	return ((CShareXLayerImplI *)m_vpLayers[nLayerIdx])->AddAxis( location );
}

CAxis*	CShareXPlotImpl::GetAxis( int nLayerIdx, int location )
{
	if( nLayerIdx<0 || nLayerIdx>=this->GetLayersCount() )return NULL;
	return ((CShareXLayerImplI *)m_vpLayers[nLayerIdx])->GetAxis( location );
}

int		CShareXPlotImpl::GetLocalIdx(int which, int &idxOfLayer, int &idxInLayer)
{
	if(which<0 || which>=GetDataSetCount())
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		return -1;
	}
	int count=0,cntOfLayer,dataID;
	for(int i=0;i<GetLayersCount();i++)
	{
		cntOfLayer=((CShareXLayerImplI *)m_vpLayers[i])->GetDataSetCount();
		if(count<=which && count+cntOfLayer>which)
		{
			idxOfLayer=i;
			idxInLayer=which-count;
			dataID=((CShareXLayerImplI *)m_vpLayers[idxOfLayer])->GetDataID(idxInLayer);
			return dataID;
		}
		count+=cntOfLayer;
	}
	return -1;
}

int		CShareXPlotImpl::GetDataSetCount( void ) const
{
	int count = 0;
	for(int i=0; i<(int)m_vpLayers.size(); i++)
	{
		count+=((CShareXLayerImplI *)m_vpLayers[i])->GetDataSetCount();
	}
	return count;
}

CPlotData<DataPoint2D>	*CShareXPlotImpl::GetAbsoluteDataSet( int which )
{
	if(which<0 || which>GetDataSetCount())return NULL;
	int count = 0, cnt0=0;
	for(int i=0; i<(int)m_vpLayers.size(); i++)
	{
		count+=((CShareXLayerImplI *)m_vpLayers[i])->GetDataSetCount();
		if(which>=cnt0 && which<count)
		{
			return ((CShareXLayerImplI *)m_vpLayers[i])->GetAbsoluteDataSet(which-cnt0);
		}
		cnt0+=((CShareXLayerImplI *)m_vpLayers[i])->GetDataSetCount();
	}
	return NULL;
}

int		CShareXPlotImpl::GetVisibleDataSetCount(  )
{
	int count = 0;
	for(int i=0; i<(int)m_vpLayers.size(); i++)
	{
		count+=((CShareXLayerImplI *)m_vpLayers[i])->GetVisibleDataSetCount();
	}
	return count;
}

void	CShareXPlotImpl::GetPlotRange( double xRange[2], double yRange[2] )
{
	xRange[0]=-1.0e30;
	xRange[1]=1.0e30;
	yRange[0]=-1.0e30;
	yRange[1]=1.0e30;
	
	double xrng[2],yrng[2];
	
	int i;
	bool set=false;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CShareXLayerImplI*)m_vpLayers[i])->NeedDrawing())continue;
		if(!set)
		{
			((CShareXLayerImplI*)m_vpLayers[i])->GetPlotRange( xRange, yRange );
			((CShareXLayerImplI*)m_vpLayers[i])->GetDataRanges( xRange, yRange );
			set=true;
			continue;
		}
		
		((CShareXLayerImplI*)m_vpLayers[i])->GetPlotRange( xrng, yrng );
		((CShareXLayerImplI*)m_vpLayers[i])->GetDataRanges( xrng, yrng );
		if(xrng[0]<xRange[0])xRange[0]=xrng[0];
		if(xrng[1]>xRange[1])xRange[1]=xrng[1];
		if(yrng[0]<yRange[0])yRange[0]=yrng[0];
		if(yrng[1]>yRange[1])yRange[1]=yrng[1];
	}

	int nTicks,nMinorTicks;
	GetOptimalRange(xRange, xrng, &nTicks, &nMinorTicks, true);
	memcpy(xRange, xrng, 2*sizeof(double));
	
	CPlot::SetXRange( xRange[0],xRange[1] );
	SetYRange(yrng[0], yrng[1]);
	CAxis *axis;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CShareXLayerImplI*)m_vpLayers[i])->NeedDrawing())continue;
		axis=((CShareXLayerImplI *)m_vpLayers[i])->AddAxis(kLocationBottom);
		((CShareXLayerImplI *)m_vpLayers[i])->SetXRange(xRange[0],xRange[1]);
		((CShareXLayerImplI *)m_vpLayers[i])->SetYAutoRange();
		((CShareXLayerImplI *)m_vpLayers[i])->GetPlotRange(xrng,yrng);
	}
}


int		CShareXPlotImpl::AddCurve(double *pX, double *pY, int nLen, int nLayerIdx)
{
	if(nLayerIdx < 0 || nLayerIdx >= GetLayersCount() )return -1;
	int dataID = ((CShareXLayerImplI*)m_vpLayers[nLayerIdx])->AddCurve(pX,pY,nLen);

	m_vpLayers[nLayerIdx]->UniteToColor(colorTableForPlot[nLayerIdx%NUMOFCOLORS]);

	double xRange[2],yRange[2];
	GetPlotRange(xRange,yRange);

	return dataID;
}

SIZE	CShareXPlotImpl::GetAxisSize( HDC hDC, int location )
{
	SIZE axisSize={0,0};
	SIZE dispSize,paddingSize,maxPaddingSize;
	int i;
	CAxis *axis;
	switch(location)
	{
	case kLocationBottom:
	case kLocationTop:
		axis=GetAxisByLocation(location);
		if(axis)
		{
			dispSize = ((CAxisImpl *)axis)->GetMinDisplaySize( hDC );
			paddingSize = ((CAxisImpl *)axis)->GetPaddingSize( hDC );
			axisSize = dispSize;
			axisSize.cx += (paddingSize.cx+paddingSize.cy);
		}
		break;
	case kLocationLeft:
	case kLocationRight:
		for(i=0;i<GetLayersCount();i++)
		{
			if(!m_vpLayers[i]->NeedDrawing())continue;
			SIZE tmp={0,0};
			maxPaddingSize=tmp;
			axis=m_vpLayers[i]->GetAxisByLocation(location);
			if(axis)
			{
				dispSize = ((CAxisImpl *)axis)->GetMinDisplaySize( hDC );
				paddingSize = ((CAxisImpl *)axis)->GetPaddingSize( hDC );
				axisSize.cx += dispSize.cx;
				if( dispSize.cy > axisSize.cy ) axisSize.cy = dispSize.cy;
				if( paddingSize.cx > maxPaddingSize.cx ) maxPaddingSize.cx = paddingSize.cx;
				if( paddingSize.cy > maxPaddingSize.cy ) maxPaddingSize.cy = paddingSize.cy;
			}
		}
		axisSize.cy+=(maxPaddingSize.cx+maxPaddingSize.cy);
		break;
	default:
		break;
	}
	return axisSize;
}

SIZE	CShareXPlotImpl::GetAxisPaddingSize( HDC hDC, int location )
{
	SIZE paddingSize,maxPaddingSize={0,0};
	int i;
	CAxis *axis;
	switch(location)
	{
	case kLocationBottom:
	case kLocationTop:
		axis=GetAxisByLocation(location);
		if(axis)
		{
			maxPaddingSize = ((CAxisImpl *)axis)->GetPaddingSize( hDC );
		}
		break;
	case kLocationLeft:
	case kLocationRight:
		for(i=0;i<GetLayersCount();i++)
		{
			if(!m_vpLayers[i]->NeedDrawing())continue;
			axis=m_vpLayers[i]->GetAxisByLocation(location);
			if(axis)
			{
				paddingSize = ((CAxisImpl *)axis)->GetPaddingSize( hDC );

				if( paddingSize.cx > maxPaddingSize.cx ) maxPaddingSize.cx = paddingSize.cx;
				if( paddingSize.cy > maxPaddingSize.cy ) maxPaddingSize.cy = paddingSize.cy;
			}
		}
		break;
	default:
		break;
	}
	return maxPaddingSize;
}
/*
SIZE	CShareXPlotImpl::GetTitleSize( HDC hDC )
{
	SIZE size={0,0};
	
	tstring strTitle=GetTitle();
	if( strTitle != "" )
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
*/
RECT	CShareXPlotImpl::GetPlotRect( HDC hDC, RECT clientRect)
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
	sizeRect = GetAxisSize( hDC, kLocationTop);
	plotRect.top+=sizeRect.cy;
	
	// then bottom axis
	sizeRect = GetAxisSize( hDC, kLocationBottom);
	plotRect.bottom-=sizeRect.cy;
	
	// then left axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CShareXLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		sizeTmp=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationLeft );
		sizeRect.cx+=sizeTmp.cx;
	}
	plotRect.left+=sizeRect.cx;
	
	// then right axis
	sizeRect.cx = sizeRect.cy = 0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CShareXLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		sizeTmp=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationRight );
		sizeRect.cx+=sizeTmp.cx;
	}
	plotRect.right-=sizeRect.cx;
	
	// Add spacing
	InflateRect( &plotRect, -10, -10 );
	
	return plotRect;
}

RECT	CShareXPlotImpl::GetTitleRect( HDC hDC, RECT clientRect, RECT chartRect )
{
	RECT	titleRect=clientRect;
	SIZE	tmSize={0,0},axSize={0,0};
	if( GetTitle() != _TEXT("") )
	{
		HFONT		hOldFont, hFont;
		
		hFont = CreateFontIndirect(&GetTitleFont());
		hOldFont = (HFONT)SelectObject(hDC, hFont);
		GetTextExtentPoint(hDC, GetTitle().c_str(),GetTitle().length(),&tmSize);
		SelectObject( hDC, hOldFont );
		DeleteObject(hFont);
	}
	
	axSize = GetAxisSize( hDC, kLocationTop );
	
	titleRect.bottom=chartRect.top-axSize.cy;
	titleRect.top=titleRect.bottom-tmSize.cy;
	titleRect.left=chartRect.left;
	titleRect.right=chartRect.right;
	
	return titleRect;
}

void	CShareXPlotImpl::OnDraw( HWND hWnd )
{
	HDC hDC = GetDC(hWnd);
	RECT destRect;
	GetClientRect(hWnd,&destRect);
	
	OnDraw( hDC, destRect );
	
	ReleaseDC(hWnd, hDC);
}

void	CShareXPlotImpl::OnDraw( HDC hDC, RECT destRect)
{
	//if there is no data, doing nothing
	if(!NeedDrawing())
	{
		EraseBkgnd( hDC, destRect );
		return;
	}

	int i;

	// Get number of left axis and right axis
	CAxis *axis=NULL;

	int numLeft=0,numRight=0;
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CShareXLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;

		axis=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisByLocation(kLocationLeft);
		if(axis)numLeft++;
		axis=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisByLocation(kLocationRight);
		if(axis)numRight++;
	}

	double xRange[2], yRange[2];
	GetPlotRange(xRange,yRange);

	// get plot rect
	RECT plotRect=GetPlotRect(hDC,destRect);

	// Erase the background using parent function
	EraseBkgnd( hDC, destRect );

	// First Draw Frame
	DrawFrame(hDC, plotRect);

	// Draw the grids using the parent function
	DrawGrids( hDC, plotRect );

	// The draw title
	RECT titleRect = GetTitleRect( hDC, destRect, plotRect);
	DrawTitle(hDC, titleRect);

	// then bottom and top axis;
	axis=GetAxisByLocation(kLocationBottom);
	if(axis)
	{
		((CAxisImpl *)axis)->OnDraw(hDC,destRect,plotRect);
	}
	axis=GetAxisByLocation(kLocationTop);
	if(axis)
	{
		((CAxisImpl *)axis)->OnDraw(hDC,destRect,plotRect);
	}
	// then left and right axis;
	SIZE axisSize;
	RECT newRect=plotRect;
	
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CShareXLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;

		axis=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisByLocation(kLocationLeft);
		{
			if(axis)
			{
				((CAxisImpl *)axis)->OnDraw(hDC,destRect,newRect);
				
				axisSize=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationLeft );
				newRect.left-=axisSize.cx;
			}
		}
		
		axis=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisByLocation(kLocationRight);
		{
			if(axis)
			{
				((CAxisImpl *)axis)->OnDraw(hDC,destRect,newRect);

				axisSize=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationRight );
				newRect.right+=axisSize.cx;
			}
		}
	}

	
	// Draw each layer,only draw curve
	for(i=0;i<(int)m_vpLayers.size();i++)
	{
		if(!((CShareXLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		((CShareXLayerImplI *)m_vpLayers[i])->OnDraw( hDC, plotRect );
	}

	// Legend should be drawn after the datasets have drawn for the setting of line color etc
	// Draw Legend
	if(IsUseLegend())m_pLegend->OnDraw(hDC, plotRect);

	SetLastClientRect(destRect);
 
}

bool	CShareXPlotImpl::IsAxisLighted()
{
	if(CPlot::IsAxisLighted())return true;
	
	for(int i=0;i<GetLayersCount();i++)
	{
		if(((CShareXLayerImplI *)m_vpLayers[i])->IsAxisLighted())return true;
	}
	
	return false;
}

void	CShareXPlotImpl::UnlightAllAxis()
{
	CPlot::UnlightAllAxis();
	
	for(int i=0;i<GetLayersCount();i++)
	{
		((CShareXLayerImplI *)m_vpLayers[i])->UnlightAllAxis();
	}
	
}

bool	CShareXPlotImpl::IsAxisSelected()
{
	if(CPlot::IsAxisSelected())return true;
	
	for(int i=0;i<GetLayersCount();i++)
	{
		if(((CShareXLayerImplI *)m_vpLayers[i])->IsAxisSelected())return true;
	}
	
	return false;
}

void	CShareXPlotImpl::UnselectAllAxis()
{
	CPlot::UnselectAllAxis();
	
	for(int i=0;i<GetLayersCount();i++)
	{
		((CShareXLayerImplI *)m_vpLayers[i])->UnselectAllAxis();
	}
	
}

int		CShareXPlotImpl::RegionIdentify(HDC hDC,POINT point)
{
	//CAxis *pAxis=0;
	RECT theRect;
	
	//data region
	theRect=GetPlotRect(hDC,GetLastClientRect() );
	if( PtInRect(&theRect, point) )
		return kXYRegionData;
	//title region
	theRect=GetTitleRect(hDC,GetLastClientRect() ,GetPlotRect(hDC,GetLastClientRect() ));
	if( PtInRect(&theRect, point) )
		return kXYRegionTitle;
	//top and bottom axis region
	CAxisImpl *axis;
	for(int i=0;i<CPlot::GetAxisCount();i++)
	{
		axis=(CAxisImpl *)CPlot::GetAxis(i);
		theRect=axis->GetPlotRect(hDC,GetLastClientRect() ,GetPlotRect(hDC,GetLastClientRect() ));
		if( PtInRect(&theRect, point) )
		{
			if(axis->GetLocation()==kLocationBottom)return kXYRegionBottom;
			if(axis->GetLocation()==kLocationTop)return kXYRegionTop;
		}
	}
	
	//left and right axis region
	theRect=GetPlotRect(hDC,GetLastClientRect() );
	SIZE axisSize,padddingSize;
	RECT axisRect;
	
	axisSize = GetAxisSize(hDC,kLocationLeft);
	padddingSize = GetAxisPaddingSize(hDC, kLocationLeft);
	axisRect.right = theRect.left;
	axisRect.left = axisRect.right - axisSize.cx;
	axisRect.top = theRect.top - padddingSize.cx;
	axisRect.bottom = theRect.bottom + padddingSize.cy;
	if( PtInRect(&axisRect, point) )
		return kXYRegionLeft;
	
	axisSize = GetAxisSize(hDC,kLocationRight);
	padddingSize = GetAxisPaddingSize(hDC, kLocationRight);
	axisRect.left = theRect.right;
	axisRect.right = axisRect.left + axisSize.cx;
	axisRect.top = theRect.top - padddingSize.cx;
	axisRect.bottom = theRect.bottom + padddingSize.cy;
	if( PtInRect(&axisRect, point) )
		return kXYRegionRight;
	
	return kXYRegionOut;
}

int		CShareXPlotImpl::LightByPoint( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer )
{
	if( RegionIdentify(hDC,point) != kXYRegionData )
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		return -1;
	}
	
	int n=((CShareXLegendImpl *)GetLegend())->RegionIdentify(hDC,point);
	int dataID;
	if(n>=0)
	{
		dataID=GetLocalIdx(n,idxOfLayer,idxInLayer);
		if(dataID<0)
		{
			return -1;
		}
		goto LightCurveByPoint;
	}
	
	dataID= GetNeareastDataSet( hDC, point, idxOfLayer, idxInLayer );
LightCurveByPoint:
	if(dataID>=0)
	{
		((CShareXLayerImplI *)m_vpLayers[idxOfLayer])->SetLighted(dataID,true);
		return dataID;
	}
	else
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		UnlightAllDataSets();
		return -1;
	}
}

int		CShareXPlotImpl::SelectByPoint( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer )
{
	if( RegionIdentify(hDC,point) != kXYRegionData )
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		return -1;
	}
	
	int n=((CShareXLegendImpl *)GetLegend())->RegionIdentify(hDC,point);
	int dataID;
	if(n>=0)
	{
		dataID=GetLocalIdx(n,idxOfLayer,idxInLayer);
		if(dataID<0)
		{
			return -1;
		}
		goto SelectCurveByPoint;
	}
	
	dataID = GetNeareastDataSet( hDC, point, idxOfLayer, idxInLayer );
	
SelectCurveByPoint:
	if(dataID>=0)
	{
		((CShareXLayerImplI *)m_vpLayers[idxOfLayer])->SetSelected(dataID,!((CShareXLayerImplI *)m_vpLayers[idxOfLayer])->IsSelected(dataID));
		return dataID;
	}
	else
	{
		idxOfLayer=-1;
		idxInLayer=-1;
		SelectAllDataSets(false);
		return -1;
	}
}

int		CShareXPlotImpl::GetNeareastDataSet( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer )
{
	int dataID;
	for(int i=0;i<GetLayersCount();i++)
	{
		dataID=((CShareXLayerImplI *)m_vpLayers[i])->GetNeareastDataSet(hDC,point);
		if(dataID>=0)
		{
			idxOfLayer=i;
			idxInLayer=((CShareXLayerImplI *)m_vpLayers[idxOfLayer])->GetIndex(dataID);
			if(idxInLayer>=0)return dataID;
		}
	}
	idxOfLayer=-1;
	idxInLayer=-1;
	return -1;
}

RECT	CShareXPlotImpl::GetLayerAxisRect( int idxOfLayer, int location, HDC hDC, RECT plotRect )
{
	CAxis *axis;
	SIZE axisSize,paddingSize;
	RECT axisRect={0,0,0,0};
	RECT newRect=plotRect;
	
	if(!((CShareXLayerImplI *)m_vpLayers[idxOfLayer])->NeedDrawing())return axisRect;
	if(location==kLocationBottom || location==kLocationTop)return axisRect;
	
	for(int i=0;i<idxOfLayer;i++)
	{
		if(!((CShareXLayerImplI *)m_vpLayers[i])->NeedDrawing())continue;
		
		axis=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisByLocation(kLocationLeft);
		{
			if(axis)
			{
				axisSize=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationLeft );
				newRect.left-=axisSize.cx;
			}
		}
		axis=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisByLocation(kLocationRight);
		{
			if(axis)
			{
				axisSize=((CShareXLayerImplI *)m_vpLayers[i])->GetAxisSize( hDC, kLocationRight );
				newRect.right+=axisSize.cx;
			}
		}
	}
	
	axis=((CShareXLayerImplI *)m_vpLayers[idxOfLayer])->GetAxisByLocation(location);
	{
		if(axis)
		{
			axisSize = ((CAxisImpl *)axis)->GetMinDisplaySize( hDC );
			paddingSize = ((CAxisImpl *)axis)->GetPaddingSize( hDC );
			
			axisRect=newRect;
			if(location==kLocationLeft)
			{
				axisRect.right=newRect.left;
				axisRect.left=axisRect.right-axisSize.cx;
			}
			else
			{
				axisRect.left=newRect.right;
				axisRect.right=axisRect.left+axisSize.cx;
			}
			axisRect.bottom+=paddingSize.cy;
			axisRect.top-=paddingSize.cx;
		}
	}
	return axisRect;
}

int		CShareXPlotImpl::GetIdxOfLayerByPointInAxisRegion(HDC hDC, RECT plotRect, POINT point)
{
	int i,region=RegionIdentify(hDC,point);
	RECT axisRect;
	if( region==kXYRegionLeft )
	{
		for(i=0;i<GetLayersCount();i++)
		{
			axisRect=GetLayerAxisRect(i,kLocationLeft,hDC,plotRect);
			if(IsRectEmpty(&axisRect))continue;
			if( PtInRect(&axisRect, point) )
			{
				return i;
			}
		}
	}
	else if( region==kXYRegionRight )
	{
		for(i=0;i<GetLayersCount();i++)
		{
			axisRect=GetLayerAxisRect(i,kLocationRight,hDC,plotRect);
			if(IsRectEmpty(&axisRect))continue;
			if( PtInRect(&axisRect, point) )
			{
				return i;
			}
		}
	}
	return -1;
}

bool	CShareXPlotImpl::IsDataSetLighted()
{
	for(int i=0;i<GetLayersCount();i++)
		if(((CShareXLayerImplI *)m_vpLayers[i])->IsDataSetLighted())return true;
	return false;
}
bool	CShareXPlotImpl::IsDataSetSelected()
{
	for(int i=0;i<GetLayersCount();i++)
		if(((CShareXLayerImplI *)m_vpLayers[i])->IsDataSetSelected())return true;
	return false;
}
void	CShareXPlotImpl::UnlightAllDataSets()
{
	for(int i=0;i<GetLayersCount();i++)
		((CShareXLayerImplI *)m_vpLayers[i])->UnlightAllDataSets();
}
void	CShareXPlotImpl::SelectAllDataSets(bool bSel)
{
	for(int i=0;i<GetLayersCount();i++)
		((CShareXLayerImplI *)m_vpLayers[i])->SelectAllDataSets(bSel);
}

/*
bool	CShareXPlotImpl::IsLayerSelected()
{
	bool bSel = false;
	for(int i=0; i<GetLayersCount(); i++)
	{
		if(GetLayer(i)->IsPlotSelected())
		{
			bSel = true;
			break;
		}
	}
	return bSel;
}

bool	CShareXPlotImpl::IsPlotSelected()
{
	return IsSelectTitle() || IsLayerSelected();
}
*/

int		CShareXPlotImpl::GetAxisCount( )
{
	CPlotImpl *pPlot;
	int location;
	int i, j, count = 0;
	for(i=0; i<=GetLayersCount(); i++)
	{
		if(i == GetLayersCount())
		{
			pPlot = this;
		}
		else
		{
			pPlot = GetLayer(i);
		}

		for(j=0; j<CPlot::GetAxisCount(); j++)
		{
			location = pPlot->GetAxis(j)->GetLocation();
			if(location == kLocationBottom || location == kLocationBottom
				|| location == kLocationCenterHB || location == kLocationCenterHT)
			{
				count++;
			}
		}
	}
	
	return count;
}

int		CShareXPlotImpl::GetVisibleAxisCount( )
{
	CPlotImpl *pPlot;
	int location;
	int i, j, count = 0;
	for(i=0; i<=GetLayersCount(); i++)
	{
		if(i == GetLayersCount())
		{
			pPlot = this;
		}
		else
		{
			pPlot = GetLayer(i);
		}
		
		for(j=0; j<CPlot::GetAxisCount(); j++)
		{
			if(!pPlot->GetAxis(j)->IsVisible())continue;

			location = pPlot->GetAxis(j)->GetLocation();
			if(location == kLocationBottom || location == kLocationBottom
				|| location == kLocationCenterHB || location == kLocationCenterHT)
			{
				count++;
			}
		}
	}
	
	return count;
}

int		CShareXPlotImpl::GetIndexOfLightedAxis(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;

	CPlotImpl *pPlot;
	int i, j;
	for(i=0; i<=GetLayersCount(); i++)
	{
		if(i==GetLayersCount())
		{
			pPlot = this;
		}
		else
		{
			pPlot = GetLayer(i);
		}
		for(j=0; j<pPlot->GetAxisCount(); j++)
		{
			if(pPlot->GetAxis(j)->IsLighted())
			{
				idxOfLayer = i;
				idxInLayer = j;
				return idxInLayer;
			}
		}
	}
	return idxInLayer;
}
int		CShareXPlotImpl::GetIndexOfSelectedAxis(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	CPlotImpl *pPlot;
	int i, j;
	for(i=0; i<=GetLayersCount(); i++)
	{
		if(i==GetLayersCount())
		{
			pPlot = this;
		}
		else
		{
			pPlot = GetLayer(i);
		}
		for(j=0; j<pPlot->GetAxisCount(); j++)
		{
			if(pPlot->GetAxis(j)->IsSelected())
			{
				idxOfLayer = i;
				idxInLayer = j;
				return idxInLayer;
			}
		}
	}
	return idxInLayer;
}
int		CShareXPlotImpl::GetIndexOfLightedDataSet(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	for(i=0; i<GetLayersCount(); i++)
	{
		for(j=0; j<GetLayer(i)->GetDataSetCount(); j++)
		{
			if(GetLayer(i)->IsLighted(j))
			{
				idxOfLayer = i;
				idxInLayer = j;
				return GetLayer(i)->GetDataID(j);
			}
		}
	}
	return -1;
}
int		CShareXPlotImpl::GetIndexOfSelectedDataSet(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	for(i=0; i<GetLayersCount(); i++)
	{
		for(j=0; j<GetLayer(i)->GetDataSetCount(); j++)
		{
			if(GetLayer(i)->IsSelected(j))
			{
				idxOfLayer = i;
				idxInLayer = j;
				return GetLayer(i)->GetDataID(j);
			}
		}
	}
	return -1;
}

int		CShareXPlotImpl::GetIndexOfFirstVisibleAxis(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	int location;
	for(j=0; j<CPlot::GetAxisCount(); j++)
	{
		location = CPlot::GetAxis(j)->GetLocation();
		if(CPlot::GetAxis(j)->IsVisible())
		{
			if(location == kLocationBottom
				|| location == kLocationTop
				|| location == kLocationCenterHT
				|| location == kLocationCenterHB)
			{
				idxOfLayer = GetLayersCount();
				idxInLayer = j;
				return idxInLayer;
			}
		}
	}
	for(i=0; i<GetLayersCount(); i++)
	{
		for(j=0; j<GetLayer(i)->GetAxisCount(); j++)
		{
			location = GetLayer(i)->GetAxis(j)->GetLocation();
			if(GetLayer(i)->GetAxis(j)->IsVisible())
			{
				if(location == kLocationBottom
					|| location == kLocationTop
					|| location == kLocationCenterHT
					|| location == kLocationCenterHB)
				{
					idxOfLayer = i;
					idxInLayer = j;
					return idxInLayer;
				}
			}
		}
	}
	return idxInLayer;
}
int		CShareXPlotImpl::GetIndexOfLastVisibleAxis(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	int location;
	for(i=GetLayersCount()-1; i>=0; i--)
	{
		for(j=GetLayer(i)->GetAxisCount()-1; j>=0; j--)
		{
			location = GetLayer(i)->GetAxis(j)->GetLocation();
			if(GetLayer(i)->GetAxis(j)->IsVisible())
			{
				if(location == kLocationBottom
					|| location == kLocationTop
					|| location == kLocationCenterHT
					|| location == kLocationCenterHB)
				{
					idxOfLayer = i;
					idxInLayer = j;
					return idxInLayer;
				}
			}
		}
	}
	for(j=CPlot::GetAxisCount()-1; j>=0; j--)
	{
		location = CPlot::GetAxis(j)->GetLocation();
		if(CPlot::GetAxis(j)->IsVisible())
		{
			if(location == kLocationBottom
				|| location == kLocationTop
				|| location == kLocationCenterHT
				|| location == kLocationCenterHB)
			{
				idxOfLayer = GetLayersCount();
				idxInLayer = j;
				return idxInLayer;
			}
		}
	}
	return idxInLayer;
}
int		CShareXPlotImpl::GetIndexOfFirstVisibleDataSet(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	for(i=0; i<GetLayersCount(); i++)
	{
		for(j=0; j<GetLayer(i)->GetDataSetCount(); j++)
		{
			if(GetLayer(i)->IsVisible(GetLayer(i)->GetDataID(j)))
			{
				idxOfLayer = i;
				idxInLayer = j;
				return GetLayer(i)->GetDataID(j);
			}
		}
	}
	return -1;
}
int		CShareXPlotImpl::GetIndexOfLastVisibleDataSet(int &idxOfLayer, int &idxInLayer)
{
	idxOfLayer = -1;
	idxInLayer = -1;
	
	int i, j;
	for(i=GetLayersCount()-1; i>=0; i--)
	{
		for(j=GetLayer(i)->GetDataSetCount()-1; j>=0; j--)
		{
			if(GetLayer(i)->IsVisible(GetLayer(i)->GetDataID(j)))
			{
				idxOfLayer = i;
				idxInLayer = j;
				return GetLayer(i)->GetDataID(j);
			}
		}
	}
	return -1;
}

int		CShareXPlotImpl::GetIndexOfNextVisibleAxis(int &idxOfLayer, int &idxInLayer)
{
	if(idxOfLayer > GetLayersCount())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else if(idxOfLayer == GetLayersCount()-1 && idxInLayer == GetLayer(idxOfLayer)->GetIndexOfLastVisibleAxis())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else
	{
		int location;
		int i, j;
		if(idxOfLayer == GetLayersCount())
		{
			j = idxInLayer<0?0:idxInLayer+1;
			while(j<CPlot::GetAxisCount())
			{
				location = CPlot::GetAxis(j)->GetLocation();
				if(location == kLocationBottom
					|| location == kLocationTop
					|| location == kLocationCenterHB
					|| location == kLocationCenterHT)
				{
					if(CPlot::GetAxis(j)->IsVisible())
					{
						//idxOfLayer = i;
						idxInLayer = j;
						return idxInLayer;
					}
					
				}
				j++;
			}
		}
		i = ( idxOfLayer<0 || idxOfLayer == GetLayersCount() )?0:idxOfLayer;
		j = idxInLayer<0?0:idxInLayer+1;
		while(i<GetLayersCount())
		{
			while(j < GetLayer(i)->GetAxisCount())
			{
				location = GetLayer(i)->GetAxis(j)->GetLocation();
				if(location == kLocationLeft
					|| location == kLocationRight
					|| location == kLocationCenterVL
					|| location == kLocationCenterVR)
				{
					if(GetLayer(i)->GetAxis(j)->IsVisible())
					{
						idxOfLayer = i;
						idxInLayer = j;
						return idxInLayer;
					}
				}
				j++;
			}
			i++;
			j=0;
		}
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
}
int		CShareXPlotImpl::GetIndexOfPrevVisibleAxis(int &idxOfLayer, int &idxInLayer)
{
	if(idxOfLayer >= GetLayersCount())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else if(idxOfLayer == 0 && idxInLayer == GetLayer(idxOfLayer)->GetIndexOfFirstVisibleAxis())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else
	{
		int location;
		int i, j;
		
		if(idxOfLayer != GetLayersCount())
		{
			i = idxOfLayer<0 ?0:idxOfLayer;
			j = idxInLayer<0?GetLayer(idxOfLayer)->GetAxisCount()-1:idxInLayer-1;
			while(i>=0)
			{
				while(j>=0)
				{
					location = GetLayer(i)->GetAxis(j)->GetLocation();
					if(location == kLocationLeft
						|| location == kLocationRight
						|| location == kLocationCenterVL
						|| location == kLocationCenterVR)
					{
						if(GetLayer(i)->GetAxis(j)->IsVisible())
						{
							idxOfLayer = i;
							idxInLayer = j;
							return idxInLayer;
						}
					}
					j--;
				}
				i--;
				if(i>=0)
					j = GetLayer(i)->GetAxisCount()-1;
			}
		}
		
		j = idxInLayer<0?CPlot::GetAxisCount()-1:idxInLayer-1;
		while(j>=0)
		{
			location = CPlot::GetAxis(j)->GetLocation();
			if(location == kLocationBottom
				|| location == kLocationTop
				|| location == kLocationCenterHB
				|| location == kLocationCenterHT)
			{
				if(CPlot::GetAxis(j)->IsVisible())
				{
					idxOfLayer = GetLayersCount();
					idxInLayer = j;
					return idxInLayer;
				}
			}
			j--;
		}
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
}
int		CShareXPlotImpl::GetIndexOfNextVisibleDataSet(int &idxOfLayer, int &idxInLayer)
{
	if(idxOfLayer >= GetDataSetCount())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else if(idxOfLayer == GetDataSetCount()-1 && idxInLayer == GetLayer(idxOfLayer)->GetIndexOfLastVisibleDataSet())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else
	{
		int i, j;
		i = idxOfLayer<0?0:idxOfLayer;
		j = idxInLayer<0?0:idxInLayer+1;
		while(i<GetLayersCount())
		{
			while(j<GetLayersCount())
			{
				if(GetLayer(i)->IsVisible(j))
				{
					idxOfLayer = i;
					idxInLayer = j;
					return idxInLayer;
				}
				j++;
			}
			i++;
			j=0;
		}
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
}
int		CShareXPlotImpl::GetIndexOfPrevVisibleDataSet(int &idxOfLayer, int &idxInLayer)
{
	if(idxOfLayer >= GetDataSetCount())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else if(idxOfLayer == 0 && idxInLayer == GetLayer(idxOfLayer)->GetIndexOfFirstVisibleDataSet())
	{
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
	else
	{
		int i, j;
		i = idxOfLayer<0?GetLayersCount()-1:idxOfLayer;
		j = idxInLayer<0?GetLayer(i)->GetDataSetCount()-1:idxInLayer-1;
		while(i<GetLayersCount())
		{
			while(j<GetLayersCount())
			{
				if(GetLayer(i)->IsVisible(j))
				{
					idxOfLayer = i;
					idxInLayer = j;
					return idxInLayer;
				}
				j--;
			}
			i--;
			if(i>=0)
				j = GetLayer(i)->GetDataSetCount()-1;
		}
		idxOfLayer = -1;
		idxInLayer = -1;
		return idxInLayer;
	}
}

bool	CShareXPlotImpl::SelectAxis(int &idxOfLayer, int &idxInLayer, bool bSel)
{
	if(idxOfLayer == GetLayersCount())
	{
		if(idxInLayer>=0 && idxInLayer<CPlot::GetAxisCount())
		{
			CPlot::GetAxis(idxInLayer)->SetSelected(bSel);
			return true;
		}
		return false;
	}
	else if(idxOfLayer<0 || idxOfLayer>GetLayersCount())
	{
		return false;
	}
	else
	{
		if(idxInLayer>=0 && idxInLayer<GetLayer(idxOfLayer)->GetAxisCount())
		{
			GetLayer(idxOfLayer)->GetAxis(idxInLayer)->SetSelected(bSel);
			return true;
		}
		return false;
	}
}
bool	CShareXPlotImpl::SelectDataSet(int &idxOfLayer, int &idxInLayer, bool bSel)
{
	if(idxOfLayer<0 || idxOfLayer>GetLayersCount())
	{
		return false;
	}
	else
	{
		if(idxInLayer>=0 && idxInLayer<GetLayer(idxOfLayer)->GetDataSetCount())
		{
			GetLayer(idxOfLayer)->SetSelected(GetLayer(idxOfLayer)->GetDataID(idxInLayer),bSel);
			return true;
		}
		return false;
	}
}

void	CShareXPlotImpl::SwitchSelAxes()
{
	if(GetVisibleAxisCount()<=0)
	{
		if(!m_bSwitchReverse)
			SwitchSelDatasets();
		else
			SwitchSelTitle();
	}
	else
	{
		int idxInLayer, idxOfLayer;
		GetIndexOfSelectedAxis(idxOfLayer, idxInLayer);
		int idxIn = idxInLayer, idxOf = idxOfLayer;
		if(idxOfLayer<0 && idxInLayer<0)idxOf = CPlot::GetAxisCount();
		
		if(!m_bSwitchReverse)
		{
			GetIndexOfNextVisibleAxis(idxOf, idxIn);
			if(idxOf<0)
				SwitchSelDatasets();
			SelectAxis(idxOfLayer, idxInLayer, false);
			SelectAxis(idxOf, idxIn, true);
		}
		else
		{
			GetIndexOfPrevVisibleAxis(idxOf, idxIn);
			if(idxOfLayer<0)
				SwitchSelTitle();
			SelectAxis(idxOfLayer, idxInLayer, false);
			SelectAxis(idxOf, idxIn, true);
		}
	}
}

void		CShareXPlotImpl::SwitchSelDatasets()
{
	if(GetVisibleAxisCount()<=0)
	{
		if(!m_bSwitchReverse)
			SwitchSelTitle();
		else
			SwitchSelAxes();
	}
	else
	{
		int idxInLayer, idxOfLayer;
		GetIndexOfSelectedDataSet(idxOfLayer, idxInLayer);
		int idxIn = idxInLayer, idxOf = idxOfLayer;
		
		if(!m_bSwitchReverse)
		{
			GetIndexOfNextVisibleDataSet(idxOf, idxIn);
			if(idxOf<0)
				SwitchSelTitle();
			SelectDataSet(idxOfLayer, idxInLayer, false);
			SelectDataSet(idxOf, idxIn, true);
		}
		else
		{
			GetIndexOfPrevVisibleDataSet(idxOf, idxIn);
			if(idxOfLayer<0)
				SwitchSelAxes();
			SelectDataSet(idxOfLayer, idxInLayer, false);
			SelectDataSet(idxOf, idxIn, true);
		}
	}
}

void		CShareXPlotImpl::SwitchSelTitle()
{
	if(!IsTitleRealyShow())
	{
		if(!m_bSwitchReverse)
			SwitchSelAxes();
		else
			SwitchSelDatasets();
	}
	else if(IsSelectTitle())
	{
		SetSelectTitle(false);
		if(!m_bSwitchReverse)
			SwitchSelAxes();
		else
			SwitchSelDatasets();
	}
	else
	{
		SetSelectTitle(true);
	}
}