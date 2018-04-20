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

#ifndef __SHAREXPLOTIMPL_H_122333444455555__
#define __SHAREXPLOTIMPL_H_122333444455555__

#include "../../Basics/XYPlotImpl.h"
#include "../../Basics/XYHandler.h"
#include "ShareXPlot.h"
#include "ShareXLayerImpl.h"
#include "ShareXLegendImpl.h"

class CShareXPlotImpl : public CShareXPlot, public CXYPlotImpl
{
public:
	CShareXPlotImpl();
	virtual ~CShareXPlotImpl();

	bool		IsEmpty();
	bool		NeedDrawing();

	int			AddCurve(double *pX, double *pY, int nLen, int nLayerIdx);

	void		GetPlotRange( double xRange[2], double yRange[2] );

	int			GetDataSetCount( void ) const;
	CPlotData<DataPoint2D>	*GetAbsoluteDataSet( int which );
	int			GetVisibleDataSetCount();

	CAxis		*AddAxis( int nLayerIdx, int location );
	CAxis		*GetAxis( int nLayerIdx, int location );

public:
	void		OnDraw( HWND hWnd );
	void		OnDraw( HDC hDC, RECT destRect);
	
	SIZE		GetAxisSize( HDC hDC, int location );
	SIZE		GetAxisPaddingSize( HDC hDC , int location );
	//SIZE		GetTitleSize( HDC hDC );
	RECT		GetPlotRect( HDC hDC, RECT clientRect);
	RECT		GetTitleRect( HDC hDC, RECT clientRect, RECT chartRect );

public:
	int			GetLocalIdx(int which, int &idxOfLayer, int &idxInLayer);

	int			RegionIdentify(HDC hDC,POINT point);

	bool		IsDataSetLighted();
	bool		IsDataSetSelected();
	void		UnlightAllDataSets();
	void		SelectAllDataSets(bool bSel);
	
