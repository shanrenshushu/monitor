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
#include "ColorControl.h"
//#include <commdlg.h>
#include <commctrl.h>
#include <tchar.h>
#include "DefHandlers.h"
#include <sstream>

/////////////////////////////////////////////////////////////////////////
// Data Settings

typedef struct
{
	CXYPlotImpl *plot;
	int whichDataSet;
}DataSettingParam;

BOOL CALLBACK DefDataSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK DefDataSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

BOOL CALLBACK DataTitleSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK DataLineSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK DataChartTypeSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK DataFillSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK DataMarkersSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK DataManagementSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

//Can be used only in the first callback function
bool SetupDataFromPlot(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
bool SaveDataToPlot(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);

void SetupDataForTitle(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SaveDataForTitle(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SetupDataForLine(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SaveDataForLine(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SetupDataForChartType(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SaveDataForChartType(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SetupDataForFill(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SaveDataForFill(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SetupDataForMarkers(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SaveDataForMarkers(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SetupDataForManagement(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);
void SaveDataForManagement(HWND hWnd, CXYPlotImpl *plot, int whichDataSet);

void InitAllTabs(HWND *hPages, DataSettingParam *para);



/////////////////////////////////////////////////////////////////////////

bool DefDataSettings( CXYPlotImpl *plot, HWND hWnd, int whichDataSet )
{
	DataSettingParam para;
	para.plot = plot;
	para.whichDataSet = whichDataSet;
	if(!CGlobal::m_bNewInface)
	{
		if(DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATA_SETTINGS), hWnd, (DLGPROC)DefDataSettingsFunc, (LPARAM)&para))
			return true;
		else
			return false;
	}
	else
	{
		if(DialogBoxParam(CGlobal::MyGetModuleHandle(),MAKEINTRESOURCE(IDD_DATASETTINGS), hWnd, (DLGPROC)DefDataSettingsFNew, (LPARAM)&para))
			return true;
		else
			return false;
	}
	
}

BOOL CALLBACK DefDataSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl *plot;
	static int whichDataSet;
	static DataSettingParam *para;
	
	int num;
	
	tstring filename;
	
	
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;
		SetupDataFromPlot(hWnd, plot, whichDataSet);
		return TRUE;
	case WM_COMMAND:
		if(HIWORD (wparam) == BN_CLICKED)
		{
			if(LOWORD(wparam) == IDC_DATA_PREVIOUS || LOWORD(wparam) == IDC_DATA_NEXT)
			{
				SaveDataToPlot(hWnd, plot, whichDataSet);
				if(LOWORD(wparam)==IDC_DATA_PREVIOUS)
				{
					num = whichDataSet-1;
					if(num<0)num=plot->GetDataSetCount()-1;
				}
				if(LOWORD(wparam)==IDC_DATA_NEXT)
				{
					num = whichDataSet+1;
					if(num>=plot->GetDataSetCount())num=0;
				}
				para->whichDataSet = num;
				SendMessage(hWnd, WM_INITDIALOG, 0, (LPARAM)para);
			}
			if(LOWORD(wparam) == IDC_DATA_LOAD)
			{
				if(ShowFileDialog(hWnd, true, filename))
					plot->ReadFromFile(whichDataSet, filename.c_str());
			}
			if(LOWORD(wparam) == IDC_DATA_SAVE)
			{
				if(ShowFileDialog(hWnd, false, filename))
					plot->WriteToFile(whichDataSet, filename.c_str());
			}
		}
		switch(LOWORD(wparam))
		{
		case IDOK:
			SaveDataToPlot(hWnd, plot, whichDataSet);
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

const int nPages=7;

BOOL CALLBACK DefDataSettingsFNew(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl *plot;
	static int whichDataSet;
	static DataSettingParam *para;

	static HWND hPage[nPages];
	static int curpage = 0;
	TCITEM tcitem;
	RECT rt;

	LPNMHDR pnmh;
	

	int i,num;
	tostringstream ostr;
	tstring str;
	UINT style;
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;

		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_DELETEALLITEMS, 0, 0);
		// add pages
		ZeroMemory(&tcitem, sizeof(TCITEM));
		tcitem.mask = TCIF_PARAM | TCIF_TEXT;
		tcitem.iImage = -1;
		tcitem.pszText = _T("Title");
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 0, (LPARAM)&tcitem);
		tcitem.pszText = _T("Line");
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 1, (LPARAM)&tcitem);
		tcitem.pszText = _T("Plot type");
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 2, (LPARAM)&tcitem);
		tcitem.pszText = _T("Fill Curve");
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 3, (LPARAM)&tcitem);
		tcitem.pszText = _T("Markers");
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 4, (LPARAM)&tcitem);
		tcitem.pszText = _T("Management");
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 5, (LPARAM)&tcitem);
		tcitem.pszText = _T("Copyright");
		SendDlgItemMessage(hWnd, IDC_DATA_SETTINGS_TAB, TCM_INSERTITEM, 6, (LPARAM)&tcitem);

		hPage[0] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_TITLE), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), DataTitleSettingsFunc, (LPARAM)para);
		hPage[1] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_LINE), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), DataLineSettingsFunc, (LPARAM)para);
		hPage[2] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_CHARTTYPE), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), DataChartTypeSettingsFunc, (LPARAM)para);
		hPage[3] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_FILL), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), DataFillSettingsFunc, (LPARAM)para);
		hPage[4] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_MAKERS), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), DataMarkersSettingsFunc, (LPARAM)para);
		hPage[5] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_DATASETTINGS_MANAGEMENT), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), DataManagementSettingsFunc, (LPARAM)para);
		hPage[6] = CreateDialogParam(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDD_XYSETTINGS_COPYRIGHT), GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), PlotCopyrightDlgFunc, (LPARAM)para);
		style = GetWindowLong(hPage[5], GWL_STYLE);
		SetWindowLong(hPage[6],  GWL_STYLE, style);

		GetClientRect(GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB), &rt);
		rt.top += 20;
		rt.bottom -= 8;
		rt.left += 8;
		rt.right -= 8;

		for(i=0; i<nPages; i++)
		{
			MoveWindow(hPage[i], rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top, TRUE);
			if(i==curpage)
			{
				ShowWindow(hPage[i], SW_SHOW);
			}
			else
			{
				ShowWindow(hPage[i], SW_HIDE);
			}
		}
		
		//create the combo box
		SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_RESETCONTENT, 0, 0);
		for(i=plot->GetDataSetCount()-1; i>=0; i--)
		{
			ostr.str(_TEXT(""));
			ostr<<_TEXT("Data ")<<i+1<<_TEXT(" of ")<<plot->GetDataSetCount()<<_TEXT(": ")<<plot->GetDataTitle(plot->GetDataID(i)).c_str();
			str=ostr.str();
			SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_INSERTSTRING, 0, (LPARAM)str.c_str());
		}
		SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_SETCURSEL, (WPARAM)whichDataSet, 0);

		// check the save when switching box
		SendDlgItemMessage(hWnd, IDC_DS_SAVE_WHEN_SWITCHING, BM_SETCHECK, FALSE, 0);

		return TRUE;
	case WM_DESTROY:
		for(i=0; i<nPages; i++)
		{
			if(IsWindow(hPage[i]))
				DestroyWindow(hPage[i]);
		}
		return TRUE;
	case WM_COMMAND:
		if(HIWORD (wparam) == CBN_SELCHANGE && LOWORD(wparam) == IDC_DS_LISTDATASETS )
		{
			if( SendDlgItemMessage(hWnd, IDC_DS_SAVE_WHEN_SWITCHING, BM_GETCHECK, 0, 0)==BST_CHECKED )
			{
				for(i=0; i<nPages; i++)
				{
					SendMessage(hPage[i], WM_COMMAND, IDOK, 0);
				}
				ostr.str(_TEXT(""));
				ostr<<_TEXT("Data ")<<whichDataSet+1<<_TEXT(" of ")<<plot->GetDataSetCount()<<_TEXT(": ")<<plot->GetDataTitle(plot->GetDataID(whichDataSet)).c_str();
				str=ostr.str();
				SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_INSERTSTRING, (WPARAM)whichDataSet, (LPARAM)str.c_str());
				SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_DELETESTRING, (WPARAM)(whichDataSet+1), 0);
			}

			whichDataSet = SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_GETCURSEL, 0, 0);
			para->whichDataSet = whichDataSet;
			InitAllTabs(hPage, para);
			return TRUE;
		}
		if(HIWORD (wparam) == BN_CLICKED)
		{
			if(LOWORD(wparam)==IDC_DATA_PREVIOUS || LOWORD(wparam)==IDC_DATA_NEXT)
			{
				if( SendDlgItemMessage(hWnd, IDC_DS_SAVE_WHEN_SWITCHING, BM_GETCHECK, 0, 0)==BST_CHECKED )
				{
					for(i=0; i<nPages; i++)
					{
						SendMessage(hPage[i], WM_COMMAND, IDOK, 0);
					}
					ostr.str(_TEXT(""));
					ostr<<_TEXT("Data ")<<whichDataSet+1<<_TEXT(" of ")<<plot->GetDataSetCount()<<_TEXT(": ")<<plot->GetDataTitle(plot->GetDataID(whichDataSet)).c_str();
					str=ostr.str();
					SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_INSERTSTRING, (WPARAM)whichDataSet, (LPARAM)str.c_str());
					SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_DELETESTRING, (WPARAM)(whichDataSet+1), 0);
				}

				//num = SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_GETCURSEL, 0, 0);
				num = whichDataSet;
				
				if(LOWORD(wparam)==IDC_DATA_PREVIOUS)
				{
					num--;
					if (num<0 ) num = plot->GetDataSetCount()-1;
				}
				else
				{
					num++;
					if( num>= plot->GetDataSetCount() )num=0;
				}
				SendDlgItemMessage(hWnd, IDC_DS_LISTDATASETS, CB_SETCURSEL, (WPARAM)num, 0);
				whichDataSet = num;
				para->whichDataSet = whichDataSet;
				InitAllTabs(hPage, para);
				return TRUE;
			}
		
		}
		switch(LOWORD(wparam))
		{
		case IDOK:
			for(i=0; i<nPages; i++)
			{
				SendMessage(hPage[i], WM_COMMAND, IDOK, 0);
			}
			
			EndDialog(hWnd, 1);
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return TRUE;
		}
		break;
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
			case TCN_SELCHANGE:
			curpage = TabCtrl_GetCurSel(GetDlgItem(hWnd, IDC_DATA_SETTINGS_TAB));
			for(i=0; i<nPages; i++)
			{
				if(i!=curpage)
					ShowWindow(hPage[i], SW_HIDE);
				else
					ShowWindow(hPage[i], SW_SHOW);
			}
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK DataTitleSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static DataSettingParam *para;
	static CXYPlotImpl	*plot;
	static int whichDataSet;
	
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;
		
		// Deal with the title
		SetupDataForTitle(hWnd, plot, whichDataSet);
		return TRUE;
	case WM_COMMAND:
		
		switch(LOWORD(wparam))
		{
		case IDOK:
			SaveDataForTitle(hWnd, plot, whichDataSet);
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

BOOL CALLBACK DataLineSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static DataSettingParam *para;
	static CXYPlotImpl	*plot;
	static int whichDataSet;
	
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;
		
		// Deal with the title
		SetupDataForLine(hWnd, plot, whichDataSet);
		return TRUE;
	case WM_COMMAND:
		
		switch(LOWORD(wparam))
		{
		case IDOK:
			SaveDataForLine(hWnd, plot, whichDataSet);
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

BOOL CALLBACK DataChartTypeSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static DataSettingParam *para;
	static CXYPlotImpl	*plot;
	static int whichDataSet;
	
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;
		
		// Deal with the title
		SetupDataForChartType(hWnd, plot, whichDataSet);
		return TRUE;
	case WM_COMMAND:
		
		switch(LOWORD(wparam))
		{
		case IDOK:
			SaveDataForChartType(hWnd, plot, whichDataSet);
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

BOOL CALLBACK DataFillSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static DataSettingParam *para;
	static CXYPlotImpl	*plot;
	static int whichDataSet;
	
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;
		
		// Deal with the title
		SetupDataForFill(hWnd, plot, whichDataSet);
		return TRUE;
	case WM_COMMAND:
		
		switch(LOWORD(wparam))
		{
		case IDOK:
			SaveDataForFill(hWnd, plot, whichDataSet);
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

BOOL CALLBACK DataMarkersSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static DataSettingParam *para;
	static CXYPlotImpl	*plot;
	static int whichDataSet;
	
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;
		
		// Deal with the title
		SetupDataForMarkers(hWnd, plot, whichDataSet);
		return TRUE;
	case WM_COMMAND:
		
		switch(LOWORD(wparam))
		{
		case IDOK:
			SaveDataForMarkers(hWnd, plot, whichDataSet);
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

BOOL CALLBACK DataManagementSettingsFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static DataSettingParam *para;
	static CXYPlotImpl	*plot;
	static int whichDataSet;
	
	switch(message)
	{
	case WM_INITDIALOG:
		para = (DataSettingParam *)lparam;
		plot = para->plot;
		whichDataSet = para->whichDataSet;
		
		// Deal with the title
		SetupDataForManagement(hWnd, plot, whichDataSet);
		return TRUE;
	case WM_COMMAND:
		
		switch(LOWORD(wparam))
		{
		case IDOK:
			SaveDataForManagement(hWnd, plot, whichDataSet);
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}

bool SetupDataFromPlot(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	tostringstream ostr;
	tstring str;

	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	// data set index
	ostr<<_TEXT("Data set ")<<whichDataSet+1<<_TEXT(" of ")<<plot->GetDataSetCount();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DATASET_LABEL, WM_SETTEXT, 0, (LPARAM)str.c_str());

	SetupDataForTitle(hWnd, plot, whichDataSet);
	SetupDataForLine(hWnd, plot, whichDataSet);
	SetupDataForChartType(hWnd, plot, whichDataSet);
	SetupDataForFill(hWnd, plot, whichDataSet);
	SetupDataForMarkers(hWnd, plot, whichDataSet);
	SetupDataForManagement(hWnd, plot, whichDataSet);

	return true;
}

bool SaveDataToPlot(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);

	SaveDataForTitle(hWnd, plot, whichDataSet);
	SaveDataForLine(hWnd, plot, whichDataSet);
	SaveDataForChartType(hWnd, plot, whichDataSet);
	SaveDataForFill(hWnd, plot, whichDataSet);
	SaveDataForMarkers(hWnd, plot, whichDataSet);
	SaveDataForManagement(hWnd, plot, whichDataSet);

	return true;
}

void SetupDataForTitle(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{	
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);

	// Deal with the title
	SendDlgItemMessage(hWnd, IDC_DS_DATA_TITLE, WM_SETTEXT, 0, (LPARAM)data->GetTitle().c_str());
}
void SaveDataForTitle(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	TCHAR	strtmp[1024];
	
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	// Deal with the title
	GetWindowText(GetDlgItem(hWnd, IDC_DS_DATA_TITLE), strtmp, 1024);
	data->SetTitle(strtmp);
}

void SetupDataForLine(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	tostringstream ostr;
	tstring str;
	
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	
	// Deal with the line
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_LINE, WM_SETCOLOR, 0, plot->GetDataColor(plot->GetDataID(whichDataSet)));
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_AUTO, BM_SETCHECK, plot->IsAutoColor(plot->GetDataID(whichDataSet)), 0);
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE_AUTO, BM_SETCHECK, plot->IsAutoLineStyle(plot->GetDataID(whichDataSet)), 0);
	ostr<<plot->GetDataLineSize(plot->GetDataID(whichDataSet));
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DS_LINE_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Solid _______________________"));
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dashed _ _ _ _ _ _ _ _ _ _ _ _ _ _"));
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dotted . . . . . . . . . . . . . . . . . . . . . ."));
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot _ . _ . _ . _ . _ . _ . _ . _ ."));
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dash-Dot-Dot _ . . _ . . _ . . _ . . _ ."));
	SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_SETCURSEL, (WPARAM)plot->GetDataLineStyle(plot->GetDataID(whichDataSet)), 0);
}
void SaveDataForLine(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	TCHAR	strtmp[1024];
	
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	int			check,num;
	COLORREF	color;
	
	// Deal with the line
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_LINE, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetDataColor(plot->GetDataID(whichDataSet), color);
	check = SendDlgItemMessage(hWnd, IDC_DS_COLOR_AUTO, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetAutoColor(plot->GetDataID(whichDataSet), true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetAutoColor(plot->GetDataID(whichDataSet), false);
	}
	check = SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE_AUTO, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetAutoLineStyle(plot->GetDataID(whichDataSet), true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetAutoLineStyle(plot->GetDataID(whichDataSet), false);
	}
	GetWindowText(GetDlgItem(hWnd, IDC_DS_LINE_SIZE), strtmp, 1024);
	plot->SetDataLineSize(plot->GetDataID(whichDataSet), _ttoi(strtmp));
	num = SendDlgItemMessage(hWnd, IDC_DS_LINESTYLE, CB_GETCURSEL, 0, 0);
	plot->SetDataLineStyle(plot->GetDataID(whichDataSet), num);
}

void SetupDataForChartType(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	
	// Deal with the chart type
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Scatter"));
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Connected"));
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Step H-V"));
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Step V-H"));
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Step H-V-H"));
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Step V-H-V"));
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Bezier"));
	SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_SETCURSEL, (WPARAM)plot->GetPlotType(plot->GetDataID(whichDataSet)), 0);
}
void SaveDataForChartType(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	int			num;
	
	// Deal with the chart type
	num = SendDlgItemMessage(hWnd, IDC_DS_CHART_TYPE, CB_GETCURSEL, 0, 0);
	plot->SetPlotType(plot->GetDataID(whichDataSet), num);
}

