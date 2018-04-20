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

//#include "Stdafx.h"
#include "../../Resources/PlotResources.h"
#include "../XYPlotImpl.h"
#include "../XYHandler.h"
#include "DefHandlers.h"
#include "ColorControl.h"
#include <prsht.h>

#include <vector>

typedef struct stPagePara
{
	unsigned int ID;
	DLGPROC DlgProc;
	tstring title;
}PagePara;
typedef vector<PagePara> vPagePara;

/////////////////////////////////////////////////////////////////////////
// Plot Settings

BOOL	CALLBACK DefPlotSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
bool	SetupDataFromPlot(HWND hWnd, CXYPlotImpl *plot, LOGFONT	&lfTitleFont, LOGFONT &lfSubtitleFont, LOGFONT &lfLegendFont);
bool	SaveDataToPlot(HWND hWnd, CXYPlotImpl *plot, LOGFONT	lfTitleFont, LOGFONT lfSubtitleFont, LOGFONT lfLegendFont);
bool	DoPropertySheet(CXYPlotImpl *plot, HWND   hwndOwner);

BOOL	CALLBACK PlotTitleDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL	CALLBACK PlotSubtitleDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL	CALLBACK PlotLegendDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL	CALLBACK PlotRangesDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL	CALLBACK PlotBkgndDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL	CALLBACK PlotMajorGridsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL	CALLBACK PlotMinorGridsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL	CALLBACK PlotAutoSettingDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

void	SetupDataForTitle(HWND hWnd, CXYPlotImpl *plot, LOGFONT	&lfTitleFont);
void	SaveDataForTitle(HWND hWnd, CXYPlotImpl *plot, LOGFONT	lfTitleFont);
void	SetupDataForSubtitle(HWND hWnd, CXYPlotImpl *plot, LOGFONT	&lfSubtitleFont);
void	SaveDataForSubtitle(HWND hWnd, CXYPlotImpl *plot, LOGFONT	lfSubtitleFont);
void	SetupDataForLegend(HWND hWnd, CXYPlotImpl *plot, LOGFONT	&lfLegendFont);
void	SaveDataForLegend(HWND hWnd, CXYPlotImpl *plot, LOGFONT	lfLegendFont);
void	SetupDataForRange(HWND hWnd, CXYPlotImpl *plot);
void	SaveDataForRange(HWND hWnd, CXYPlotImpl *plot);
void	SetupDataForBkgnd(HWND hWnd, CXYPlotImpl *plot);
void	SaveDataForBkgnd(HWND hWnd, CXYPlotImpl *plot);
void	SetupDataForMajorGrid(HWND hWnd, CXYPlotImpl *plot);
void	SaveDataForMajorGrid(HWND hWnd, CXYPlotImpl *plot);
void	SetupDataForMinorGrid(HWND hWnd, CXYPlotImpl *plot);
void	SaveDataForMinorGrid(HWND hWnd, CXYPlotImpl *plot);
void	SetupDataForAutoSettings(HWND hWnd, CXYPlotImpl *plot);
void	SaveDataForAutoSettings(HWND hWnd, CXYPlotImpl *plot);

void	InitPagePara(vPagePara &vPP);

vPagePara g_vPP;
/////////////////////////////////////////////////////////////////////////

bool DefPlotSettings( CXYPlotImpl *plot, HWND hWnd )
{
	if(!CGlobal::m_bNewInface)
	{
		if( DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_XY_SETTINGS), hWnd, (DLGPROC)DefPlotSettingsFunc, (LPARAM)plot) )
			return true;
		else
			return false;
	}
	else
	{
		if( DoPropertySheet(plot, hWnd) )
			return true;
		else
			return false;
	}
	
}

