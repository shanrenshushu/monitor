/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2012 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */

#ifndef __LAYEREDPLOTIMPL_H_122333444455555__
#define __LAYEREDPLOTIMPL_H_122333444455555__

#include "LayeredPlot.h"
#include "../../Basics/XYPlotImpl.h"

class CLayeredPlotImpl : public CLayeredPlot, public CXYPlotImpl
{
public:
	CLayeredPlotImpl();
	virtual ~CLayeredPlotImpl();
public:
	bool		IsEmpty();
	bool		NeedDrawing();

	int			AddCurve(double *pX, double *pY, int nLen, int nLayerIdx);

	void		SetTitle(tstring title){CPlot::SetTitle(title);}
	void		SetTitle(tstring title, int index){if(index>=0 && index<GetLayersCount())m_vpLayers[index]->SetTitle(title);}

public:
	void		OnDraw( HWND hWnd );
	void		OnDraw( HDC hDC, RECT destRect);
	
public:
	SIZE		GetTitleSize( HDC hDC );
	RECT		GetTitleRect( HDC hDC, RECT clientRect, RECT chartRect );
	RECT		GetPlotRect( HDC hDC, RECT clientRect);

public:
	void		SwitchSelection();
};

template <typename LayeredPlotImplT>
class CLayeredHandler : public CXYHandler<LayeredPlotImplT>
{
public:
	CLayeredHandler();
	virtual		~CLayeredHandler();
public:
	bool		OnLButtonDown( HWND hWnd, POINT point );
	bool		OnLButtonUp( HWND hWnd, POINT point );
	bool		OnLButtonDblClk( HWND hWnd, POINT point );
	bool		OnMouseMove( HWND hWnd, POINT point );
	bool		OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	bool		OnKeyDown( HWND hWnd, UINT key );
	
	void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
};

class CLayeredPlotImplI : public CLayeredPlotImpl, public CLayeredHandler<CLayeredPlotImplI>
{

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

template <typename LayeredPlotImplT>
CLayeredHandler<LayeredPlotImplT>::CLayeredHandler()
{
}

template <typename LayeredPlotImplT>
CLayeredHandler<LayeredPlotImplT>::~CLayeredHandler()
{
}

template <typename LayeredPlotImplT>
bool	CLayeredHandler<LayeredPlotImplT>::OnLButtonDown( HWND hWnd, POINT point )
{
	LayeredPlotImplT *pT = (LayeredPlotImplT *)this;
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(pT->GetLayersCount()<=0)return false;
	
	int top = pT->GetTop();
	if(top<0 || top>= pT->GetLayersCount())top=0;
	for(int i=0; i<pT->GetLayersCount(); i++)
	{
		if(i!=top)pT->GetLayer(i)->UnselectAll();
	}
	return pT->GetLayer(top)->OnLButtonDown(hWnd, point);
}

template <typename LayeredPlotImplT>
bool	CLayeredHandler<LayeredPlotImplT>::OnLButtonUp( HWND hWnd, POINT point )
{
	LayeredPlotImplT *pT = (LayeredPlotImplT *)this;
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(pT->GetLayersCount()<=0)return false;
	
	int top = pT->GetTop();
	if(top<0 || top>= pT->GetLayersCount())top=0;
	return pT->GetLayer(top)->OnLButtonUp(hWnd, point);
}

template <typename LayeredPlotImplT>
bool	CLayeredHandler<LayeredPlotImplT>::OnLButtonDblClk( HWND hWnd, POINT point )
{
	LayeredPlotImplT *pT = (LayeredPlotImplT *)this;
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(pT->GetLayersCount()<=0)return false;
	
	int top = pT->GetTop();
	if(top<0 || top>= pT->GetLayersCount())top=0;
	return pT->GetLayer(top)->OnLButtonDblClk(hWnd, point);
}

template <typename LayeredPlotImplT>
bool	CLayeredHandler<LayeredPlotImplT>::OnMouseMove( HWND hWnd, POINT point )
{
	LayeredPlotImplT *pT = (LayeredPlotImplT *)this;
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(pT->GetLayersCount()<=0)return false;

	int i;
	bool ret=false;

	// Deal with title
	HDC hDC = GetDC( hWnd );
	int region = pT->RegionIdentify(hDC, point);
	ReleaseDC(hWnd, hDC);
	if(region == kXYRegionTitle)
	{
		//pT->SetLightTitle(true);
		int top = pT->GetTop();
		if(top < 0)return false;;
		pT->SetTop(top, true);
		pT->GetLayer(top)->SetLightTitle(true);
		return true;
	}
	else if(region == kXYRegionOut)
	{
		for(i=0; i<pT->GetLayersCount(); i++)
		{
			pT->GetLayer(i)->SetLightTitle(false);
		}
		ret = true;;
	}

	// use layer interactive
	for(i=0; i<pT->GetLayersCount(); i++)
	{
		if(pT->GetLayer(i)->OnMouseMove(hWnd, point))
		{
			pT->SetTop(i, true);
			for(int j=0; j<pT->GetLayersCount(); j++)
			{
				if(j!=i)pT->GetLayer(j)->UnlightAll();
			}
			return true;
		}
	}
	return ret;
}

template <typename LayeredPlotImplT>
bool	CLayeredHandler<LayeredPlotImplT>::OnContextMenu( HMENU hMenu, HWND hWnd, POINT point )
{
	LayeredPlotImplT *pT = (LayeredPlotImplT *)this;

	POINT pt = point;
	ScreenToClient(hWnd, &pt);
	if(!PtInRect(&pT->GetLastClientRect(), pt))return false;

	if(pT->GetLayersCount()<=0)return false;
	
	int top = pT->GetTop();
	if(top<0 || top>= pT->GetLayersCount())top=0;
	return pT->GetLayer(top)->OnContextMenu(hMenu, hWnd, point);
}

template <typename LayeredPlotImplT>
bool	CLayeredHandler<LayeredPlotImplT>::OnKeyDown( HWND hWnd, UINT key )
{
	LayeredPlotImplT *pT = (LayeredPlotImplT *)this;
	if(pT->GetLayersCount()<=0)return false;

	int top;
	switch(key)
	{
	case VK_TAB:
		pT->SwitchSelection();
		return true;
	default:
		top = pT->GetTop();
		if(top<0 || top>= pT->GetLayersCount())top=0;
		return pT->GetLayer(top)->OnKeyDown(hWnd, key);
	}
	
	
}

template <typename LayeredPlotImplT>
void	CLayeredHandler<LayeredPlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LayeredPlotImplT *pT = (LayeredPlotImplT *)this;
	if(pT->GetLayersCount()<=0)return;
	
	int top = pT->GetTop();
	if(top<0 || top>= pT->GetLayersCount())top=0;
	pT->GetLayer(top)->CXYHandler::Interactive(hWnd, message, wParam, lParam);
}

#endif