	bool		IsAxisLighted();
	void		UnlightAllAxis();
	bool		IsAxisSelected();
	void		UnselectAllAxis();
	int			GetIndexOfLightedAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfSelectedAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfLightedDataSet(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfSelectedDataSet(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfFirstVisibleAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfLastVisibleAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfFirstVisibleDataSet(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfLastVisibleDataSet(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfNextVisibleAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfPrevVisibleAxis(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfNextVisibleDataSet(int &idxOfLayer, int &idxInLayer);
	int			GetIndexOfPrevVisibleDataSet(int &idxOfLayer, int &idxInLayer);
	bool		SelectAxis(int &idxOfLayer, int &idxInLayer, bool bSel);
	bool		SelectDataSet(int &idxOfLayer, int &idxInLayer, bool bSel);
	
	int			LightByPoint( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer );
	int			SelectByPoint( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer );
	
	int			GetNeareastDataSet( HDC hDC, POINT point, int &idxOfLayer, int &idxInLayer );
		
	RECT		GetLayerAxisRect( int idxOfLayer, int location, HDC hDC, RECT plotRect );
	int			GetIdxOfLayerByPointInAxisRegion(HDC hDC, RECT plotRect, POINT point);

public:
	//void		SwitchSelection();
	void		SwitchSelAxes();
	void		SwitchSelDatasets();
	void		SwitchSelTitle();
private:
	//bool		IsLayerSelected();
	//bool		IsPlotSelected();
	int			GetAxisCount( );
	int			GetVisibleAxisCount();
};

template <typename ShareXPlotImplT>
class CShareXHandler : public CHandler
{
public:
	CShareXHandler();
	virtual ~CShareXHandler();
protected:
	FcnDataSettings		*m_pFcnDS;
	FcnAxisSettings		*m_pFcnAS;
	FcnPlotSettings		*m_pFcnPS;
	FcnShowPopupMenu	*m_pFcnPM;

	int			m_nTopLayer;
public:
	bool		OnLButtonDown( HWND hWnd, POINT point );
	bool		OnLButtonUp( HWND hWnd, POINT point );
	bool		OnLButtonDblClk( HWND hWnd, POINT point );
	bool		OnMouseMove( HWND hWnd, POINT point );
	bool		OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	bool		OnKeyDown( HWND hWnd, UINT key );
	
	//void		Interactive(HWND hWnd, UINT message, POINT point);
	void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

class CShareXPlotImplI : public CShareXPlotImpl, public CShareXHandler<CShareXPlotImplI>
{
	
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

template <typename ShareXPlotImplT>
CShareXHandler<ShareXPlotImplT>::CShareXHandler()
{
	m_pFcnDS = DefDataSettings;
	m_pFcnAS = DefAxisSettings;
	m_pFcnPS = DefPlotSettings;
	m_pFcnPM = DefContextMenu;

	m_nTopLayer = -1;
}

template <typename ShareXPlotImplT>
CShareXHandler<ShareXPlotImplT>::~CShareXHandler()
{
}

template <typename ShareXPlotImplT>
bool	CShareXHandler<ShareXPlotImplT>::OnLButtonDown( HWND hWnd, POINT point )
{
	ShareXPlotImplT *pT=static_cast<ShareXPlotImplT *>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(pT->IsEmpty()) return false;

	CShareXLayerImplI *pLayer=0;
	
	HDC hDC = GetDC( hWnd );

	// Capture
	SetCapture(hWnd);
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	ClientToScreen(hWnd,(LPPOINT)&clientRect);
	ClientToScreen(hWnd,(LPPOINT)&clientRect+1);
	ClipCursor(&clientRect);
	// End Capture

	int location;
	CAxis *axis;
	bool bSel;

	static int curve_id0=-1;
	bool needUpdate = false;
	int idxOfLayer,idxInLayer;
	
	DPtoLP( hDC, &point,1);
	
	//Set Drag mode
	int curve_id, which = ((CShareXLegendImpl *)(pT->GetLegend()))->RegionIdentify(hDC,point);
	if(which>=0)
	{
		pT->GetLegend()->SetDragMode(true,point);
		if(which == pT->GetDataSetCount())goto UnselectCurve;//in drag region
		curve_id=pT->GetLocalIdx(which,idxOfLayer,idxInLayer);
		m_nTopLayer = idxOfLayer;
		pLayer=pT->GetLayer(idxOfLayer);
		if(!pLayer)
		{
			needUpdate=false;
			goto UnselectCurve;
		}
		pLayer->SetSelected(pLayer->GetDataID(curve_id),!pLayer->IsSelected(pLayer->GetDataID(curve_id)));
		goto SelectCurve;
	}
	
	int region;
	region=pT->RegionIdentify(hDC,point);
	switch(region)
	{
	case kXYRegionTitle:
		bSel = pT->IsSelectTitle();
		pT->UnselectAll();
		pT->SetSelectTitle(!bSel);
		return true;
	case kXYRegionData:
		curve_id=pT->SelectByPoint(hDC, point, idxOfLayer, idxInLayer);
		m_nTopLayer = idxOfLayer;
SelectCurve:
		if(curve_id>=0 && curve_id != curve_id0)
		{
			bSel = pT->GetLayer(idxOfLayer)->IsSelected(curve_id);
			pT->UnselectAll();
			pT->GetLayer(idxOfLayer)->SetSelected(curve_id, bSel);
			curve_id0 = curve_id0;
			needUpdate = true;
		}
		else
		{
			goto UnselectCurve;
		}
		break;
	case kXYRegionBottom:
		location=kLocationBottom;
		goto SelectAxisTB;
	case kXYRegionTop:
		location=kLocationTop;
SelectAxisTB:
		axis = pT->GetAxisByLocation(location);
		if(axis)
		{
			bSel = axis->IsSelected();
			pT->UnselectAll();
			axis->SetSelected(!bSel);
			needUpdate = true;
		}
		break;
	case kXYRegionLeft:
		location=kLocationLeft;
		goto SelectAxisLR;
	case kXYRegionRight:
		location=kLocationRight;
SelectAxisLR:
		idxOfLayer = pT->GetIdxOfLayerByPointInAxisRegion( hDC, pT->GetPlotRect(hDC,pT->GetLastClientRect()),point);
		pLayer = pT->GetLayer(idxOfLayer);
		if(!pLayer)
		{
			needUpdate = false;
			goto lbuttondown_return;
		}
		if(idxOfLayer>=0)
		{
			axis = pLayer->GetAxisByLocation(location);
			if(axis)
			{
				bSel = axis->IsSelected();
				pT->UnselectAll();
				axis->SetSelected(!bSel);
				needUpdate = true;
			}
		}
		break;
	default:
UnselectCurve:
		pT->SelectAllDataSets(false);
		needUpdate = true;
	}
	
	LPtoDP( hDC, &point,1);
	
lbuttondown_return:
	ReleaseDC(hWnd,hDC);

	return needUpdate;
}

template <typename ShareXPlotImplT>
bool	CShareXHandler<ShareXPlotImplT>::OnLButtonUp( HWND hWnd, POINT point )
{
	ShareXPlotImplT *pT=static_cast<ShareXPlotImplT *>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;
	
	if((((CShareXLegendImpl *)pT->GetLegend()))->IsDragMode()) (((CShareXLegendImpl *)pT->GetLegend()))->SetDragMode(false,point);
	if(GetCapture())ReleaseCapture();
	ClipCursor(NULL);
		
	return false;
}

template <typename ShareXPlotImplT>
bool	CShareXHandler<ShareXPlotImplT>::OnLButtonDblClk( HWND hWnd, POINT point )
{
	ShareXPlotImplT *pT=static_cast<ShareXPlotImplT *>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(pT->IsEmpty()) return false;

	CShareXLayerImplI *pLayer;

	if( m_pFcnAS==NULL || m_pFcnDS==NULL || m_pFcnPS==NULL ) return false;

	HDC hDC = GetDC( hWnd );

	bool needUpdate = false;

	if(pT->IsEmpty()) goto lbuttondblclk_return;

	DPtoLP( hDC, &point,1);

	int which,curve_id;
	int idxOfLayer,idxInLayer;
	
	which=(((CLegendImpl *)pT->GetLegend()))->RegionIdentify(hDC,point);
	if(which >= 0)
	{
		if(which == pT->GetDataSetCount())goto lbuttondblclk_return;
		
		curve_id=pT->GetLocalIdx(which,idxOfLayer,idxInLayer);
		m_nTopLayer = idxOfLayer;
		if(curve_id>=0)
		{
			pLayer = pT->GetLayer(idxOfLayer);
			if(!pLayer)
			{
				needUpdate = false;
				goto lbuttondblclk_return;
			}
			pLayer->SetSelected(curve_id,!pLayer->IsSelected(curve_id));
		}
		goto CurveProperty;
	}

	int location,region;
	region=pT->RegionIdentify(hDC,point);
	switch(region)
	{
	case kXYRegionData:
		curve_id=pT->GetNeareastDataSet(hDC,point,idxOfLayer,idxInLayer);
		m_nTopLayer = idxOfLayer;
CurveProperty:
		if(curve_id>=0)
		{
			pLayer = pT->GetLayer(idxOfLayer);
			if(!pLayer)
			{
				needUpdate = false;
				goto lbuttondblclk_return;
			}
			which=pLayer->GetIndex(curve_id);
			//if(pLayer->m_pFcnDS(static_cast<ShareXPlotImplT *>(this),hWnd,which))
			if(pLayer->m_pFcnDS(static_cast<CShareXLayerImpl *>(pLayer),hWnd,which))
			{
				needUpdate = true;
			}
		}
		else
		{
			goto PlotProperty;
		}
		break;
	case kXYRegionBottom:
		location=kLocationBottom;
		goto SetAxisTB;
	case kXYRegionTop:
		location=kLocationTop;
SetAxisTB:
		if((*m_pFcnAS)(static_cast<ShareXPlotImplT *>(this),hWnd,location))
		{
			needUpdate = true;
		}
		break;
	case kXYRegionLeft:
		location=kLocationLeft;
		goto SetAxisLR;
	case kXYRegionRight:
		location=kLocationRight;
SetAxisLR:
		idxOfLayer = pT->GetIdxOfLayerByPointInAxisRegion( hDC, pT->GetPlotRect(hDC,pT->GetLastClientRect()),point);
		pLayer = pT->GetLayer(idxOfLayer);
		if(!pLayer)
		{
			needUpdate = false;
			goto lbuttondblclk_return;
		}
		if(idxOfLayer>=0 && pLayer->m_pFcnAS(static_cast<CShareXLayerImpl *>(pLayer),hWnd,location))
		{
			needUpdate = true;
		}
		break;
	default:

PlotProperty:
		if((*m_pFcnPS)(static_cast<ShareXPlotImplT *>(this),hWnd))
		{
			needUpdate = true;
		}
		break;
	}

	LPtoDP( hDC, &point,1);

lbuttondblclk_return:

	ReleaseDC(hWnd,hDC);

	return needUpdate;
}

template <typename ShareXPlotImplT>
bool	CShareXHandler<ShareXPlotImplT>::OnMouseMove( HWND hWnd, POINT point )
{
	ShareXPlotImplT *pT=static_cast<ShareXPlotImplT *>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(pT->IsEmpty()) return false;

	CShareXLayerImplI *pLayer;

	HDC hDC = GetDC( hWnd );

	static int curve_id0=-1,layer_idx0_cv=-1;
	static int loc0,layer_idx0_ax=-1;
	static bool light_title0=false;
	static bool light_dataregion0=false;

	bool needUpdate = false;

	if(pT->IsEmpty()) goto mousemove_return;
	
	DPtoLP( hDC, &point,1);

	if(pT->GetLegend()->IsDragMode())
	{
		pT->GetLegend()->SetCurrentPoint(point);
		needUpdate = true;
		goto mousemove_return;
	}

	int which,curve_id;
	int idxOfLayer,idxInLayer;

	CAxis *axis;
	int location;
	bool light_title,light_dataregion;
	int region;
	region=pT->RegionIdentify(hDC,point);
	switch(region)
	{
	case kXYRegionData:
		if(pT->IsAxisLighted())
		{
			pT->UnlightAllAxis();
			if(loc0>=0 || layer_idx0_ax>=0)
			{
				needUpdate = true;
			}
			loc0=-1;
			layer_idx0_ax=-1;
		}
		//如果处于legend区域
		which=((CLegendImpl *)(pT->GetLegend()))->RegionIdentify(hDC,point);
		if(which>=0)
		{
			if(which<pT->GetDataSetCount())
			{
				curve_id=pT->GetLocalIdx(which,idxOfLayer,idxInLayer);
				m_nTopLayer = idxOfLayer;
			}
			else//处于Drag区域
			{
				curve_id=-1;
				needUpdate=true;
				m_nTopLayer = -1;
			}
			pT->GetLegend()->SetLighted(true);
		}
		else
		{
			//curve_id=CLayeredPlotImpl::LightByPoint(hDC,point,idxOfLayer,idxInLayer);
			curve_id=pT->LightByPoint(hDC,point,idxOfLayer,idxInLayer);
			m_nTopLayer = idxOfLayer;
			if(pT->GetLegend()->IsLighted())needUpdate=true;//考虑Drag区域
			pT->GetLegend()->SetLighted(false);
		}

		if(curve_id>=0)
		{
			if(curve_id!=curve_id0 || idxOfLayer!=layer_idx0_cv)
			{
				pT->UnlightAllDataSets();
				pLayer = pT->GetLayer(idxOfLayer);
				if(!pLayer)
				{
					needUpdate = false;
					goto mousemove_return;
				}
				pLayer->SetLighted(curve_id,true);
				needUpdate = true;
			}
		}
		else
		{
			goto SameAsOutRegion;
		}
		curve_id0=curve_id;
		layer_idx0_cv=idxOfLayer;
		break;
	case kXYRegionBottom:
		location=kLocationBottom;
		goto LightAxisTB;
	case kXYRegionTop:
		location=kLocationTop;		
LightAxisTB:
		axis=pT->GetAxisByLocation(location);
		m_nTopLayer = -1;
		if(axis)
		{
			axis->SetLighted(true);
			if(location!=loc0)
			{
				needUpdate = true;
			}
			loc0=location;
		}
		break;
	case kXYRegionLeft:
		location=kLocationLeft;
		goto LightAxisLR;
	case kXYRegionRight:
		location=kLocationRight;
LightAxisLR:
		idxOfLayer=pT->GetIdxOfLayerByPointInAxisRegion(hDC, pT->GetPlotRect(hDC,pT->GetLastClientRect()),point);
		m_nTopLayer = idxOfLayer;
		if(idxOfLayer<0)
		{
			needUpdate = false;
			goto mousemove_return;
		}
		pLayer = pT->GetLayer(idxOfLayer);
		if(!pLayer)
		{
			needUpdate = false;
			goto mousemove_return;
		}
		axis=pLayer->GetAxisByLocation(location);
		if(axis)
		{
			pT->UnlightAllAxis();
			axis->SetLighted(true);
			if(location!=loc0 || idxOfLayer!=layer_idx0_ax )
			{
				needUpdate = true;
			}
			loc0=location;
			layer_idx0_ax=idxOfLayer;
		}
		break;
	default:
		// unlight axis
		if(pT->IsAxisLighted())
		{
			pT->UnlightAllAxis();
			if( loc0>=0 || layer_idx0_ax>=0 )
			{
				needUpdate = true;
			}
			loc0=-1;
			layer_idx0_ax=-1;
		}
SameAsOutRegion:
		pT->UnlightAllDataSets();
		if(curve_id0>=0 || layer_idx0_cv>=0)
		{
			needUpdate = true;
		}
		
		curve_id0=-1;
		layer_idx0_cv=-1;
		layer_idx0_ax=-1;
		break;
	}

	if(region == kXYRegionTitle)
	{		
		light_title=true;
		if(light_title != light_title0)
		{
			pT->SetLightTitle(true);
			needUpdate = true;
		}
		light_title0=light_title;
	}
	else
	{
		// unlight title
		if(pT->IsLightTitle())
		{
			pT->SetLightTitle(false);
			if(light_title0)
			{
				needUpdate = true;
			}
			light_title0=false;
		}
	}

	if(region == kXYRegionData)
	{		
		light_dataregion=true;
		if(light_dataregion != light_dataregion0)
		{
			pT->SetLightDataRegion(true);
			needUpdate = true;
		}
		light_dataregion0=light_dataregion;
	}
	else
	{
		// unlight data region
		if(pT->IsLightDataRegion())
		{
			pT->SetLightDataRegion(false);
			if(light_dataregion0)
			{
				needUpdate = true;
			}
			light_dataregion0=false;
		}
	}

	double data[2];
	pT->LPToData( hDC, &point, data);
	//SetCursor(data);
	//needUpdate = true;

	LPtoDP( hDC, &point,1);

mousemove_return:

	ReleaseDC(hWnd,hDC);
	return needUpdate;
}

template <typename ShareXPlotImplT>
bool	CShareXHandler<ShareXPlotImplT>::OnContextMenu( HMENU hMenu, HWND hWnd, POINT point )
{
	ShareXPlotImplT *pT=static_cast<ShareXPlotImplT *>(this);

	POINT pt = point;
	ScreenToClient(hWnd, &pt);
	if(!PtInRect(&pT->GetLastClientRect(), pt))return false;

	if(m_nTopLayer<0)
	{
		if( m_pFcnPM == NULL ) return false;
		if( (*m_pFcnPM)( pT, hMenu, hWnd, point )==kPopupUpdate )
		{
			return true ;
		}
		else
			return false;
	}
	else
	{
		if( pT->GetLayer(m_nTopLayer)->m_pFcnPM == NULL ) return false;
		if( (*pT->GetLayer(m_nTopLayer)->m_pFcnPM)( pT->GetLayer(m_nTopLayer), hMenu, hWnd, point )==kPopupUpdate )
		{
			return true ;
		}
		else
			return false;
	}
	
}

template <typename ShareXPlotImplT>
bool	CShareXHandler<ShareXPlotImplT>::OnKeyDown( HWND hWnd, UINT key )
{
	ShareXPlotImplT *pT=static_cast<ShareXPlotImplT *>(this);
	
	int idxOfLayer, idxInLayer;
	int location;

	bool needUpdate = false;
	switch(key)
	{
	case VK_SPACE:
	case VK_TAB:
		pT->SwitchSelection();
		needUpdate = true;
		break;
	case VK_RETURN:
		if(pT->IsSelectTitle())
		{
			if((*m_pFcnPS)(pT,hWnd))
			{
				needUpdate = true;
			}
		}
		else if(pT->IsAxisSelected())
		{
			pT->GetIndexOfSelectedAxis(idxOfLayer, idxInLayer);

			bool retV;
			if(idxOfLayer == pT->GetLayersCount())
			{
				location = pT->CPlot::GetAxis(idxInLayer)->GetLocation();
				retV = (*m_pFcnAS)(pT, hWnd, location);
			}
			else
			{
				location = pT->GetLayer(idxOfLayer)->GetAxis(idxInLayer)->GetLocation();
				retV = (*pT->GetLayer(idxOfLayer)->m_pFcnAS)(pT->GetLayer(idxOfLayer), hWnd, location);
			}

			if(retV)
			{
				needUpdate = true;
			}
		}
		else if(pT->IsDataSetSelected())
		{
			pT->GetIndexOfSelectedDataSet(idxOfLayer, idxInLayer);
			
			if((*pT->GetLayer(idxOfLayer)->m_pFcnDS)(pT->GetLayer(idxOfLayer),hWnd,idxInLayer))
			{
				needUpdate = true;
			}
		}
		else if((*m_pFcnPS)(pT,hWnd))
		{
			needUpdate = true;
		}
		break;
	default:
		needUpdate = false;
	}
	return needUpdate;
}

template <typename ShareXPlotImplT>
void	CShareXHandler<ShareXPlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT	point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	UINT	key = wParam;

	ShareXPlotImplT *pT=static_cast<ShareXPlotImplT *>(this);
	switch(message)
	{
	case WM_LBUTTONDOWN:
		if(OnLButtonDown(hWnd,point))pT->OnDraw(hWnd);
		break;
	case WM_LBUTTONUP:
		if(OnLButtonUp(hWnd,point))pT->OnDraw(hWnd);
		break;
	case WM_LBUTTONDBLCLK:
		if(OnLButtonDblClk(hWnd,point))pT->OnDraw(hWnd);
		break;
	case WM_MOUSEMOVE:
		if(OnMouseMove(hWnd,point))pT->OnDraw(hWnd);
		break;
	case WM_CONTEXTMENU:
		if(OnContextMenu(NULL, hWnd, point))pT->OnDraw(hWnd);
		break;
	case WM_KEYDOWN:
		if(OnKeyDown(hWnd, key))pT->OnDraw(hWnd);
		break;
	default:
		return;
	}
}


#endif