void SetupDataForFill(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data;
	
	data = plot->GetAbsoluteDataSet(whichDataSet);
	
	// Deal with the fill enclosed curve
	SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILL, BM_SETCHECK, plot->IsDataFill(plot->GetDataID(whichDataSet)), 0);
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_ENCLOSEDCURVE, WM_SETCOLOR, 0, plot->GetDataFillColor(plot->GetDataID(whichDataSet)));
}
void SaveDataForFill(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	int			check;
	COLORREF	color;

	// Deal with the fill enclosed curve
	check = SendDlgItemMessage(hWnd, IDC_DS_ENCLOSEDCURVE_FILL, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetDataFill(plot->GetDataID(whichDataSet), true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetDataFill(plot->GetDataID(whichDataSet), false);
	}
	SendDlgItemMessage(hWnd, IDC_DS_COLOR_ENCLOSEDCURVE, WM_GETCOLOR, 0, (LPARAM)&color); 
	plot->SetDataFillColor(plot->GetDataID(whichDataSet), color);
}

void SetupDataForMarkers(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	tostringstream ostr;
	tstring str;
	
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	int		num;
	
	// Deal with the markers
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("No markers"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Circle"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Upright square"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Oblique square"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Left triangle"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Right triangle"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Up triangle"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Down triangle"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("X"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_ADDSTRING, 0, (LPARAM)_TEXT("Dot"));
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_SETCURSEL, (WPARAM)plot->GetMarkerType(plot->GetDataID(whichDataSet)), 0);
	ostr<<plot->GetMarkerSize(plot->GetDataID(whichDataSet));
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_SIZE, WM_SETTEXT, 0, (LPARAM)str.c_str());
	SendDlgItemMessage(hWnd, IDC_DS_MARKER_FILL, BM_SETCHECK, plot->GetMarkerFillState(plot->GetDataID(whichDataSet)), 0);
	num = plot->GetMarkerFrequency(plot->GetDataID(whichDataSet));
	if(num == 0)
	{
		CheckRadioButton(hWnd,IDC_DS_MARKER_FREQ_EVERY,IDC_DS_MARKER_FREQ_NONE,IDC_DS_MARKER_FREQ_NONE);
		ostr.str(_TEXT(""));
		ostr<<10;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_NUMBER, WM_SETTEXT, 0, (LPARAM)str.c_str());
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA, WM_SETTEXT, 0, (LPARAM)str.c_str());
	}
	else if(num == 1)
	{
		CheckRadioButton(hWnd,IDC_DS_MARKER_FREQ_EVERY,IDC_DS_MARKER_FREQ_NONE,IDC_DS_MARKER_FREQ_EVERY);
		ostr.str(_TEXT(""));
		ostr<<10;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_NUMBER, WM_SETTEXT, 0, (LPARAM)str.c_str());
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA, WM_SETTEXT, 0, (LPARAM)str.c_str());
	}
	else if(num > 0)
	{
		CheckRadioButton(hWnd,IDC_DS_MARKER_FREQ_EVERY,IDC_DS_MARKER_FREQ_NONE,IDC_DS_MARKER_FREQ_NTH);
		ostr.str(_TEXT(""));
		ostr<<num;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_NUMBER, WM_SETTEXT, 0, (LPARAM)str.c_str());
		ostr.str(_TEXT(""));
		ostr<<10;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA, WM_SETTEXT, 0, (LPARAM)str.c_str());
	}
	else if(num <0)
	{
		CheckRadioButton(hWnd,IDC_DS_MARKER_FREQ_EVERY,IDC_DS_MARKER_FREQ_NONE,IDC_DS_MARKER_FREQ_TOTAL);
		ostr.str(_TEXT(""));
		ostr<<10;
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_NUMBER, WM_SETTEXT, 0, (LPARAM)str.c_str());
		ostr.str(_TEXT(""));
		ostr<<abs(num);
		str=ostr.str();
		SendDlgItemMessage(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA, WM_SETTEXT, 0, (LPARAM)str.c_str());
	}
}
void SaveDataForMarkers(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	TCHAR	strtmp[1024];
	
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);
	int			check,num;
	
	// Deal with the markers
	num = SendDlgItemMessage(hWnd, IDC_DS_MARKER_SHAPE, CB_GETCURSEL, 0, 0);
	plot->SetMarkerType(plot->GetDataID(whichDataSet), num);
	GetWindowText(GetDlgItem(hWnd, IDC_DS_MARKER_SIZE), strtmp, 1024);
	plot->SetMarkerSize(plot->GetDataID(whichDataSet), _ttoi(strtmp));
	check = SendDlgItemMessage(hWnd, IDC_DS_MARKER_FILL, BM_GETCHECK, 0, 0);
	if(check==BST_CHECKED)
	{
		plot->SetMarkerFillState(plot->GetDataID(whichDataSet), true);
	}
	else if(check==BST_UNCHECKED)
	{
		plot->SetMarkerFillState(plot->GetDataID(whichDataSet), false);
	}
	
	if( IsDlgButtonChecked(hWnd, IDC_DS_MARKER_FREQ_EVERY) == BST_CHECKED)
	{
		plot->SetMarkerFrequency(plot->GetDataID(whichDataSet), 1);
	}
	else if( IsDlgButtonChecked(hWnd, IDC_DS_MARKER_FREQ_NTH) == BST_CHECKED)
	{
		GetWindowText(GetDlgItem(hWnd, IDC_DS_MARKER_FREQ_NUMBER), strtmp, 1024);
		plot->SetMarkerFrequency(plot->GetDataID(whichDataSet), _ttoi(strtmp));
	}
	else if( IsDlgButtonChecked(hWnd, IDC_DS_MARKER_FREQ_TOTAL) == BST_CHECKED)
	{
		GetWindowText(GetDlgItem(hWnd, IDC_DS_MARKER_FREQ_TOTAL_DATA), strtmp, 1024);
		plot->SetMarkerFrequency(plot->GetDataID(whichDataSet), -abs(_ttoi(strtmp)) );
	}
	else if( IsDlgButtonChecked(hWnd, IDC_DS_MARKER_FREQ_NONE) == BST_CHECKED)
	{
		plot->SetMarkerFrequency(plot->GetDataID(whichDataSet), 0);
	}
}

void SetupDataForManagement(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	CPlotData<DataPoint2D> *data = plot->GetAbsoluteDataSet(whichDataSet);

	tostringstream ostr;
	tstring str;
	ostr<<data->GetDataSize();
	str=ostr.str();
	SendDlgItemMessage(hWnd, IDC_DS_NUMOFPOINTS, WM_SETTEXT, 0, (LPARAM)str.c_str());
}
void SaveDataForManagement(HWND hWnd, CXYPlotImpl *plot, int whichDataSet)
{
	return;
}

void InitAllTabs(HWND *hPages, DataSettingParam *para)
{
	for(int i=0; i<nPages; i++)
	{
		SendMessage(hPages[i], WM_INITDIALOG, 0, (LPARAM)para);
	}
}