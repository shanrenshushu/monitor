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

#include "PlotInterface.h"
#include "Chart.h"

///////////////////////////////////////////////////////////////////////////////////////
// Class CChart
CChart::CChart()
{
	m_pPlot = 0;
	m_nType = -1;
}

CChart::~CChart()
{
	Release();
}

void	CChart::Release()
{
	if(m_pPlot)
	{
		delete (CPlotImpl *)m_pPlot;
		m_nType = -1;
	}
}

void	*CChart::GetPlot()
{
	return m_pPlot;
}

int		CChart::GetType()
{
	return m_nType;
}

bool	CChart::SetType(int nType)
{
	if(nType<0 || nType>=kTypeCount)
		return false;
	switch(nType)
	{
	case kTypeXY:
		m_pPlot = new XYChartID;
		break;
	case kTypeSplit:
		m_pPlot = new SplitChartID;
		break;
	case kTypeShareX:
		m_pPlot = new ShareXChartID;
		break;
	case kTypeLayered:
		m_pPlot = new LayeredChartID;
		break;
	default:
		return false;
	}
	m_nType = nType;
	return true;
}

bool	CChart::SetConfineRect(HWND hWnd, RECT ConfineRect)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return false;
	if(!m_pPlot)
		return false;

	switch(m_nType)
	{
	case kTypeXY:
		((XYChartID *)m_pPlot)->SetWnd(hWnd, ConfineRect);
		break;
	case kTypeSplit:
		((SplitChartID *)m_pPlot)->SetWnd(hWnd, ConfineRect);
		break;
	case kTypeShareX:
		((ShareXChartID *)m_pPlot)->SetWnd(hWnd, ConfineRect);
		break;
	case kTypeLayered:
		((LayeredChartID *)m_pPlot)->SetWnd(hWnd, ConfineRect);
		break;
	default:
		return false;
	}
	return true;
}

void	CChart::OnDraw(HWND hWnd)
{
	HDC hDC = GetDC(hWnd);
	RECT rt;
	GetClientRect(hWnd, &rt);
	OnDraw(hDC, rt);
	ReleaseDC(hWnd, hDC);
}

void	CChart::OnDraw(HDC hDC, RECT destRect)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;
	
	switch(m_nType)
	{
	case kTypeXY:
		((XYChartID*)m_pPlot)->OnDraw( hDC, destRect );
		return;
	case kTypeSplit:
		((SplitChartID*)m_pPlot)->OnDraw( hDC, destRect );
		return;
	case kTypeShareX:
		((ShareXChartID*)m_pPlot)->OnDraw( hDC, destRect );
		return;
	case kTypeLayered:
		((LayeredChartID*)m_pPlot)->OnDraw( hDC, destRect );
		return;
	}
}

bool	CChart::OnLButtonDown( HWND hWnd, POINT point )
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return false;
	if(!m_pPlot)
		return false;
	
	bool bDraw = false;
	switch(m_nType)
	{
	case kTypeXY:
		bDraw = ((XYChartID*)m_pPlot)->OnLButtonDown( hWnd, point );
		break;
	case kTypeSplit:
		bDraw = ((SplitChartID*)m_pPlot)->OnLButtonDown( hWnd, point );
		break;
	case kTypeShareX:
		bDraw = ((ShareXChartID*)m_pPlot)->OnLButtonDown( hWnd, point );
		break;
	case kTypeLayered:
		bDraw = ((LayeredChartID*)m_pPlot)->OnLButtonDown( hWnd, point );
		break;
	}
	if(bDraw)OnDraw(hWnd);
	return bDraw;
}

bool	CChart::OnLButtonUp( HWND hWnd, POINT point )
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return false;
	if(!m_pPlot)
		return false;
	
	bool bDraw = false;
	switch(m_nType)
	{
	case kTypeXY:
		bDraw = ((XYChartID*)m_pPlot)->OnLButtonUp( hWnd, point );
		break;
	case kTypeSplit:
		bDraw = ((SplitChartID*)m_pPlot)->OnLButtonUp( hWnd, point );
		break;
	case kTypeShareX:
		bDraw = ((ShareXChartID*)m_pPlot)->OnLButtonUp( hWnd, point );
		break;
	case kTypeLayered:
		bDraw = ((LayeredChartID*)m_pPlot)->OnLButtonUp( hWnd, point );
		break;
	}
	if(bDraw)OnDraw(hWnd);
	return bDraw;
}

