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
#include "../XYPlotImpl.h"
#include "../XYHandler.h"
#include "DefHandlers.h"
#include "../../Accessary/MyString/MyString.h"

/////////////////////////////////////////////////////////////////////////
// Context Menu

// Some constants for the popup menus
const int		tickSizes[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const int		nTickSizes				=	9;

const int		nLineSizes				=	10;

// These constants are for the popup menu support. They
// facilitate making and interpretation of the menus

const int		kMenuPlotSettings		=	32768;

// for title
const int		kMenuTitle				=	10;
const int		kMenuSubTitle			=	kMenuTitle+nFontSizes+5;

// for axes
const int		kMenuAxisRange			=	100;
const int		kMenuAxisMinimum		=	(1*kMenuAxisRange);
const int		kMenuDataRange			=	100;
const int		kMenuDataMinimum		=	(1*kMenuDataRange+1000);

const int		kMenuAxisTitleSize		=	1;
const int		kMenuAxisLabelSize		=	(kMenuAxisTitleSize+nFontSizes);
const int		kMenuAxisMajorTickCount	=	(kMenuAxisLabelSize+nFontSizes);
// The +1 below is because there is one additional tick size (which is the "Auto" item)
const int		kMenuAxisMinorTickCount	=	(kMenuAxisMajorTickCount+nTickSizes+1);

// for background
const int		kHutchTypeCount			=	6;
const int		nGridsSizes				=	6;
const int		nGradientSizes			=	4;
const int		nBkgndSizes				=	1+kBrushTypeCount+kHutchTypeCount;
const int		nDataRegionSizes		=	2+kBrushTypeCount+kHutchTypeCount;
const int		nBorderSizes			=	1+nLineSizes;
const int		kMenuBackground			=	(kMenuAxisRange*kMaxAxes)+kMenuAxisMinimum;
const int		kMenuGrids				=	kMenuBackground;
const int		kMenuGradient			=	kMenuGrids + nGridsSizes;
const int		kMenuBkgnd				=	kMenuGradient + nGradientSizes;
const int		kMenuBkgndBrushType		=	kMenuBkgnd + 1;
const int		kMenuBkgndHutchType		=	kMenuBkgndBrushType+kBrushTypeCount;
const int		kMenuDataRegion			=	kMenuBkgnd + nBkgndSizes;
const int		kMenuDataRegionBrushType	=	kMenuDataRegion + 2;
const int		kMenuDataRegionHutchType	=	kMenuDataRegionBrushType+kBrushTypeCount;
const int		kMenuBorder				=	kMenuDataRegion + nDataRegionSizes;

const int		kMenuBackgroundRange	=	100;

// for legend
const int		kMenuLegend				=	kMenuBackground+kMenuBackgroundRange;
const int		kMenuLegendRange		=	30;

// for data set
const int		kMenuChartType			=	0;
const int		kMenuLineSize			=	(1+kPlotTypeCount);
const int		kMenuLineStyle			=	(1+kMenuLineSize+nLineSizes);
const int		kMenuMarker				=	(1+kMenuLineStyle+5);
const int		kMenuMarkerFill			=	(kMarkerTypeCount);
const int		kMenuMarkerSize			=	(kMenuMarkerFill+1);
const int		nMarkerSizes			=	20;
const int		kMenuMarkerFrequency	=	(kMenuMarkerSize+nMarkerSizes);
const int		nMaxDataSetsMenus		=	64;

/////////////////////////////////////////////////////////////////////////

HMENU	CreateContextMenu(CXYPlotImpl *plot);

void	AddFontSizePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddTitlePopup(CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int startVal);
void	AddSubtitlePopup(CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int startVal);
void	AddLineSizePopup(HMENU addMenu, tstring titleString, long compare, int startVal);
void	AddGridPopup(CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int startVal);
void	AddPlotTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddDataLineSizePopup(CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int dataID, int startVal );
void	AddDataLineStylePopup( CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int dataID, int startVal );
void	AddMarkerPopup( CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int dataID, int startVal );
void	AddDataPopup( CXYPlotImpl *plot, int whichDataSet, HMENU menu, int startVal );
void	AddGradientPopup( CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int startVal);
void	AddAxisPopup( CXYPlotImpl *plot, HMENU menu, int which, int startVal );
void	AddNumberOfTicksPopup( HMENU addMenu, tstring titleString, long compare, int startVal, BOOL bAuto );
void	AddBrushTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal );
void	AddHutchTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal );

int		InterpretPopupItem( CXYPlotImpl *plot, HWND hWnd, int result );
int		InterpretDataSetPopupItem( CXYPlotImpl *plot,HWND hWnd, int whichDataSet, int result );
int		InterpretAxisPopupItem( CXYPlotImpl *plot, int which, int whichResult );

/////////////////////////////////////////////////////////////////////////

int DefContextMenu( CXYPlotImpl *plot, HMENU hMenu, HWND hWnd, POINT point )
{
	HMENU			menu;
	int				result;

	if(!IsWindow(hWnd))return kPopupError;
	if( plot->GetDataSetCount()<=0 )return kPopupError;

	if( IsMenu(hMenu) )
	{
		result = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, 0, hWnd, NULL);
		SendMessage( hWnd, WM_COMMAND, (WPARAM)result, 0);
		return kPopupNoAction;
	}

	menu = CreateContextMenu(plot);
	result = TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, 0, hWnd, NULL);
	DestroyMenu(menu);

	return InterpretPopupItem( plot, hWnd, result );
}