BOOL	CALLBACK DefPlotSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;

	static LOGFONT	lfTitleFont;
	static LOGFONT	lfSubtitleFont;
	static LOGFONT	lfLegendFont;

	// local varibles
	int		oldHeight;

	HDC		hdc;
	LOGFONT	*pLogFont;
	COLORREF color=0;

	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)lparam;
		SetupDataFromPlot(hWnd, plot, lfTitleFont, lfSubtitleFont, lfLegendFont);

		return TRUE;
	case WM_COMMAND:
		// range boxes are changed
		if(HIWORD (wparam) == EN_CHANGE)
		{
			switch(LOWORD(wparam))
			{
			case IDC_PS_XRANGE_HIGH:
			case IDC_PS_XRANGE_LOW:
				//bAutoXRange = false;
				SendDlgItemMessage(hWnd, IDC_PS_AUTO_XRANGE, BM_SETCHECK, FALSE, 0);
				return TRUE;
			case IDC_PS_YRANGE_HIGH:
			case IDC_PS_YRANGE_LOW:
				//bAutoYRange = false;
				SendDlgItemMessage(hWnd, IDC_PS_AUTO_YRANGE, BM_SETCHECK, FALSE, 0);
				return TRUE;
			}
		}

		// font buttons are clicked
		if(HIWORD (wparam) == BN_CLICKED)
		{
			if(LOWORD(wparam)==IDC_PS_CHOOSE_TITLE_FONT || LOWORD(wparam)==IDC_PS_CHOOSE_SUBTITLE_FONT || LOWORD(wparam)==IDC_PS_CHOOSE_LEGEND_FONT)
			{
				hdc = GetDC(hWnd);
				
				if(LOWORD(wparam)==IDC_PS_CHOOSE_TITLE_FONT)
				{
					pLogFont = &lfTitleFont;
					SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
				}
				else if(LOWORD(wparam)==IDC_PS_CHOOSE_SUBTITLE_FONT)
				{
					pLogFont = &lfSubtitleFont;
					SendDlgItemMessage(hWnd, IDC_PS_COLOR_SUBTITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
				}
				else if(LOWORD(wparam)==IDC_PS_CHOOSE_LEGEND_FONT)
				{
					pLogFont = &lfLegendFont;
					SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
				}
				
				oldHeight = pLogFont->lfHeight;
				pLogFont->lfHeight = -MulDiv(oldHeight, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				
				// Font Dialog
				if( ShowFontDialog(hWnd, *pLogFont, color))
				{
					pLogFont->lfHeight = -MulDiv(pLogFont->lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));
					
					if(LOWORD(wparam)==IDC_PS_CHOOSE_TITLE_FONT)
					{
						SendDlgItemMessage(hWnd, IDC_PS_TITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfTitleFont ).c_str());
						//SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_SETCOLOR, 0, (LPARAM)color);
					}
					else if(LOWORD(wparam)==IDC_PS_CHOOSE_SUBTITLE_FONT)
					{
						SendDlgItemMessage(hWnd, IDC_PS_SUBTITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfSubtitleFont ).c_str());
						//SendDlgItemMessage(hWnd, IDC_PS_COLOR_SUBTITLE_FONT, WM_SETCOLOR, 0, (LPARAM)color);
					}
					else if(LOWORD(wparam)==IDC_PS_CHOOSE_LEGEND_FONT)
					{
						SendDlgItemMessage(hWnd, IDC_PS_LEGEND_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfLegendFont ).c_str());
					}
					
				}

				ReleaseDC(hWnd, hdc);
			}
		}

		switch(LOWORD(wparam))
		{
		case IDOK:
			// Apply Changes
			SaveDataToPlot(hWnd, plot, lfTitleFont, lfSubtitleFont, lfLegendFont);

			EndDialog(hWnd, 1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return FALSE;
		}
		break;
	}
	return FALSE;
}

bool	SetupDataFromPlot(HWND hWnd, CXYPlotImpl *plot, LOGFONT	&lfTitleFont, LOGFONT &lfSubtitleFont, LOGFONT &lfLegendFont)
{

	SetupDataForTitle(hWnd, plot, lfTitleFont);
	SetupDataForSubtitle(hWnd, plot, lfSubtitleFont);
	SetupDataForRange(hWnd,plot);
	SetupDataForLegend(hWnd, plot, lfLegendFont);
	SetupDataForBkgnd(hWnd, plot);
	SetupDataForMajorGrid(hWnd, plot);
	SetupDataForMinorGrid(hWnd, plot);
	SetupDataForAutoSettings(hWnd, NULL);
	SetupDataForCopyright(hWnd, plot);
	

	return true;
}