bool	CChart::OnLButtonDblClk( HWND hWnd, POINT point )
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return false;
	if(!m_pPlot)
		return false;
	
	bool bDraw = false;
	switch(m_nType)
	{
	case kTypeXY:
		bDraw = ((XYChartID*)m_pPlot)->OnLButtonDblClk( hWnd, point );
		break;
	case kTypeSplit:
		bDraw = ((SplitChartID*)m_pPlot)->OnLButtonDblClk( hWnd, point );
		break;
	case kTypeShareX:
		bDraw = ((ShareXChartID*)m_pPlot)->OnLButtonDblClk( hWnd, point );
		break;
	case kTypeLayered:
		bDraw = ((LayeredChartID*)m_pPlot)->OnLButtonDblClk( hWnd, point );
		break;
	}
	if(bDraw)OnDraw(hWnd);
	return bDraw;
}

bool	CChart::OnMouseMove( HWND hWnd, POINT point )
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return false;
	if(!m_pPlot)
		return false;
	
	bool bDraw = false;
	switch(m_nType)
	{
	case kTypeXY:
		bDraw = ((XYChartID*)m_pPlot)->OnMouseMove( hWnd, point );
		break;
	case kTypeSplit:
		bDraw = ((SplitChartID*)m_pPlot)->OnMouseMove( hWnd, point );
		break;
	case kTypeShareX:
		bDraw = ((ShareXChartID*)m_pPlot)->OnMouseMove( hWnd, point );
		break;
	case kTypeLayered:
		bDraw = ((LayeredChartID*)m_pPlot)->OnMouseMove( hWnd, point );
		break;
	}
	if(bDraw)OnDraw(hWnd);
	return bDraw;
}
bool	CChart::OnContextMenu( HMENU hMenu, HWND hWnd, POINT point )
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return false;
	if(!m_pPlot)
		return false;
	
	bool bDraw = false;
	switch(m_nType)
	{
	case kTypeXY:
		bDraw = ((XYChartID*)m_pPlot)->OnContextMenu( hMenu, hWnd, point );
		break;
	case kTypeSplit:
		bDraw = ((SplitChartID*)m_pPlot)->OnContextMenu( hMenu, hWnd, point );
		break;
	case kTypeShareX:
		bDraw = ((ShareXChartID*)m_pPlot)->OnContextMenu( hMenu, hWnd, point );
		break;
	case kTypeLayered:
		bDraw = ((LayeredChartID*)m_pPlot)->OnContextMenu( hMenu, hWnd, point );
		break;
	}
	if(bDraw)OnDraw(hWnd);
	return bDraw;
}

bool	CChart::OnKeyDown( HWND hWnd, UINT key )
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return false;
	if(!m_pPlot)
		return false;
	
	bool bDraw = false;
	switch(m_nType)
	{
	case kTypeXY:
		bDraw = ((XYChartID*)m_pPlot)->OnKeyDown( hWnd, key );
		break;
	case kTypeSplit:
		bDraw = ((SplitChartID*)m_pPlot)->OnKeyDown( hWnd, key );
		break;
	case kTypeShareX:
		bDraw = ((ShareXChartID*)m_pPlot)->OnKeyDown( hWnd, key );
		break;
	case kTypeLayered:
		bDraw = ((LayeredChartID*)m_pPlot)->OnKeyDown( hWnd, key );
		break;
	}
	if(bDraw)OnDraw(hWnd);
	return bDraw;
}