HMENU	CreateContextMenu(CXYPlotImpl *plot)
{
	HMENU			menu, subMenu;
	int				i;
	UINT			flag;

	menu = CreatePopupMenu();

	// Check for a title, and put in title menu if needed
	if(CGlobal::m_bMenuTitles)
	{
		if( ( plot->GetTitle() != _TEXT("") && plot->IsTitleShow() )
			|| ( plot->GetSubtitle() != _TEXT("") && plot->IsSubtitleShow()) )
		{
			subMenu = CreatePopupMenu();
			AddTitlePopup(plot, subMenu, _TEXT("&Main Title"), kMenuTitle);
			AddSubtitlePopup(plot, subMenu, _TEXT("&Sub Title"), kMenuSubTitle);
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("&Titles") );
		}
	}

	if( CGlobal::m_bMenuAxes && plot->GetAxisCount() > 0 )
	{
		subMenu = CreatePopupMenu();
		for( i = 0; i < plot->GetAxisCount(); i++ )
		{
			AddAxisPopup(plot, subMenu, i, (kMenuAxisRange*i)+kMenuAxisMinimum );
		}
		
		if( plot->GetAxisCount() > 1)
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("&Axes") );
		else
			AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("&Axis") );
	}
	

	// Back ground
	if( CGlobal::m_bMenuBackground )
	{
		subMenu = CreatePopupMenu();

		AddGridPopup(plot, subMenu, _TEXT("&Grids"), kMenuGrids);
		AddGradientPopup(plot, subMenu, _TEXT("Gra&dient"), kMenuGradient);
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		AppendMenu(subMenu, MF_STRING, kMenuBkgnd, _TEXT("&Background Color...") );
		AddBrushTypePopup(subMenu, _TEXT("Background B&rush"), plot->GetBkgndBrushType(), kMenuBkgndBrushType);
		AddHutchTypePopup(subMenu, _TEXT("Background &Hutch"), plot->GetBkgndHatchStyle(), kMenuBkgndHutchType);
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		if( plot->IsUseDataRegionBrush() )
			AppendMenu(subMenu, MF_STRING, kMenuDataRegion, _TEXT("D&isable Data Area Brush") );
		else
			AppendMenu(subMenu, MF_STRING, kMenuDataRegion, _TEXT("&Enable Data Area Brush") );
		AppendMenu(subMenu, MF_STRING, kMenuDataRegion+1, _TEXT("&Data Area Color...") );
		AddBrushTypePopup(subMenu, _TEXT("Data Area B&rush"), plot->GetDataRegionBrushType(), kMenuDataRegionBrushType);
		AddHutchTypePopup(subMenu, _TEXT("Data Area &Hutch"), plot->GetDataRegionHatchStyle(), kMenuDataRegionHutchType);
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		AppendMenu(subMenu, MF_STRING, kMenuBorder, _TEXT("Border &Color...") );
		AddLineSizePopup(subMenu, _TEXT("Border &Sizes"), plot->GetBorderSize(), kMenuBorder+1);

		AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("B&ackground") );
	}

	// Legend
	if( CGlobal::m_bMenuLegend )
	{
		subMenu = CreatePopupMenu();
		if( plot->IsUseLegend() )
			AppendMenu(subMenu, MF_STRING, kMenuLegend, _TEXT("&Hide Legend") );
		else
			AppendMenu(subMenu, MF_STRING, kMenuLegend, _TEXT("&Show Legend") );
		flag = GetPopupCheckedFlag( plot->GetLegend()->IsTransparent() );
		AppendMenu(subMenu, MF_STRING | flag, kMenuLegend+1, _TEXT("&Transparent") );
		flag = GetPopupCheckedFlag( plot->GetLegend()->IsConfined() );
		AppendMenu(subMenu, MF_STRING | flag, kMenuLegend+2, _TEXT("&Confined") );
		AppendMenu(subMenu, MF_STRING, kMenuLegend+3, _TEXT("Legend &Font...") );
		AppendMenu(subMenu, MF_STRING, kMenuLegend+4, _TEXT("&Bkgnd Color...") );
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		AppendMenu(subMenu, MF_STRING, kMenuLegend+5, _TEXT("Border &Color...") );
		AddLineSizePopup(subMenu, _TEXT("Border &Size"), plot->GetLegend()->GetBorderLineSize(),kMenuLegend+6);

		AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("L&egend") );
	}
	
	// Data sets
	if( CGlobal::m_bMenuDatasets && plot->GetXYDataInfoSize()>0)
	{
		subMenu = CreatePopupMenu();
		
		for( i = 0; i < min( plot->GetDataSetCount(),nMaxDataSetsMenus ); i++ )
		{
			AddDataPopup(plot, i, subMenu, (kMenuDataRange*i) + kMenuDataMinimum );
		}
	
		tostringstream ostr;
		tstring str;
		ostr<<_TEXT("&Data Sets(Total ")<<plot->GetDataSetCount()<<_TEXT(")");
		str=ostr.str();
		AppendMenu(menu, MF_POPUP | MF_STRING, (UINT) subMenu, str.c_str() );
		
		AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
		
		AppendMenu(subMenu, MF_STRING, kMenuPlotSettings-2, _TEXT("&Load data..."));
		AppendMenu(subMenu, MF_STRING, kMenuPlotSettings-1, _TEXT("&Save data..."));
	}

	// Settings...
	AppendMenu(menu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	AppendMenu(menu, MF_STRING, kMenuPlotSettings, _TEXT("&Plot Settings...") );

	return menu;
}