bool	SaveDataToPlot(HWND hWnd, CXYPlotImpl *plot, LOGFONT	lfTitleFont, LOGFONT lfSubtitleFont, LOGFONT lfLegendFont)
{

	SaveDataForTitle(hWnd, plot, lfTitleFont);
	SaveDataForSubtitle(hWnd, plot, lfSubtitleFont);
	SaveDataForRange(hWnd, plot);
	SaveDataForLegend(hWnd, plot, lfLegendFont);
	SaveDataForBkgnd(hWnd, plot);
	SaveDataForMajorGrid(hWnd, plot);
	SaveDataForMinorGrid(hWnd, plot);
	SaveDataForAutoSettings(hWnd, plot);
	SaveDataForCopyright(hWnd,plot);
	

	return true;
}

void	SetupDataForTitle(HWND hWnd, CXYPlotImpl *plot, LOGFONT	&lfTitleFont)
{
	SendDlgItemMessage(hWnd, IDC_PS_PLOT_TITLESHOW, BM_SETCHECK, plot->IsTitleShow(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_PLOT_TITLE, WM_SETTEXT, 0, (LPARAM)plot->GetTitle().c_str()); 
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_SETCOLOR, 0, (LPARAM)plot->GetTitleColor()); 
	lfTitleFont = plot->GetTitleFont();
	SendDlgItemMessage(hWnd, IDC_PS_TITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &plot->GetTitleFont() ).c_str()); 
}

void	SaveDataForTitle(HWND hWnd, CXYPlotImpl *plot, LOGFONT	lfTitleFont)
{
	TCHAR strtmp[1024];
	int check;
	COLORREF color;

	check = SendDlgItemMessage(hWnd, IDC_PS_PLOT_TITLESHOW, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
		plot->SetTitleShow(true);
	else if(check==BST_UNCHECKED)
		plot->SetTitleShow(false);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_PLOT_TITLE), strtmp, 1024);
	plot->SetTitle(strtmp);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetTitleColor(color);
	plot->SetTitleFont(lfTitleFont);
}

void	SetupDataForSubtitle(HWND hWnd, CXYPlotImpl *plot, LOGFONT	&lfSubtitleFont)
{
	SendDlgItemMessage(hWnd, IDC_PS_PLOT_SUBTITLESHOW, BM_SETCHECK, plot->IsSubtitleShow(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_PLOT_SUBTITLE, WM_SETTEXT, 0, (LPARAM)plot->GetSubtitle().c_str()); 
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_SUBTITLE_FONT, WM_SETCOLOR, 0, (LPARAM)plot->GetSubtitleColor());
	lfSubtitleFont = plot->GetSubtitleFont();
	SendDlgItemMessage(hWnd, IDC_PS_SUBTITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &plot->GetSubtitleFont() ).c_str()); 
}
void	SaveDataForSubtitle(HWND hWnd, CXYPlotImpl *plot, LOGFONT	lfSubtitleFont)
{
	TCHAR	strtmp[1024];
	int check;
	COLORREF color;

	check = SendDlgItemMessage(hWnd, IDC_PS_PLOT_SUBTITLESHOW, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
		plot->SetSubtitleShow(true);
	else if(check==BST_UNCHECKED)
		plot->SetSubtitleShow(false);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_PLOT_SUBTITLE), strtmp, 1024);
	plot->SetSubtitle(strtmp);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_SUBTITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetSubtitleColor(color);
	plot->SetSubtitleFont(lfSubtitleFont);
}