void	CChart::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT	point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	
	switch(m_nType)
	{
	case kTypeXY:
		((XYChartID*)m_pPlot)->Interactive(hWnd, message, wParam, lParam);
		return;
	case kTypeSplit:
		((SplitChartID*)m_pPlot)->Interactive(hWnd, message, wParam, lParam);
		return;
	case kTypeShareX:
		((ShareXChartID*)m_pPlot)->Interactive(hWnd, message, wParam, lParam);
		return;
	case kTypeLayered:
		((LayeredChartID*)m_pPlot)->Interactive(hWnd, message, wParam, lParam);
		return;
	}
	
}
/*
int		CChart::AddCurve(double *pX, double *pY, int nLen)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return -1;
	if(!m_pPlot)
		return -1;

	int dataID;
	switch(m_nType)
	{
	case kTypeXY:
		dataID = ((XYChartID*)m_pPlot)->AddCurve(pX, pY,nLen);
		break;
	default:
		return -1;
	}
	return dataID;
}
*/
int		CChart::AddCurve(double *pX, double *pY, int nLen, int nIndex)
{
	if(nIndex < 0)
		return -1;
	if(!pX || !pY || nLen <=0)
		return -1;
	if(m_nType<0 || m_nType>=kTypeCount|| !m_pPlot)
		return -1;
	
	int subNum;
	int dataID;
	switch(m_nType)
	{
	case kTypeXY:
		dataID = ((XYChartID*)m_pPlot)->AddCurve(pX, pY,nLen);
		break;
	case kTypeSplit:
		subNum = ((SplitChartID*)m_pPlot)->GetPlotsCount();
		if(nIndex>=subNum)
			return -1;
		dataID = ((SplitChartID*)m_pPlot)->AddCurve(pX, pY, nLen, nIndex);
		break;
	case kTypeShareX:
		subNum = ((ShareXChartID*)m_pPlot)->GetLayersCount();
		if(nIndex>=subNum)
			return -1;
		dataID = ((ShareXChartID*)m_pPlot)->AddCurve(pX, pY, nLen, nIndex);
		break;
	case kTypeLayered:
		subNum = ((LayeredChartID*)m_pPlot)->GetLayersCount();
		if(nIndex>=subNum)
			return -1;
		dataID = ((LayeredChartID*)m_pPlot)->AddCurve(pX, pY, nLen, nIndex);
		break;
	default:
		return -1;
	}
	return dataID;
}

int		CChart::AddLayer()
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return -1;
	if(!m_pPlot)
		return -1;

	int	nLayers;
	switch(m_nType)
	{
	case kTypeXY:
		return -1;
	case kTypeSplit:
		return -1;
	case kTypeShareX:
		nLayers = ((ShareXChartID*)m_pPlot)->AddLayer();
		return nLayers;
	case kTypeLayered:
		nLayers = ((LayeredChartID*)m_pPlot)->AddLayer();
		return nLayers;
	default:
		return -1;
	}
}

void	CChart::ResizePlots(int mode, int nRows, int nCols)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;

	switch(m_nType)
	{
	case kTypeSplit:
		((SplitChartID*)m_pPlot)->ResizePlots(mode, nRows, nCols);
		return;
	default:
		return;
	}
}

void	CChart::SetTitle(tstring title)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;

	switch(m_nType)
	{
	case kTypeXY:
		((XYChartID*)m_pPlot)->SetTitle(title);
		return;
	case kTypeShareX:
		((ShareXChartID*)m_pPlot)->SetTitle(title);
		return;
	case kTypeSplit:
		((SplitChartID*)m_pPlot)->SetTitle(title);
		return;
	}
}