// Take the result of the popup menu selection, figure out
// what the user wanted, and take the appropriate action
int		InterpretPopupItem( CXYPlotImpl *plot, HWND hWnd, int result )
{
	int			whichAxis, returnVal;
	tstring		filename;

	COLORREF	color=0;
	LOGFONT		logfont;

	//if( result >= kMinimumPopupValue ) return result;

	// Grids
	if( result <= 0 ) return kPopupError;

	// Title
	if(result >= kMenuTitle && result < kMenuSubTitle)
	{
		if( result == kMenuTitle )
		{
			plot->SetTitleShow( !plot->IsTitleShow() );
			return kPopupUpdate;
		}
		if( result >= kMenuTitle + 1 && result < kMenuTitle + 1 + nFontSizes )
		{
			plot->GetTitleFont().lfHeight = fontSizes[result-kMenuTitle-1];
			return kPopupUpdate;
		}
		else if( result == kMenuTitle + 1 + nFontSizes )
		{
			color = plot->GetTitleColor();
			if(ShowColorDialog(hWnd, color))
			{
				plot->SetTitleColor(color);
				return kPopupUpdate;
			}
		}
		else if( result == kMenuTitle + 1 + nFontSizes +1 )
		{
			logfont = plot->GetTitleFont();
			if(ShowFontDialog(hWnd, logfont, color))
			{
				plot->SetTitleFont(logfont);
				return kPopupUpdate;
			}
		}
	}
	if(result >= kMenuSubTitle && result < kMenuSubTitle + nFontSizes + 3)
	{
		if( result == kMenuSubTitle )
		{
			plot->SetSubtitleShow( !plot->IsSubtitleShow() );
			return kPopupUpdate;
		}
		if( result >= kMenuSubTitle + 1 && result < kMenuSubTitle + 1 + nFontSizes )
		{
			plot->GetSubtitleFont().lfHeight = fontSizes[result - kMenuSubTitle - 1];
			return kPopupUpdate;
		}
		else if( result == kMenuSubTitle + nFontSizes + 1 )
		{
			color = plot->GetSubtitleColor();
			if(ShowColorDialog(hWnd, color))
			{
				plot->SetSubtitleColor(color);
				return kPopupUpdate;
			}
		}
		else if( result == kMenuSubTitle + nFontSizes + 2 )
		{
			logfont = plot->GetSubtitleFont();
			if(ShowFontDialog(hWnd, logfont, color))
			{
				plot->SetSubtitleFont(logfont);
				return kPopupUpdate;
			}
		}
	}
	

	// Axes
	if( result >= kMenuAxisMinimum && result < kMenuAxisMinimum + (kMaxAxes*kMenuAxisRange) )
	{
		// These must be axis sets
		whichAxis = (result-kMenuAxisMinimum) / kMenuAxisRange;

		if( whichAxis < 0 || whichAxis > plot->GetAxisCount() ) return kPopupError;

		if( result % kMenuAxisRange == kMenuAxisRange-1 ) // have to do this one manually
			if( DefAxisSettings(plot, hWnd, plot->GetAxis(whichAxis)->GetLocation()) ) return kPopupUpdate; else return kPopupNoAction;

		//if( (returnVal = plot->GetAxis(whichAxis)->InterpretAxisPopupItem( (result-kMenuAxisMinimum) % kMenuAxisRange ) ) == kPopupError )
		if( (returnVal = InterpretAxisPopupItem(plot, whichAxis, (result-kMenuAxisMinimum) % kMenuAxisRange ) ) == kPopupError )
			return result;
		else
			return returnVal;
	}

	// Background
	if( result >= kMenuBackground && result < kMenuBackground + kMenuBackgroundRange )//gradient back ground
	{
		if( result < kMenuGrids + nGridsSizes )
		{
			bool bSet;
			switch( result - kMenuGrids )
			{
			case 0: 
				bSet=plot->IsUseMajorHGrids()&&plot->IsUseMajorVGrids();
				plot->SetUseMajorHGrids(!bSet);plot->SetUseMajorVGrids(!bSet);break;
			case 1: plot->SetUseMajorHGrids( !plot->IsUseMajorHGrids() ); break;
			case 2: plot->SetUseMajorVGrids( !plot->IsUseMajorVGrids() ); break;
			case 3:
				bSet=plot->IsUseMinorHGrids()&&plot->IsUseMinorVGrids();
				plot->SetUseMinorHGrids(!bSet);plot->SetUseMinorVGrids(!bSet);break;;
			case 4: plot->SetUseMinorHGrids( !plot->IsUseMinorHGrids() ); break;
			case 5: plot->SetUseMinorVGrids( !plot->IsUseMinorVGrids() ); break;
			default: break;
			}
			return kPopupUpdate;
		}
		if( result >= kMenuGradient && result < kMenuGradient + nGradientSizes)
		{
			switch(result - kMenuGradient)
			{
			case 0: plot->SetGradientBkgnd(!plot->IsGradientBkgnd());return kPopupUpdate;
			case 1: plot->SetGradientHorizontal( !plot->IsGradientHorizontal() );return kPopupUpdate;
			case 2:
				color = plot->GetGradientColor1();
				if(ShowColorDialog(hWnd, color))
				{
					plot->SetGradientColor1(color);
					return kPopupUpdate;
				}
				return kPopupNoAction;
			case 3:
				color = plot->GetGradientColor2();
				if(ShowColorDialog(hWnd, color))
				{
					plot->SetGradientColor2(color);
					return kPopupUpdate;
				}
				return kPopupNoAction;
			}
		}
		if( result >= kMenuBkgnd && result < kMenuBkgnd + nBkgndSizes )
		{
			if( result - kMenuBkgnd ==0 )
			{
				color = plot->GetBkgndColor();
				if(ShowColorDialog(hWnd, color))
				{
					plot->SetBkgndColor(color);
					return kPopupUpdate;
				}
				return kPopupNoAction;
			}
			if( result - kMenuBkgndBrushType >= 0 && result - kMenuBkgndBrushType < kBrushTypeCount )
			{
				plot->SetBkgndBrushType(result - kMenuBkgndBrushType );
				return kPopupUpdate;
			}
			if( result - kMenuBkgndHutchType >= 0 && result - kMenuBkgndHutchType < kHutchTypeCount )
			{
				plot->SetBkgndHatchStyle(result - kMenuBkgndHutchType );
				return kPopupUpdate;
			}
			
		}
		if( result >= kMenuDataRegion && result < kMenuDataRegion + nDataRegionSizes )
		{
			if( result - kMenuDataRegion == 0 )
			{
				plot->SetUseDataRegionBrush( !plot->IsUseDataRegionBrush() );
				return kPopupUpdate;
			}
			if( result - kMenuDataRegion == 1 )
			{
				color = plot->GetDataRegionColor();
				if(ShowColorDialog(hWnd, color))
				{
					plot->SetDataRegionColor(color);
					return kPopupUpdate;
				}
				return kPopupNoAction;
			}
			if( result - kMenuDataRegionBrushType >= 0 && result - kMenuDataRegionBrushType < kBrushTypeCount )
			{
				plot->SetDataRegionBrushType(result - kMenuDataRegionBrushType );
				return kPopupUpdate;
			}
			if( result - kMenuDataRegionHutchType >= 0 && result - kMenuDataRegionHutchType < kHutchTypeCount )
			{
				plot->SetDataRegionHatchStyle(result - kMenuDataRegionHutchType );
				return kPopupUpdate;
			}
		}
		if( result >= kMenuBorder && result < kMenuBorder + nBorderSizes )
		{
			if( result - kMenuBorder == 0 )
			{
				color = plot->GetBorderColor();
				if(ShowColorDialog(hWnd, color))
				{
					plot->SetBorderColor(color);
					return kPopupUpdate;
				}
				return kPopupNoAction;
			}
			if( result - kMenuBorder > 0 && result - kMenuBorder <= nLineSizes )
			{
				plot->SetBorderSize( result - kMenuBorder );
				return kPopupUpdate;
			}
		}
	}

	// Legend
	if( result >= kMenuLegend && result < kMenuLegend + kMenuLegendRange )//use legend
	{
		if( result == kMenuLegend )
		{
			plot->SetUseLegend(!plot->IsUseLegend());
			return kPopupUpdate;
		}
		if( result == kMenuLegend + 1 )
		{
			plot->GetLegend()->SetTransparent( !plot->GetLegend()->IsTransparent() );
			return kPopupUpdate;
		}
		if( result == kMenuLegend + 2 )
		{
			plot->GetLegend()->SetConfined( !plot->GetLegend()->IsConfined() );
			return kPopupUpdate;
		}
		if( result == kMenuLegend + 3 )
		{
			logfont = plot->GetLegend()->GetFont();
			if(ShowFontDialog(hWnd, logfont, color))
			{
				plot->GetLegend()->SetFont(logfont);
				return kPopupUpdate;
			}
			return kPopupNoAction;
		}
		if( result == kMenuLegend + 4 )
		{
			color = plot->GetLegend()->GetBkgndColor();
			if(ShowColorDialog(hWnd, color))
			{
				plot->GetLegend()->SetBkgndColor(color);
				return kPopupUpdate;
			}
			return kPopupNoAction;
		}
		if( result == kMenuLegend + 5 )
		{
			color = plot->GetLegend()->GetBorderColor();
			if(ShowColorDialog(hWnd, color))
			{
				plot->GetLegend()->SetBorderColor(color);
				return kPopupUpdate;
			}
			return kPopupNoAction;
		}
		if( result >= kMenuLegend + 6 && result < kMenuLegend + 6 + nLineSizes )
		{
			plot->GetLegend()->SetBorderLizeSize( result - kMenuLegend - 5 );
			return kPopupUpdate;
		}
	}

	// Dataset
	if( result >= kMenuDataMinimum && result < kMenuDataMinimum + min( plot->GetDataSetCount(), nMaxDataSetsMenus )*kMenuDataRange )
	{
		int			whichDataSet;

		whichDataSet = (result-kMenuDataMinimum) / kMenuDataRange;

		if( whichDataSet < 0 || whichDataSet > plot->GetDataSetCount()-1 ) return kPopupError;

		if( (result-kMenuDataMinimum) % kMenuDataRange == kMenuDataRange-1 ) // have to do this one manually
		{
			if( DefDataSettings(plot, hWnd, whichDataSet) ) return kPopupUpdate; else return kPopupNoAction;
		}

		return InterpretDataSetPopupItem( plot, hWnd, whichDataSet, (result-kMenuDataMinimum) % kMenuDataRange );
	}

	if( result == kMenuPlotSettings - 2 )
	{
		if( !ShowFileDialog(hWnd, true, filename) )	return kPopupNoAction;
		if( plot->ReadFromFile(filename) )
		{
			MessageBox(hWnd, _TEXT("Load data OK"), _TEXT("Infomation"), MB_OK);
			return kPopupUpdate;
		}
		else
		{
			return kPopupNoAction;
		}
	}

	if( result == kMenuPlotSettings - 1 )
	{
		if( !ShowFileDialog(hWnd, false, filename) ) return kPopupNoAction;
		if( plot->WriteToFile(filename) )
		{
			MessageBox(hWnd, _TEXT("Save data OK"), _TEXT("Infomation"), MB_OK);
		}
		return kPopupNoAction;
	}

	// The last menu, ^_^
	if( result == kMenuPlotSettings )
	{
		if( DefPlotSettings(plot, hWnd) ) return kPopupUpdate; else return kPopupNoAction;
	}

	return kPopupError;
}

