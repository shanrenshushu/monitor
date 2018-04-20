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

#ifndef __CHART_H_122333444455555__
#define __CHART_H_122333444455555__

#include <windows.h>
#include <string>
using namespace std;

#if defined(_UNICODE) || defined(UNICODE)
typedef	wstring tstring;
#else
typedef	string	tstring;
#endif

#ifdef PLOTDLL_EXPORTS
#define PLOTDLL_API __declspec(dllexport)
#else
#define PLOTDLL_API __declspec(dllimport)
#endif

enum
{
	kTypeXY,
	kTypeSplit,
	kTypeShareX,
	kTypeLayered,

	kTypeCount
};
/*
enum
{
	kSplitNot=0,
	kSplitNM=1,
	kSplit3L1R2=2,
	kSplit3L2R1=3,
	kSplit3T1B2=4,
	kSplit3T2B1=5,
		
	kSplitModeCount
};

enum
{
	kLocationLeft = 0,
	kLocationBottom,
	kLocationRight,
	kLocationTop,
	kLocationCenterVL,
	kLocationCenterVR,
	kLocationCenterHB,
	kLocationCenterHT,
	
	kLocationCount
};
*/
///////////////////////////////////////////////////////////////////////////////////////////
// Declaration of CChart

//class	PLOTDLL_API	CChart
class	CChart
{
public:
	CChart();
	virtual	~CChart();
protected:
	void		*m_pPlot;
	int			m_nType;
	
	void		Release();

public:
	void		OnDraw(HWND hWnd);
	void		OnDraw(HDC hDC, RECT destRect);

public:
	bool		OnLButtonDown( HWND hWnd, POINT point );
	bool		OnLButtonUp( HWND hWnd, POINT point );
	bool		OnLButtonDblClk( HWND hWnd, POINT point );
	bool		OnMouseMove( HWND hWnd, POINT point );
	bool		OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	bool		OnKeyDown( HWND hWnd, UINT key );

	void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	void		*GetPlot();
	int			GetType();
	bool		SetType(int nType);
	
	bool		SetConfineRect(HWND hWnd, RECT ConfineRect);

public:	
	int			AddCurve(double *pX, double *pY, int nLen, int nIndex=0);

	int			AddLayer();
	void		ResizePlots(int mode, int nRows, int nCols);

public:
	void		SetTitle(tstring title);
	void		SetTitle(tstring title, int nIndex);
	
	void		AddAxis(int location, int nIndex=0);
	void		SetAxisTitle(tstring title, int location, int nIndex=0);

	void		SetUseLegend(bool bUse, int nIndex=0);

public:
	void		SetGridLine(bool MajorH=true, bool MajorV=true, bool MinorH=false, bool MinorV=false, int nIndex=0);

	void		SetBkgndColor(COLORREF color, int nIndex=0);	
};

///////////////////////////////////////////////////////////////////////////////////////////
// Declaration of CChartWnd

//class	PLOTDLL_API	CChartWnd
class	CChartWnd
{
public:
	CChartWnd();
	virtual	~CChartWnd();

private:
	HWND	m_hWnd;
	UINT	m_uintOldWndProc;
	static	LRESULT	CALLBACK	ChartWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CChart	*GetChart();
	bool	Attach(HWND hWnd, int nType);
	bool	Attach(HWND hWnd, int nType, RECT rtClient);
	bool	Detach();
};

#endif