void	CChart::SetTitle(tstring title, int nIndex)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;

	switch(m_nType)
	{
	case kTypeSplit:
		((SplitChartID*)m_pPlot)->GetPlot(nIndex)->SetTitle(title);
		return;
	case kTypeLayered:
		((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->SetTitle(title);
		return;
	case kTypeShareX:
		((ShareXChartID*)m_pPlot)->GetLayer(nIndex)->SetTitle(title);
		return;
	}
}

void	CChart::AddAxis(int location, int nIndex)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;
	
	switch(m_nType)
	{
	case kTypeXY:
		((XYChartID*)m_pPlot)->AddAxis(location);
		return;
	case kTypeSplit:
		if(nIndex<0 || nIndex>=((SplitChartID*)m_pPlot)->GetPlotsCount())return;
		((SplitChartID*)m_pPlot)->GetPlot(nIndex)->AddAxis(location);
		return;
	case kTypeShareX:
		if(nIndex<0 || nIndex>=((ShareXChartID*)m_pPlot)->GetLayersCount())return;
		((ShareXChartID*)m_pPlot)->GetLayer(nIndex)->AddAxis(location);
		return;
	case kTypeLayered:
		if(nIndex<0 || nIndex>=((LayeredChartID*)m_pPlot)->GetLayersCount())return;
		((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->AddAxis(location);
		return;
	}
}

void	CChart::SetAxisTitle(tstring title, int location, int nIndex)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;
	
	CAxis *axis;
	switch(m_nType)
	{
	case kTypeXY:
		axis = ((XYChartID*)m_pPlot)->GetAxisByLocation(location);
		if(axis)axis->SetTitle(title);
		return;
	case kTypeSplit:
		axis = ((SplitChartID*)m_pPlot)->GetPlot(nIndex)->GetAxisByLocation(location);
		if(axis)axis->SetTitle(title);
		return;
	case kTypeShareX:
		axis = ((ShareXChartID*)m_pPlot)->GetLayer(nIndex)->GetAxisByLocation(location);
		if(axis)axis->SetTitle(title);
		return;
	case kTypeLayered:
		axis = ((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->GetAxisByLocation(location);
		if(axis)axis->SetTitle(title);
		return;
	}
}

void	CChart::SetUseLegend(bool bUse, int nIndex)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;
	switch(m_nType)
	{
	case kTypeXY:
		((XYChartID*)m_pPlot)->SetUseLegend(bUse);
		return;
	case kTypeShareX:
		((ShareXChartID*)m_pPlot)->SetUseLegend(bUse);
		return;
	case kTypeSplit:
		((SplitChartID*)m_pPlot)->GetPlot(nIndex)->SetUseLegend(bUse);
		return;
	case kTypeLayered:
		((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->SetUseLegend(bUse);
		return;
	}
}

void	CChart::SetGridLine(bool MajorH, bool MajorV, bool MinorH, bool MinorV, int nIndex)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;

	switch(m_nType)
	{
	case kTypeXY:
		((XYChartID*)m_pPlot)->SetUseMajorHGrids(MajorH);
		((XYChartID*)m_pPlot)->SetUseMajorVGrids(MajorV);
		((XYChartID*)m_pPlot)->SetUseMinorHGrids(MinorH);
		((XYChartID*)m_pPlot)->SetUseMinorVGrids(MinorV);
		break;
	case kTypeSplit:
		((SplitChartID*)m_pPlot)->GetPlot(nIndex)->SetUseMajorHGrids(MajorH);
		((SplitChartID*)m_pPlot)->GetPlot(nIndex)->SetUseMajorVGrids(MajorV);
		((SplitChartID*)m_pPlot)->GetPlot(nIndex)->SetUseMinorHGrids(MinorH);
		((SplitChartID*)m_pPlot)->GetPlot(nIndex)->SetUseMinorVGrids(MinorV);
		return;
	case kTypeShareX:
		((ShareXChartID*)m_pPlot)->GetLayer(nIndex)->SetUseMajorHGrids(MajorH);
		((ShareXChartID*)m_pPlot)->GetLayer(nIndex)->SetUseMajorVGrids(MajorV);
		((ShareXChartID*)m_pPlot)->GetLayer(nIndex)->SetUseMinorHGrids(MinorH);
		((ShareXChartID*)m_pPlot)->GetLayer(nIndex)->SetUseMinorVGrids(MinorV);
		return;
	case kTypeLayered:
		((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->SetUseMajorHGrids(MajorH);
		((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->SetUseMajorVGrids(MajorV);
		((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->SetUseMinorHGrids(MinorH);
		((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->SetUseMinorVGrids(MinorV);
		return;
	}
}

void	CChart::SetBkgndColor(COLORREF color, int nIndex)
{
	if(m_nType<0 || m_nType>=kTypeCount)
		return;
	if(!m_pPlot)
		return;
	
	switch(m_nType)
	{
	case kTypeXY:
		((XYChartID*)m_pPlot)->SetBkgndColor(color);
		break;
	case kTypeSplit:
		((SplitChartID*)m_pPlot)->GetPlot(nIndex)->SetBkgndColor(color);
		return;
	case kTypeShareX:
		((ShareXChartID*)m_pPlot)->GetLayer(nIndex)->SetBkgndColor(color);
		return;
	case kTypeLayered:
		((LayeredChartID*)m_pPlot)->GetLayer(nIndex)->SetBkgndColor(color);
		return;
	}
}

CChartWnd *pThisChartWnd;
///////////////////////////////////////////////////////////////////////////////////////
// Class CChartWnd

CChartWnd::CChartWnd()
{
	m_hWnd = 0;
	m_uintOldWndProc = 0;

	pThisChartWnd = this;
}

CChartWnd::~CChartWnd()
{
	Detach();
}

CChart	*CChartWnd::GetChart()
{
	CChart *pChart=(CChart *)::GetWindowLong(m_hWnd, GWL_USERDATA);
	return pChart;
}

bool	CChartWnd::Attach(HWND hWnd, int nType)
{
	if(!IsWindow(hWnd))
		return false;
	if(nType<0 || nType>=kTypeCount)
		return false;
	
	CChart *pChart = 0;
	if(IsWindow(m_hWnd))
	{
		pChart=(CChart *)GetWindowLong(hWnd, GWL_USERDATA);
	}
	
	if(!pChart) pChart = new CChart;
	if(!pChart->SetType(nType))
	{
		delete pChart;
		return false;
	}
	
	m_hWnd = hWnd;
	SetWindowLong(m_hWnd, GWL_USERDATA, (long)pChart);
	m_uintOldWndProc = SetWindowLong(m_hWnd, GWL_WNDPROC, (long)ChartWindowProc);
	
	return true;
}

bool	CChartWnd::Attach(HWND hWnd, int nType, RECT rtClient)
{
	if(Attach(hWnd, nType))
	{
		CChart *pChart=(CChart *)GetWindowLong(hWnd, GWL_USERDATA);
		pChart->SetConfineRect(hWnd, rtClient);
		return true;
	}
	else
	{
		return false;
	}
}

bool	CChartWnd::Detach()
{
	if(!IsWindow(m_hWnd))return true;
	SetWindowLong(m_hWnd, GWL_WNDPROC, m_uintOldWndProc);

	CChart *pChart = (CChart *)GetWindowLong(m_hWnd, GWL_USERDATA);
	if(pChart)delete pChart;
	SetWindowLong(m_hWnd, GWL_USERDATA, 0);

	m_hWnd = 0;
	
	return true;
}

LRESULT	CALLBACK	CChartWnd::ChartWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CChart *pChart = (CChart *)GetWindowLong(hWnd, GWL_USERDATA);
	if(!pChart)
	{
		return 0;
	}
	void *pPlot = pChart->GetPlot();
	
	CChartWnd* pT= pThisChartWnd;
	
	if(pPlot)
	{
		switch(msg)
		{
		case WM_PAINT:
			pChart->OnDraw(hWnd);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		case WM_CONTEXTMENU:
		case WM_KEYDOWN:
			switch(pChart->GetType())
			{
			case kTypeXY:
				((XYChartID *)pPlot)->Interactive(hWnd, msg, wParam, lParam);
				break;
			case kTypeSplit:
				((SplitChartID *)pPlot)->Interactive(hWnd, msg, wParam, lParam);
				break;
			case kTypeShareX:
				((ShareXChartID *)pPlot)->Interactive(hWnd, msg, wParam, lParam);
				break;
			case kTypeLayered:
				((LayeredChartID *)pPlot)->Interactive(hWnd, msg, wParam, lParam);
				break;
			}
			break;
		case WM_ERASEBKGND:
			return 0;
		case WM_DESTROY:
			SetWindowLong(hWnd, GWL_USERDATA, 0);
			if(pChart) delete pChart;
			SetWindowLong(hWnd, GWL_WNDPROC, pT->m_uintOldWndProc);
			CallWindowProc((WNDPROC)pT->m_uintOldWndProc, hWnd, msg, wParam, lParam);
			break;
		default:
			return CallWindowProc((WNDPROC)pT->m_uintOldWndProc, hWnd, msg, wParam, lParam);
		}
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