// Adds a popup menu for font sizes.
void	AddFontSizePopup( HMENU addMenu, tstring titleString, long compare, int startVal )
{
	HMENU			subMenu = CreatePopupMenu();
	UINT			flag;
	
	for( int i = 0; i < nFontSizes; i++ )
	{
		flag = GetPopupCheckedFlag( compare == fontSizes[i] );
		tostringstream ostr;
		tstring str;
		ostr<<fontSizes[i];
		str=ostr.str();
		AppendMenu(subMenu, MF_STRING | flag, startVal+i, str.c_str() );
	}
	
	AppendMenu(addMenu, MF_POPUP, (UINT)subMenu, titleString.c_str() );
}

void	AddLineSizePopup(HMENU addMenu, tstring titleString, long compare, int startVal)
{
	HMENU			subMenu = CreatePopupMenu();
	UINT			flag;

	for(int i=0; i<nLineSizes; i++)
	{
		flag = GetPopupCheckedFlag( compare == i+1 );
		tostringstream ostr;
		tstring str;
		ostr<<i+1;
		str=ostr.str();
		AppendMenu( subMenu, MF_STRING | flag, startVal+i, str.c_str() );
	}
	AppendMenu(addMenu, MF_POPUP | MF_STRING, (UINT) subMenu, titleString.c_str() );
}

//
void	AddTitlePopup(CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int startVal)
{
	HMENU			subMenu;

	subMenu = CreatePopupMenu();
	if( plot->IsTitleRealyShow() )
		AppendMenu(subMenu, MF_STRING, startVal, _TEXT("&Hide It") );
	else
		AppendMenu(subMenu, MF_STRING, startVal, _TEXT("&Show It") );
	AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	AddFontSizePopup( subMenu, _TEXT("S&ize"), plot->GetTitleFont().lfHeight, startVal+1 );
	AppendMenu(subMenu, MF_STRING, startVal+1+nFontSizes, _TEXT("&Color...") );
	AppendMenu(subMenu, MF_STRING, startVal+1+nFontSizes+1, _TEXT("&Font...") );

	AppendMenu(addMenu, MF_POPUP | MF_STRING, (UINT) subMenu, titleString.c_str() );
}

//
void	AddSubtitlePopup(CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int startVal)
{
	HMENU			subMenu;
	
	subMenu = CreatePopupMenu();
	if( plot->IsSubtitleShow() )
		AppendMenu(subMenu, MF_STRING, startVal, _TEXT("&Hide It") );
	else
		AppendMenu(subMenu, MF_STRING, startVal, _TEXT("&Show It") );
	AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	AddFontSizePopup( subMenu, _TEXT("S&ize"), plot->GetTitleFont().lfHeight, startVal+1 );
	AppendMenu(subMenu, MF_STRING, startVal+1+nFontSizes, _TEXT("&Color...") );
	AppendMenu(subMenu, MF_STRING, startVal+1+nFontSizes+1, _TEXT("&Font...") );
	
	AppendMenu(addMenu, MF_POPUP | MF_STRING, (UINT) subMenu, titleString.c_str() );
}

