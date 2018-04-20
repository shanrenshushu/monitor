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
//#include <tchar.h>
#include "PlotControl.h"
#include "../../Basics/XYPlotImpl.h"
#include "../../Basics/XYHandler.h"
#include "../DblBufferPlot/DblBufferPlotImpl.h"
#include "../PlotWnd/PlotWnd.h"

Declare_CDblBufferPlotImplI( XYChartID1, CXYPlotImpl, CXYHandler )
typedef XYChartID1<CXYPlotImpl> XYChartID;

LRESULT CALLBACK PlotControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static	CPlotWnd<XYChartID> plotWnd;
	XYChartID *pPlot;
	PlotData *plotdata;
	
	RECT rt;
	
	switch (message) 
	{
	case WM_CREATE:
		plotWnd.Attach(hWnd);
		pPlot=plotWnd.GetPlot();
		pPlot->SetForCtrl( true );
		
		GetClientRect(hWnd, &rt);
		pPlot->SetWnd(hWnd, rt);
		
		pPlot->SetBkgndColor(GetSysColor(COLOR_3DFACE));
		pPlot->SetUseDataRegionBrush(false);
		pPlot->SetEdgeShow(true);
		pPlot->SetEdgeButton(true);
		break;
	case WM_DESTROY:
		plotWnd.Detach();
		break;
	case WM_SETPLOTDATA:
		pPlot=plotWnd.GetPlot();
		plotdata = (PlotData *)lParam;
		pPlot->AddCurve(plotdata->pX, plotdata->pY, plotdata->nLen);
		break;
	case WM_SETPLOTTITLE:
		pPlot=plotWnd.GetPlot();
		pPlot->SetTitle((LPCTSTR)lParam);
		break;
	case WM_SETAXISTITLE:
		pPlot=plotWnd.GetPlot();
		if(pPlot->GetAxisByLocation(int(wParam)))
		{
			pPlot->GetAxisByLocation(int(wParam))->SetTitle((LPCTSTR)lParam);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM RegisterPlotControl()
{	
	WNDCLASS wcls;
	// check to see if class already registered
	static const TCHAR szClass[] = _TEXT("plotcontrol");
	if (GetClassInfo(CGlobal::MyGetModuleHandle(), szClass, &wcls))
	{
		return 1;// name already registered - ok if it was us
	}
	// Use standard "button" control as a template.
	GetClassInfo(NULL, _TEXT("button"), &wcls);
	// set new values
	wcls.style |=  CS_DBLCLKS; // Make it to receive double clicks
	wcls.lpfnWndProc = (WNDPROC)PlotControlProc;
	wcls.hInstance = CGlobal::MyGetModuleHandle();
	wcls.lpszClassName = szClass;
	
	return RegisterClass(&wcls);
}