void	SetupDataForLegend(HWND hWnd, CXYPlotImpl *plot, LOGFONT	&lfLegendFont)
{
	SendDlgItemMessage(hWnd, IDC_PS_PLOT_LEGEND, BM_SETCHECK, plot->IsUseLegend(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_PLOT_LEGENDTRANSPARENT, BM_SETCHECK, plot->GetLegend()->IsTransparent(), 0);
	lfLegendFont = plot->GetLegend()->GetFont();
	SendDlgItemMessage(hWnd, IDC_PS_LEGEND_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &plot->GetLegend()->GetFont() ).c_str()); 
}
void	SaveDataForLegend(HWND hWnd, CXYPlotImpl *plot, LOGFONT	lfLegendFont)
{
	int check;

	check = SendDlgItemMessage(hWnd, IDC_PS_PLOT_LEGEND, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetUseLegend(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetUseLegend(false);
	}
	check = SendDlgItemMessage(hWnd, IDC_PS_PLOT_LEGENDTRANSPARENT, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->GetLegend()->SetTransparent(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->GetLegend()->SetTransparent(false);
	}
	plot->GetLegend()->SetFont(lfLegendFont);
}

void	SetupDataForRange(HWND hWnd, CXYPlotImpl *plot)
{
	double xRange[2],yRange[2];
	tostringstream ostr;
	tstring str;
	plot->GetPlotRange(xRange, yRange);
	ostr<<xRange[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_XRANGE_HIGH, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	ostr.str(_TEXT(""));
	ostr<<xRange[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_XRANGE_LOW, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	ostr.str(_TEXT(""));
	ostr<<yRange[1];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_YRANGE_HIGH, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	ostr.str(_TEXT(""));
	ostr<<yRange[0];
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_YRANGE_LOW, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	SendDlgItemMessage(hWnd, IDC_PS_AUTO_XRANGE, BM_SETCHECK, plot->IsXAutoRange(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_AUTO_YRANGE, BM_SETCHECK, plot->IsYAutoRange(), 0);
}
void	SaveDataForRange(HWND hWnd, CXYPlotImpl *plot)
{
	double xRange[2], yRange[2];
	TCHAR strtmp[1024];
	int check;
	
	GetWindowText(GetDlgItem(hWnd, IDC_PS_XRANGE_LOW), strtmp, 1024);
	xRange[0]=_tcstod(strtmp, 0);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_XRANGE_HIGH), strtmp, 1024);
	xRange[1]=_tcstod(strtmp, 0);
	check = SendDlgItemMessage(hWnd, IDC_PS_AUTO_XRANGE, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetXAutoRange();
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetXRange(xRange[0], xRange[1]);
	}
	GetWindowText(GetDlgItem(hWnd, IDC_PS_YRANGE_LOW), strtmp, 1024);
	yRange[0]=_tcstod(strtmp, 0);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_YRANGE_HIGH), strtmp, 1024);
	yRange[1]=_tcstod(strtmp, 0);
	check = SendDlgItemMessage(hWnd, IDC_PS_AUTO_YRANGE, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetYAutoRange();
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetYRange(yRange[0], yRange[1]);
	}
}

void	SetupDataForBkgnd(HWND hWnd, CXYPlotImpl *plot)
{
	tostringstream ostr;
	tstring str;

	SendDlgItemMessage(hWnd, IDC_PS_BACKGROUND_GRADIENT, BM_SETCHECK, plot->IsGradientBkgnd(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_BACKGROUND_GRADIENTHORV, BM_SETCHECK, plot->IsGradientHorizontal(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_BACKGROUND, WM_SETCOLOR, 0, (LPARAM)plot->GetBkgndColor()); 
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_DATAAREA, WM_SETCOLOR, 0, (LPARAM)plot->GetDataRegionColor()); 
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_BORDER, WM_SETCOLOR, 0, (LPARAM)plot->GetBorderColor()); 
	SendDlgItemMessage(hWnd, IDC_PS_BKGND_COLOR1, WM_SETCOLOR, 0, (LPARAM)plot->GetGradientColor1()); 
	SendDlgItemMessage(hWnd, IDC_PS_BKGND_COLOR2, WM_SETCOLOR, 0, (LPARAM)plot->GetGradientColor2()); 
	ostr<<plot->GetBorderSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_BORDER_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
}
void	SaveDataForBkgnd(HWND hWnd, CXYPlotImpl *plot)
{
	int check, num;
	COLORREF color;
	TCHAR strtmp[1024];
	check = SendDlgItemMessage(hWnd, IDC_PS_BACKGROUND_GRADIENT, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetGradientBkgnd(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetGradientBkgnd(false);
	}
	check = SendDlgItemMessage(hWnd, IDC_PS_BACKGROUND_GRADIENTHORV, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetGradientHorizontal(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetGradientHorizontal(false);
	}
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_BACKGROUND, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetBkgndColor(color);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_DATAAREA, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetDataRegionColor(color);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_BORDER, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetBorderColor(color);
	SendDlgItemMessage(hWnd, IDC_PS_BKGND_COLOR1, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetGradientColor1(color);
	SendDlgItemMessage(hWnd, IDC_PS_BKGND_COLOR2, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetGradientColor2(color);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_BORDER_SIZE), strtmp, 1024);
	num=(int)_tcstod(strtmp, 0);
	plot->SetBorderSize(num);
}

void	SetupDataForMajorGrid(HWND hWnd, CXYPlotImpl *plot)
{
	tostringstream ostr;
	tstring str;
	SendDlgItemMessage(hWnd, IDC_PS_MAJOR_HORIZONTAL_GRIDS, BM_SETCHECK, plot->IsUseMajorHGrids(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_MAJOR_VERTICAL_GRIDS, BM_SETCHECK, plot->IsUseMajorVGrids(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_MAJOR_GRIDS, WM_SETCOLOR, 0, (LPARAM)plot->GetMajorGridColor());
	ostr<<plot->GetMajorGridLineSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_MAJOR_GRID_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendMessage(GetDlgItem(hWnd,IDC_PS_MAJOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Solid _______________________"));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MAJOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dashed _ _ _ _ _ _ _ _ _ _ _ _ _ _"));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MAJOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dotted . . . . . . . . . . . . . . . . . . . . . ."));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MAJOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot _ . _ . _ . _ . _ . _ . _ . _ ."));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MAJOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot-Dot _ . . _ . . _ . . _ . . _ ."));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MAJOR_LINESTYLE), CB_SETCURSEL, (WPARAM)plot->GetMajorGridLineStyle(), 0);

}
void	SaveDataForMajorGrid(HWND hWnd, CXYPlotImpl *plot)
{
	int check,num;
	TCHAR strtmp[1024];
	COLORREF color;

	check = SendDlgItemMessage(hWnd, IDC_PS_MAJOR_HORIZONTAL_GRIDS, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetUseMajorHGrids(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetUseMajorHGrids(false);
	}
	check = SendDlgItemMessage(hWnd, IDC_PS_MAJOR_VERTICAL_GRIDS, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetUseMajorVGrids(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetUseMajorVGrids(false);
	}
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_MAJOR_GRIDS, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetMajorGridColor(color);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_MAJOR_GRID_SIZE), strtmp, 1024);
	plot->SetMajorGridLineSize(_ttoi(strtmp));
	num = SendDlgItemMessage(hWnd, IDC_PS_MAJOR_LINESTYLE, CB_GETCURSEL, 0, 0);
	plot->SetMajorGridLineStyle(num);
}

void	SetupDataForMinorGrid(HWND hWnd, CXYPlotImpl *plot)
{
	tostringstream ostr;
	tstring str;
	SendDlgItemMessage(hWnd, IDC_PS_MINOR_HORIZONTAL_GRIDS, BM_SETCHECK, plot->IsUseMinorHGrids(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_MINOR_VERTICAL_GRIDS, BM_SETCHECK, plot->IsUseMinorVGrids(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_MINOR_GRIDS, WM_SETCOLOR, 0, (LPARAM)plot->GetMinorGridColor()); 
	ostr<<plot->GetMinorGridLineSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_PS_MINOR_GRID_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	SendMessage(GetDlgItem(hWnd,IDC_PS_MINOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Solid _______________________"));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MINOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dashed _ _ _ _ _ _ _ _ _ _ _ _ _ _"));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MINOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dotted . . . . . . . . . . . . . . . . . . . . . ."));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MINOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot _ . _ . _ . _ . _ . _ . _ . _ ."));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MINOR_LINESTYLE), CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot-Dot _ . . _ . . _ . . _ . . _ ."));
	SendMessage(GetDlgItem(hWnd,IDC_PS_MINOR_LINESTYLE), CB_SETCURSEL, (WPARAM)plot->GetMinorGridLineStyle(), 0);
}
void	SaveDataForMinorGrid(HWND hWnd, CXYPlotImpl *plot)
{
	int check,num;
	TCHAR strtmp[1024];
	COLORREF color;
	check = SendDlgItemMessage(hWnd, IDC_PS_MINOR_HORIZONTAL_GRIDS, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetUseMinorHGrids(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetUseMinorHGrids(false);
	}
	check = SendDlgItemMessage(hWnd, IDC_PS_MINOR_VERTICAL_GRIDS, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetUseMinorVGrids(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetUseMinorVGrids(false);
	}
	
	SendDlgItemMessage(hWnd, IDC_PS_COLOR_MINOR_GRIDS, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetMinorGridColor(color);
	GetWindowText(GetDlgItem(hWnd, IDC_PS_MINOR_GRID_SIZE), strtmp, 1024);
	num=(int)_tcstod(strtmp, 0);
	plot->SetMinorGridLineSize(num);
	num = SendDlgItemMessage(hWnd, IDC_PS_MINOR_LINESTYLE, CB_GETCURSEL, 0, 0);
	plot->SetMinorGridLineStyle(num);
}

void	SetupDataForAutoSettings(HWND hWnd, CXYPlotImpl *plot)
{
	SendDlgItemMessage(hWnd, IDC_PS_DATA_COLOR_AUTO, BM_SETCHECK, plot->IsAutoDataColor(), 0);
	SendDlgItemMessage(hWnd, IDC_PS_DATA_LINESTYLE_AUTO, BM_SETCHECK, plot->IsAutoLineStyle(), 0);
}
void	SaveDataForAutoSettings(HWND hWnd, CXYPlotImpl *plot)
{
	int check;
	check = SendDlgItemMessage(hWnd, IDC_PS_DATA_COLOR_AUTO, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetAutoDataColor(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetAutoDataColor(false);
	}
	check = SendDlgItemMessage(hWnd, IDC_PS_DATA_LINESTYLE_AUTO, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetAutoLineStyle(true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetAutoLineStyle(false);
	}
}

void	InitPagePara(vPagePara &vPP)
{
	vPP.clear();
	PagePara pp;

	pp.ID = IDD_XYSETTINGS_TITLE;
	pp.DlgProc = PlotTitleDlgFunc;
	pp.title = _TEXT("Title");
	vPP.push_back(pp);

	pp.ID = IDD_XYSETTINGS_SUBTITLE;
	pp.DlgProc = PlotSubtitleDlgFunc;
	pp.title = _TEXT("Subtitle");
	vPP.push_back(pp);

	pp.ID = IDD_XYSETTINGS_LEGEND;
	pp.DlgProc = PlotLegendDlgFunc;
	pp.title = _TEXT("Legend");
	vPP.push_back(pp);

	pp.ID = IDD_XYSETTINGS_PLOTRANGES;
	pp.DlgProc = PlotRangesDlgFunc;
	pp.title = _TEXT("Ranges");
	vPP.push_back(pp);

	pp.ID = IDD_XYSETTINGS_BACKGROUND;
	pp.DlgProc = PlotBkgndDlgFunc;
	pp.title = _TEXT("Background");
	vPP.push_back(pp);

	pp.ID = IDD_XYSETTINGS_MAJORGRIDS;
	pp.DlgProc = PlotMajorGridsDlgFunc;
	pp.title = _TEXT("Major Grids");
	vPP.push_back(pp);

	pp.ID = IDD_XYSETTINGS_MINORGRIDS;
	pp.DlgProc = PlotMinorGridsDlgFunc;
	pp.title = _TEXT("Minor Grids");
	vPP.push_back(pp);

	pp.ID = IDD_XYSETTINGS_AUTOSETTINGS;
	pp.DlgProc = PlotAutoSettingDlgFunc;
	pp.title = _TEXT("Auto Settings");
	vPP.push_back(pp);

	pp.ID = IDD_XYSETTINGS_COPYRIGHT;
	pp.DlgProc = PlotCopyrightDlgFunc;
	pp.title = _TEXT("Copyright(C)");
	vPP.push_back(pp);
}

bool	DoPropertySheet(CXYPlotImpl *plot, HWND   hwndOwner)   
{   
	PROPSHEETPAGE   *psp;
	PROPSHEETHEADER   psh;
    
	InitPagePara(g_vPP);
	if((int)g_vPP.size()<=0)return false;

	psp = new PROPSHEETPAGE[g_vPP.size()];
	for(int i=0; i<(int)g_vPP.size(); i++)
	{
		psp[i].dwSize   =   sizeof(PROPSHEETPAGE);
		psp[i].dwFlags   =   PSP_USEICONID   |   PSP_USETITLE;
		psp[i].hInstance   =   CGlobal::MyGetModuleHandle();
		psp[i].pszTemplate   =   MAKEINTRESOURCE(g_vPP[i].ID);
		psp[i].pszIcon   =   NULL;//MAKEINTRESOURCE(IDI_FONT);
		psp[i].pfnDlgProc   =   g_vPP[i].DlgProc;
		psp[i].pszTitle   =   g_vPP[i].title.c_str();
		psp[i].lParam   =   (LPARAM)plot;
		psp[i].pfnCallback   =   NULL;
	}
    
	psh.dwSize   =   sizeof(PROPSHEETHEADER);
	psh.dwFlags   =   PSH_USEICONID   |   PSH_PROPSHEETPAGE;
	psh.hwndParent   =   hwndOwner;
	psh.hInstance   =   CGlobal::MyGetModuleHandle();
	psh.pszIcon   =   NULL;//MAKEINTRESOURCE(IDI_CELL_PROPERTIES);
	psh.pszCaption   =   _TEXT("Plot Settings");
	psh.nPages   =   g_vPP.size();//sizeof(psp)   /   sizeof(PROPSHEETPAGE);
	psh.nStartPage   =   0;
	psh.ppsp   =   (LPCPROPSHEETPAGE)   psp;
	psh.pfnCallback   =   NULL;
    
	bool ret;
	if( PropertySheet(&psh) > 0)
		ret=true;
	else
		ret=false;

	delete []psp;
	return ret;
}

BOOL	CALLBACK PlotTitleDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;

	static LOGFONT	lfTitleFont;

	// local varibles
	int		oldHeight;

	HDC		hdc;
	LOGFONT	*pLogFont;
	COLORREF color=0;

	LPNMHDR pnmh;

	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		// Deal with the title
		SetupDataForTitle(hWnd, plot, lfTitleFont);
		return TRUE;
	case WM_COMMAND:

		// font buttons are clicked
		if(HIWORD (wparam) == BN_CLICKED)
		{
			if(LOWORD(wparam)==IDC_PS_CHOOSE_TITLE_FONT)
			{
				hdc = GetDC(hWnd);
				
				pLogFont = &lfTitleFont;
				SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
				
				oldHeight = pLogFont->lfHeight;
				pLogFont->lfHeight = -MulDiv(oldHeight, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				
				// Font Dialog				
				if( ShowFontDialog(hWnd, *pLogFont, color))
				{
					pLogFont->lfHeight = -MulDiv(pLogFont->lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));
					
					SendDlgItemMessage(hWnd, IDC_PS_TITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfTitleFont ).c_str());
					
				}

				ReleaseDC(hWnd, hdc);
			}
		}
		break;

	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForTitle(hWnd, plot, lfTitleFont);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

BOOL	CALLBACK PlotSubtitleDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;
	
	static LOGFONT	lfSubtitleFont;
	
	// local varibles
	int		oldHeight;
	
	HDC		hdc;
	LOGFONT	*pLogFont;
	COLORREF color=0;

	LPNMHDR pnmh;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		SetupDataForSubtitle(hWnd, plot, lfSubtitleFont);
		return TRUE;
	case WM_COMMAND:
		
		// font buttons are clicked
		if(HIWORD (wparam) == BN_CLICKED)
		{
			if(LOWORD(wparam)==IDC_PS_CHOOSE_SUBTITLE_FONT)
			{
				hdc = GetDC(hWnd);
				
				pLogFont = &lfSubtitleFont;
				SendDlgItemMessage(hWnd, IDC_PS_COLOR_SUBTITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
				
				oldHeight = pLogFont->lfHeight;
				pLogFont->lfHeight = -MulDiv(oldHeight, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				
				// Font Dialog				
				if( ShowFontDialog(hWnd, *pLogFont, color))
				{
					pLogFont->lfHeight = -MulDiv(pLogFont->lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));
					
					SendDlgItemMessage(hWnd, IDC_PS_SUBTITLE_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfSubtitleFont ).c_str());
					
				}
				
				ReleaseDC(hWnd, hdc);
			}
		}
		break;

	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForSubtitle(hWnd, plot, lfSubtitleFont);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL	CALLBACK PlotLegendDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;
	
	static LOGFONT	lfLegendFont;
	
	// local varibles
	int		oldHeight;
	
	HDC		hdc;
	LOGFONT	*pLogFont;
	COLORREF color=0;
	
	LPNMHDR pnmh;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		// Deal with the title
		SetupDataForLegend(hWnd, plot, lfLegendFont);
		
		return TRUE;
	case WM_COMMAND:
		
		// font buttons are clicked
		if(HIWORD (wparam) == BN_CLICKED)
		{
			if(LOWORD(wparam)==IDC_PS_CHOOSE_LEGEND_FONT)
			{
				hdc = GetDC(hWnd);
				
				pLogFont = &lfLegendFont;
				SendDlgItemMessage(hWnd, IDC_PS_COLOR_TITLE_FONT, WM_GETCOLOR, 0, (LPARAM)&color); 
				
				oldHeight = pLogFont->lfHeight;
				pLogFont->lfHeight = -MulDiv(oldHeight, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				
				// Font Dialog				
				if( ShowFontDialog(hWnd, *pLogFont, color))
				{
					pLogFont->lfHeight = -MulDiv(pLogFont->lfHeight, 72, GetDeviceCaps(hdc, LOGPIXELSY));
					
					SendDlgItemMessage(hWnd, IDC_PS_LEGEND_FONT, WM_SETTEXT, 0, (LPARAM)GetFontDescription( &lfLegendFont ).c_str());
					
				}
				
				ReleaseDC(hWnd, hdc);
			}
		}
		break;
		
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForLegend(hWnd, plot, lfLegendFont);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL	CALLBACK PlotRangesDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;
	
	// local varibles
	
	LPNMHDR pnmh;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		
		SetupDataForRange(hWnd, plot); 
		
		return TRUE;
	case WM_COMMAND:
		
		if(HIWORD (wparam) == EN_CHANGE)
		{
			switch(LOWORD(wparam))
			{
			case IDC_PS_XRANGE_HIGH:
			case IDC_PS_XRANGE_LOW:
				SendDlgItemMessage(hWnd, IDC_PS_AUTO_XRANGE, BM_SETCHECK, FALSE, 0);
				return TRUE;
			case IDC_PS_YRANGE_HIGH:
			case IDC_PS_YRANGE_LOW:
				SendDlgItemMessage(hWnd, IDC_PS_AUTO_YRANGE, BM_SETCHECK, FALSE, 0);
				return TRUE;
			}
		}
		break;
		
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForRange(hWnd, plot);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL	CALLBACK PlotBkgndDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;
		
	// local varibles
		
	LPNMHDR pnmh;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		SetupDataForBkgnd(hWnd,plot);
		
		return TRUE;
		
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForBkgnd(hWnd, plot);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL	CALLBACK PlotMajorGridsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;
	
	// local varibles
	
	
	LPNMHDR pnmh;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		SetupDataForMajorGrid(hWnd, plot);		
		return TRUE;
		
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForMajorGrid(hWnd, plot);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL	CALLBACK PlotMinorGridsDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;
	
	// local varibles
	
	
	LPNMHDR pnmh;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		SetupDataForMinorGrid(hWnd, plot);		
		return TRUE;
		
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForMinorGrid(hWnd, plot);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL	CALLBACK PlotAutoSettingDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;
	
	// local varibles
	
	LPNMHDR pnmh;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		SetupDataForAutoSettings(hWnd, plot);
		
		return TRUE;
		
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForAutoSettings(hWnd, plot);
		}
		break;
	}
	return FALSE;
}