//
void	AddGridPopup(CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int startVal)
{
	HMENU			subMenu;
	UINT			flag;

	subMenu = CreatePopupMenu();
	flag = GetPopupCheckedFlag( plot->IsUseMajorHGrids() && plot->IsUseMajorVGrids());
	AppendMenu(subMenu, MF_STRING | flag, startVal, _TEXT("M&ajor Grids") );
	flag = GetPopupCheckedFlag( plot->IsUseMajorHGrids() );
	AppendMenu(subMenu, MF_STRING | flag, startVal+1, _TEXT("---&Horizontal") );
	flag = GetPopupCheckedFlag(plot->IsUseMajorVGrids() );
	AppendMenu(subMenu, MF_STRING | flag, startVal+2, _TEXT("---&Vertical") );
	flag = GetPopupCheckedFlag( plot->IsUseMinorHGrids() && plot->IsUseMinorVGrids());

	AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );

	AppendMenu(subMenu, MF_STRING | flag, startVal+3, _TEXT("M&inor Grids") );
	flag = GetPopupCheckedFlag(plot->IsUseMinorHGrids() );
	AppendMenu(subMenu, MF_STRING | flag, startVal+4, _TEXT("---H&orizontal") );
	flag = GetPopupCheckedFlag(plot->IsUseMinorVGrids() );
	AppendMenu(subMenu, MF_STRING | flag, startVal+5, _TEXT("---V&ertical") );
	AppendMenu(addMenu, MF_POPUP | MF_STRING, (UINT) subMenu, _TEXT("&Grids") );
}

// Add a popup menu for the different chart types
void	AddPlotTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal )
{
	HMENU			subMenu;
	UINT			flag;
	
	subMenu = CreatePopupMenu();

	flag = GetPopupCheckedFlag( compare == kXYPlotScatter  );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kXYPlotScatter , _TEXT("XY &Scatter") );
	flag = GetPopupCheckedFlag( compare == kXYPlotConnect );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kXYPlotConnect, _TEXT("XY &Line") );
	flag = GetPopupCheckedFlag( compare == kXYPlotStepHV );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kXYPlotStepHV, _TEXT("Step &H-V") );
	flag = GetPopupCheckedFlag( compare == kXYPlotStepVH );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kXYPlotStepVH, _TEXT("Step &V-H") );
	flag = GetPopupCheckedFlag( compare == kXYPlotStepHVH );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kXYPlotStepHVH, _TEXT("S&tep H-V-H") );
	flag = GetPopupCheckedFlag( compare == kXYPlotStepVHV );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kXYPlotStepVHV, _TEXT("St&ep &V-H-V") );
	flag = GetPopupCheckedFlag( compare == kXYPlotBezier );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kXYPlotBezier, _TEXT("&Bezier Line") );
	
	AppendMenu(addMenu, MF_POPUP, (UINT) subMenu, titleString.c_str());
}

// Add a popup menu for brush type
void	AddBrushTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal )
{
	HMENU			subMenu;
	UINT			flag;
	
	subMenu = CreatePopupMenu();
	
	flag = GetPopupCheckedFlag( compare == kBrushNull  );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kBrushNull , _TEXT("&Null Brush") );
	flag = GetPopupCheckedFlag( compare == kBrushSolid );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kBrushSolid, _TEXT("&Solid Brush") );
	flag = GetPopupCheckedFlag( compare == kBrushHatch );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kBrushHatch, _TEXT("&Hatch Brush") );
	flag = GetPopupCheckedFlag( compare == kBrushPattern );
	AppendMenu(subMenu, MF_STRING | flag, startVal+kBrushPattern, _TEXT("&Pattern Brush") );
	
	AppendMenu(addMenu, MF_POPUP, (UINT) subMenu, titleString.c_str());
}

// Add a popup menu for hutch type
void	AddHutchTypePopup( HMENU addMenu, tstring titleString, long compare, int startVal )
{
	HMENU			subMenu;
	UINT			flag;
	
	subMenu = CreatePopupMenu();

	flag = GetPopupCheckedFlag( compare == 0  );
	AppendMenu(subMenu, MF_STRING | flag, startVal+0 , _TEXT("&Horizontal Hutch") );/* ----- */
	flag = GetPopupCheckedFlag( compare == 1 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+1, _TEXT("&Vertical Hutch") );/* ||||| */
	flag = GetPopupCheckedFlag( compare == 2 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+2, _TEXT("&FDiagonal Hutch") );/* \\\\\ */
	flag = GetPopupCheckedFlag( compare == 3 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+3, _TEXT("&BDiagonal Hutch") );/* ///// */
	flag = GetPopupCheckedFlag( compare == 4 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+4, _TEXT("&Cross Hutch") );/* +++++ */
	flag = GetPopupCheckedFlag( compare == 5 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+5, _TEXT("&Diagcross Hutch") );/* xxxxx */
	
	AppendMenu(addMenu, MF_POPUP, (UINT) subMenu, titleString.c_str());
}

// Add a popup for the line size of a data set
void	AddDataLineSizePopup(CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int dataID, int startVal )
{
	HMENU		subMenu = CreatePopupMenu();
	UINT		flag;
		
	// Line size
	for( int i = 0; i < nLineSizes; i++ )
	{
		flag = GetPopupCheckedFlag( plot->GetDataLineSize( dataID ) == i+1 );
		tostringstream ostr;
		tstring str;
		ostr<<i+1;
		str=ostr.str();
		AppendMenu(subMenu, MF_STRING | flag, startVal+i, str.c_str() );
	}
	AppendMenu(addMenu, MF_POPUP, (UINT) subMenu, titleString.c_str() );
}

// Add a popup for the line size of a data set
void	AddDataLineStylePopup( CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int dataID, int startVal )
{
	HMENU		subMenu;
	UINT		flag;
	
	subMenu = CreatePopupMenu();
	
	// We'll just hard code these for now
	flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 0 );
	AppendMenu(subMenu, MF_STRING | flag, startVal,   _TEXT("Solid ____________") );
	flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 1 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+1, _TEXT("Dashed _ _ _ _ _ _") );
	flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 2 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+2, _TEXT("Dotted . . . . . .") );
	flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 3 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+3, _TEXT("Dash-Dot _ . _ . _") );
	flag = GetPopupCheckedFlag( plot->GetDataLineStyle( dataID ) == 4 );
	AppendMenu(subMenu, MF_STRING | flag, startVal+4, _TEXT("Dash-Dot-Dot _ . .") );
	
	AppendMenu(addMenu, MF_POPUP, (UINT) subMenu, titleString.c_str() );
}

// Add a popup menu for the different marker types and frequencies
void	AddMarkerPopup( CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int dataID, int startVal )
{
	HMENU			shapeMenu,sizeMenu,freqMenu;
	int				i, markerShape, freq;
	UINT			flag;

	tostringstream ostr;
	tstring str;
	
	// Marker shape
	shapeMenu = CreatePopupMenu();
	markerShape = plot->GetMarkerType( dataID );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerNone );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerNone, _TEXT("&No marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerCircle );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerCircle, _TEXT("&Circle marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerSquareUpright );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerSquareUpright, _TEXT("&Upright square marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerSquareOblique );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerSquareOblique, _TEXT("&Oblique square marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleLeft );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerTriangleLeft, _TEXT("&Left riangle marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleRight );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerTriangleRight, _TEXT("&Right triangle marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleUp );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerTriangleUp, _TEXT("&Up triangle marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerTriangleDown );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerTriangleDown, _TEXT("&Down triangle marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerX );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerX, _TEXT("&X marker") );
	flag = GetPopupCheckedFlag( markerShape == kXYMarkerDot );
	AppendMenu(shapeMenu, MF_STRING | flag, startVal+kXYMarkerDot, _TEXT("&Dot marker") );
	
	AppendMenu(addMenu, MF_POPUP, (UINT) shapeMenu, _TEXT("&Marker shape") );
	
	// Marker Fill
	flag = GetPopupCheckedFlag( plot->GetMarkerFillState( dataID ) );
	AppendMenu(addMenu, MF_STRING | flag, startVal+kMenuMarkerFill, _TEXT("M&arker fill") );
	
	// Marker size
	sizeMenu = CreatePopupMenu();
	for( i = 0; i < nMarkerSizes; i++ )
	{
		flag = GetPopupCheckedFlag( plot->GetMarkerSize( dataID ) == (i+1) );
		ostr.str(_TEXT(""));
		ostr<<i+1;
		str=ostr.str();
		AppendMenu(sizeMenu, MF_STRING | flag, startVal+i+kMenuMarkerSize, str.c_str() );
	}
	AppendMenu(addMenu, MF_POPUP, (UINT) sizeMenu, _TEXT("Ma&rker size") );
	
	// Marker frequency
	freqMenu= CreatePopupMenu();
	freq = abs(plot->GetMarkerFrequency( dataID ));
	if( freq == 1 || freq ==0 ) freq = 10;
	flag = GetPopupCheckedFlag( plot->GetMarkerFrequency( dataID ) == 1 );
	AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency, _TEXT("&Mark every point") );
	flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) > 1 );
	ostr.str(_TEXT(""));
	ostr<<_TEXT("M&ark every ")<<freq<<_TEXT(" points");
	str=ostr.str();
	AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+1, str.c_str() );
	flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) < 0 );
	ostr.str(_TEXT(""));
	ostr<<_TEXT("Ma&rk ")<<freq<<_TEXT(" points");
	str=ostr.str();
	AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+2, str.c_str() );
	flag = GetPopupCheckedFlag( (plot->GetMarkerFrequency( dataID )) == 0 );
	str=_TEXT("Mar&k none");
	AppendMenu(freqMenu, MF_STRING | flag, startVal+kMenuMarkerFrequency+3, str.c_str() );
	
	AppendMenu(addMenu, MF_POPUP, (UINT) freqMenu, _TEXT("Mar&ker frequency") );
}

// Add the popup menu for a specific data set
void	AddDataPopup( CXYPlotImpl *plot, int whichDataSet, HMENU menu, int startVal )
{
	tstring			str,name;
	HMENU			dataMenu;
	HMENU			subMenu;
	int				dataID;
	
	if( whichDataSet < 0 || whichDataSet >= plot->GetDataSetCount() ) return;
	
	dataID = plot->GetDataID(whichDataSet);
	
	dataMenu = CreatePopupMenu();
	
	AddPlotTypePopup( dataMenu, _TEXT("&Plot type"), plot->GetPlotType( dataID ), startVal+kMenuChartType );
	
	AppendMenu(dataMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	AddDataLineSizePopup( plot, dataMenu, _TEXT("&Line size"), dataID, startVal + kMenuLineSize );
	
	AddDataLineStylePopup( plot, dataMenu, _TEXT("L&ine style"), dataID, startVal + kMenuLineStyle );
	
	AppendMenu(dataMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	AddMarkerPopup( plot, dataMenu, _TEXT("Marker"), dataID, startVal + kMenuMarker );
	
	AppendMenu(dataMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	subMenu = CreatePopupMenu();
	AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	AppendMenu(subMenu, MF_STRING, startVal+kMenuDataRange-5, _TEXT("&Load data...") );
	AppendMenu(subMenu, MF_STRING, startVal+kMenuDataRange-4, _TEXT("S&ave data...") );
	AppendMenu(subMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	AppendMenu(subMenu, MF_STRING, startVal+kMenuDataRange-3, _TEXT("&Erase data") );
	if(plot->IsVisible(plot->GetDataID(whichDataSet)))
		AppendMenu(subMenu, MF_STRING, startVal+kMenuDataRange-2, _TEXT("&Hide  data") );
	else
		AppendMenu(subMenu, MF_STRING, startVal+kMenuDataRange-2, _TEXT("&Show  data") );
	AppendMenu(dataMenu, MF_POPUP, (UINT) subMenu, _TEXT("&Data management") );
	// Add it to main menu
	AppendMenu(dataMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	AppendMenu(dataMenu, MF_STRING, startVal+kMenuDataRange-1, _TEXT("&Data Settings...") );
	
	
	str = plot->GetDataTitle( dataID );
	if( str == _TEXT("") )str = _TEXT("Untitled");
	tostringstream ostr;
	ostr<<str.c_str()<<_TEXT("\t("<<whichDataSet+1)<<_TEXT("/")<<plot->GetDataSetCount()<<_TEXT(")");
	name = ostr.str();
	if( name.length() > 24 )
	{
		name = str.substr(0, 24);
		name = str + _TEXT("...");
	}
	
	AppendMenu(menu, MF_POPUP, (UINT) dataMenu, name.c_str() );
}

void	AddGradientPopup( CXYPlotImpl *plot, HMENU addMenu, tstring titleString, int startVal)
{
	HMENU		subMenu;
	int			flag;

	subMenu = CreatePopupMenu();

	flag = GetPopupCheckedFlag( plot->IsGradientBkgnd() );
	AppendMenu(subMenu, MF_STRING | flag, startVal, _TEXT("&Enabled") );
	if(plot->IsGradientHorizontal())
		AppendMenu(subMenu, MF_STRING, startVal+1, _TEXT("&Vertical") );
	else
		AppendMenu(subMenu, MF_STRING, startVal+1, _TEXT("&Horizontal") );
	AppendMenu( subMenu, MF_STRING, startVal+2, _TEXT("&Start Color") );
	AppendMenu( subMenu, MF_STRING, startVal+3, _TEXT("Stop &Color") );

	AppendMenu(addMenu, MF_POPUP | MF_STRING, (UINT) subMenu, titleString.c_str() );
}

// Result here is rounded down to zero for the data set items
int		InterpretDataSetPopupItem( CXYPlotImpl *plot, HWND hWnd, int whichDataSet, int result )
{

	tstring		filename;
	int			dataID = plot->GetDataID(whichDataSet);

	// result is chart type
	if( result >= kMenuChartType && result < kMenuLineSize )
	{
		// These correlate perfectly
		plot->SetPlotType( dataID, result - kMenuChartType );

		return kPopupUpdate;
	}

	// result is line size
	if( result - kMenuLineSize >= 0 && result < kMenuLineSize + nLineSizes )
	{
		plot->SetDataLineSize( dataID, result - kMenuLineSize + 1 );

		return kPopupUpdate;
	}

	// result is line style
	if( result - kMenuLineStyle >= 0 && result < kMenuLineStyle + 5 )
	{
		plot->SetDataLineStyle( dataID, result - kMenuLineStyle );

		return kPopupUpdate;
	}

	// result is marker type
	if( result - kMenuMarker >= 0 && result - kMenuMarker < kMarkerTypeCount )
	{
		plot->SetMarkerType( dataID, result - kMenuMarker );
		return kPopupUpdate;
	}

	// marker fill item
	if( result - kMenuMarker == kMenuMarkerFill )
	{
		plot->SetMarkerFillState( dataID, !plot->GetMarkerFillState(dataID) );
		return kPopupUpdate;
	}

	// marker size popup
	if( result - kMenuMarker >= kMenuMarkerSize && result - kMenuMarker < kMenuMarkerSize+nMarkerSizes )
	{
		plot->SetMarkerSize( dataID, (result - (kMenuMarkerFill + kMenuMarker) ) );
		return kPopupUpdate;
	}

	// marker frequency
	if( result - kMenuMarker >= kMenuMarkerFrequency && result - kMenuMarker < kMenuMarkerFrequency + 4 )
	{
		int			tempResult = result - kMenuMarkerFrequency - kMenuMarker;
		int			freq = abs(plot->GetMarkerFrequency(dataID));
		
		if( freq == 1 || freq==0 ) freq = 10;
		
		switch( tempResult )
		{
		case 0: plot->SetMarkerFrequency( dataID, 1 ); break;
		case 1: plot->SetMarkerFrequency( dataID, freq ); break;
		case 2: plot->SetMarkerFrequency( dataID, -freq ); break;
		case 3: plot->SetMarkerFrequency( dataID, 0 ); break;
		default: break;
		}
		
		return kPopupUpdate;
	}

	// Load data
	if( result == kMenuDataRange - 5)
	{
		if( !ShowFileDialog(hWnd, true, filename) )
			return kPopupNoAction;
		if(plot->GetDataSet(plot->GetDataID(whichDataSet))->ReadFromFile(filename.c_str()))
		{
			plot->SetDataModified(plot->GetDataID(whichDataSet));
			//plot->GetXYDataInfo()[whichDataSet].bDataRangesSet = false;
			//plot->GetDataRanges();
			MessageBox(hWnd, _TEXT("Load data OK"), _TEXT("Infomation"), MB_OK);
			return kPopupUpdate;
		}
		else
		{
			return kPopupNoAction;
		}
	}

	// Save data
	if( result == kMenuDataRange - 4)
	{
		if( !ShowFileDialog(hWnd, false, filename) )
			return kPopupNoAction;
		if(plot->GetDataSet(plot->GetDataID(whichDataSet))->WriteToFile(filename.c_str()))
		{
			MessageBox(hWnd, _TEXT("Save data OK"), _TEXT("Infomation"), MB_OK);
		}

		return kPopupNoAction;
	}

	// delete this curve
	if( result == kMenuDataRange - 3)
	{
		plot->DeleteData(plot->GetDataID(whichDataSet));
		return kPopupUpdate;
	}
	// hide this curve
	if( result == kMenuDataRange - 2)
	{
		plot->SetVisible(plot->GetDataID(whichDataSet), !plot->IsVisible(plot->GetDataID(whichDataSet)));
		return kPopupUpdate;		
	}
	return kPopupNoAction;
}

// Adds the menus that pertain to the axis, including auto ranging,
// title size, label size, tick marks, etc. This is only called
// by the popup menu constructors for the chart types.
void	AddAxisPopup( CXYPlotImpl *plot, HMENU menu, int which, int startVal )
{
	UINT			flag;
	tstring			str;
	HMENU			axisMenu;
	
	axisMenu = CreatePopupMenu();
	
	flag = GetPopupCheckedFlag( plot->GetAxis(which)->IsRangeSet() == false );
	AppendMenu(axisMenu, MF_STRING | flag, startVal, _TEXT("&Auto range") );
	
	// Font sizes
	AddFontSizePopup( axisMenu, _TEXT("&Title size"), plot->GetAxis(which)->GetTitleFont().lfHeight, startVal+kMenuAxisTitleSize );
	AddFontSizePopup( axisMenu, _TEXT("&Label size"), plot->GetAxis(which)->GetLabelFont().lfHeight, startVal+kMenuAxisLabelSize );
	
	AppendMenu(axisMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	// Number of major and minor ticks
	//AddNumberOfTicksPopup( &axisMenu, "Major ticks", m_nTickCount+1, startVal + kMenuAxisMajorTickCount );
	//AddNumberOfTicksPopup( &axisMenu, "Minor ticks", m_nMinorTickCount, startVal + kMenuAxisMinorTickCount );
	AddNumberOfTicksPopup( axisMenu, _TEXT("&Major ticks"), plot->GetAxis(which)->GetTickCount()+1, startVal + kMenuAxisMajorTickCount, plot->GetAxis(which)->IsAutoMajorTick() );
	AddNumberOfTicksPopup( axisMenu, _TEXT("M&inor ticks"), plot->GetAxis(which)->GetMinorTickCount(), startVal + kMenuAxisMinorTickCount, plot->GetAxis(which)->IsAutoMinorTick() );
	
	// Add it to main menu
	switch( plot->GetAxis(which)->GetLocation() )
	{
	case kLocationLeft: str = _TEXT("&Left Axis"); break;
	case kLocationRight: str = _TEXT("&Right Axis"); break;
	case kLocationTop: str = _TEXT("&Top Axis"); break;
	case kLocationBottom: str = _TEXT("&Bottom Axis"); break;
	default: str = _TEXT("Error"); break;
	}
	
	AppendMenu(axisMenu, MF_SEPARATOR, 0, (LPCTSTR)0 );
	
	// Visible
	if( plot->GetAxis(which)->IsVisible() )
		AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-2, _TEXT("&Hide Axis") );
	else
		AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-2, _TEXT("&Show Axis") );
	// A settings item is always useful
	AppendMenu(axisMenu, MF_STRING, startVal+kMenuAxisRange-1, _TEXT("&Axis Settings...") );
	
	AppendMenu(menu, MF_POPUP, (UINT) axisMenu, str.c_str() );
}


// Adds a popup menu for the number of ticks
//void	CAxis::AddNumberOfTicksPopup( CMenu *addMenu, LPCSTR titleString, long compare, int startVal )
void	AddNumberOfTicksPopup( HMENU addMenu, tstring titleString, long compare, int startVal, BOOL bAuto )
{
	HMENU			subMenu = CreatePopupMenu();
	UINT			flag;

	//flag = GetPopupCheckedFlag( compare <= 0 );
	flag = GetPopupCheckedFlag( bAuto );
	AppendMenu(subMenu, MF_STRING | flag, startVal, _TEXT("Auto") );

	for( int i = 0; i < nTickSizes; i++ )
	{
		//flag = GetPopupCheckedFlag( compare == tickSizes[i] );
		flag = GetPopupCheckedFlag( compare == tickSizes[i] && !bAuto );
		tostringstream ostr;
		tstring str;
		ostr<<tickSizes[i];
		str=ostr.str();
		AppendMenu(subMenu, MF_STRING | flag, startVal+i+1, str.c_str() );
	}

	AppendMenu(addMenu, MF_POPUP, (UINT) subMenu, titleString.c_str() );
}


// Takes a return value from the popup menu and figures
// out what menu function it was. The value of "which"
// should be 0 for the first axis menu item.
int		InterpretAxisPopupItem( CXYPlotImpl *plot, int which, int whichResult )
{
	if( whichResult < 0 ) return kPopupError;

	// Did user say "auto range"?
	//if( whichResult == 0 ) { m_bRangeSet = !m_bRangeSet; return kPopupUpdate; }
	if( whichResult == 0 ) { plot->GetAxis(which)->SetRangeSet(!plot->GetAxis(which)->IsRangeSet()); return kPopupUpdate; }

	// font sizes
	if( whichResult < kMenuAxisLabelSize + nFontSizes )
	{
		if( whichResult < kMenuAxisTitleSize + nFontSizes ) // title font
		{
			//m_lfTitleFont.lfHeight = fontSizes[whichResult - kMenuAxisTitleSize];
			plot->GetAxis(which)->GetTitleFont().lfHeight=fontSizes[whichResult - kMenuAxisTitleSize];
		}
		else // label font
		{
			plot->GetAxis(which)->GetLabelFont().lfHeight = fontSizes[whichResult - kMenuAxisLabelSize];
		}


		return kPopupUpdate;
	}

	// Number of ticks
	if( whichResult >= kMenuAxisMajorTickCount && whichResult < kMenuAxisMajorTickCount + nTickSizes + 1 ) // major ticks
	{
		//if( whichResult - kMenuAxisMajorTickCount == 0 ) m_nTickCount = -abs(m_nTickCount);
		if( whichResult - kMenuAxisMajorTickCount == 0 )
		{
			//m_bAutoMajorTick	 = true;
			//m_nTickCount = abs(m_nTickCount);
			plot->GetAxis(which)->SetAutoMajorTick(true);
			plot->GetAxis(which)->SetTickCount(-abs(plot->GetAxis(which)->GetTickCount()));
		}
		else
		{
			//m_bAutoMajorTick  = FALSE;
			//m_nTickCount = tickSizes[whichResult-kMenuAxisMajorTickCount-1] - 1;
			plot->GetAxis(which)->SetAutoMajorTick(false);
			plot->GetAxis(which)->SetTickCount(tickSizes[whichResult-kMenuAxisMajorTickCount-1] - 1);
		}
		return kPopupUpdate;
	}
	else if( whichResult >= kMenuAxisMinorTickCount && whichResult < kMenuAxisMinorTickCount + nTickSizes + 1 ) // minor ticks
	{
		//if( (whichResult-kMenuAxisMinorTickCount) == 0 ) m_nMinorTickCount = -abs(m_nTickCount);
		if( (whichResult-kMenuAxisMinorTickCount) == 0 )
		{
			//m_bAutoMinorTick  = true;
			//m_nMinorTickCount = abs(m_nTickCount);
			plot->GetAxis(which)->SetAutoMinorTick(true);
			plot->GetAxis(which)->SetMinorTickCount(abs(plot->GetAxis(which)->GetMinorTickCount()));
		}
		else
		{
			//m_bAutoMinorTick  = false;
			//m_nMinorTickCount = tickSizes[(whichResult-kMenuAxisMinorTickCount-1)];
			plot->GetAxis(which)->SetAutoMinorTick(false);
			plot->GetAxis(which)->SetMinorTickCount(tickSizes[(whichResult-kMenuAxisMinorTickCount-1)]);
		}
		return kPopupUpdate;
	}

	// Visible
	else if( whichResult == kMenuAxisRange - 2 )
	{
		plot->GetAxis(which)->SetVisible(!plot->GetAxis(which)->IsVisible());
		return kPopupUpdate;
	}
	// Settings (unfortunately, have to do this one in parent)

	return kPopupError;
}
