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
#include "../../Resources/PlotResources.h"
#include "DefHandlers.h"
#include "../XYPlotImpl.h"

#include <sstream>
#include <windows.h>
#include <commdlg.h>

// Returns a string containing the name and font size of
// the passed font
tstring	GetFontDescription( LOGFONT *font )
{
	if( font == NULL ) return _TEXT("");

	tostringstream ostr;
	ostr<<font->lfHeight<<_TEXT(" point ")<<font->lfFaceName;
	return ostr.str();
}

UINT	GetPopupCheckedFlag( BOOL how )
{
	if( how ) return MF_CHECKED;
	else return MF_UNCHECKED;
}

bool	ShowFileDialog(HWND hWnd, bool open, tstring &filename, LPCTSTR filter)
{
	OPENFILENAME ofn;       // common dialog box structure
	const int nMaxFile = 1024;
	TCHAR szFile[nMaxFile];       // buffer for file name
	
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	//szFile[0] = TCHAR('\0'); 
	ZeroMemory(szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME); 
	ofn.hwndOwner = hWnd; 
	ofn.hInstance = CGlobal::MyGetModuleHandle();
	ofn.lpstrFilter = filter; 
	ofn.lpstrCustomFilter = _TEXT(""); 
	ofn.nMaxCustFilter = 0L; 
	ofn.nFilterIndex = 1L; 
	ofn.lpstrFile= szFile; 
	ofn.nMaxFile = nMaxFile; 
	ofn.lpstrFileTitle = NULL; 
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = _TEXT(""); 
	ofn.lpstrTitle = NULL; 
	ofn.lpstrDefExt = _TEXT("txt");
	ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; 
	ofn.nFileOffset = 0; 
	ofn.nFileExtension = 0; 
	
	//OleInitialize(NULL);
	if(open)
	{
		if( GetOpenFileName(&ofn) == TRUE)
		{
			filename = ofn.lpstrFile;
			return true;
		}
		else
		{
			UINT errCode = CommDlgExtendedError();

			tostringstream ostr;
			ostr<<_TEXT("Open File Dialog failed, Error code is ")<<errCode<<_TEXT(" .");
			tstring str = ostr.str();
			MessageBox(hWnd, str.c_str(),_TEXT("Error"), MB_OK);
			if(errCode == CDERR_INITIALIZATION)
			{
				MessageBox(hWnd, _TEXT("Initialization failed!!"), _TEXT("Error"), MB_OK);
			}
		}
	}
	else
	{
		if( GetSaveFileName(&ofn) == TRUE)
		{
			filename = ofn.lpstrFile;
			return true;
		}
		else
		{
			UINT errCode = CommDlgExtendedError();
			
			tostringstream ostr;
			ostr<<_TEXT("Open File Dialog failed, Error code is ")<<errCode<<_TEXT(" .");
			tstring str = ostr.str();
			MessageBox(hWnd, str.c_str(),_TEXT("Error"), MB_OK);
			if(errCode == CDERR_INITIALIZATION)
			{
				MessageBox(hWnd, _TEXT("Initialization failed!"), _TEXT("Error"), MB_OK);
			}
		}
	}
	return false;
}

bool	ShowFontDialog(HWND hWnd, LOGFONT &logfont, COLORREF &color)
{
	CHOOSEFONT cf;

	ZeroMemory(&cf, sizeof(CHOOSEFONT));
	cf.lStructSize = sizeof (CHOOSEFONT);
	cf.hwndOwner = hWnd;
	cf.hDC = (HDC)NULL; 
	cf.lpLogFont = &logfont;
	cf.iPointSize = 0; 
	cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	cf.rgbColors = color;
	cf.lCustData = 0L;
	cf.lpfnHook = (LPCFHOOKPROC)NULL; 
	cf.lpTemplateName = _TEXT(""); 
	cf.hInstance = (HINSTANCE) NULL; 
	cf.lpszStyle = _TEXT(""); 
	cf.nFontType = SCREEN_FONTTYPE; 
	cf.nSizeMin = 0; 
	cf.nSizeMax = 0; 
	
	if (ChooseFont(&cf)==TRUE)
	{
		color = cf.rgbColors;
		return true;
	}
	else
	{
		return false;
	}
}

bool	ShowColorDialog(HWND hWnd, COLORREF &cr)
{
	CHOOSECOLOR cc;
	COLORREF crCustColors[16];
	cc.lStructSize=sizeof(CHOOSECOLOR);
	cc.hwndOwner=hWnd;
	cc.hInstance=NULL;
	cc.rgbResult=cr;
	cc.lpCustColors =crCustColors;
	cc.Flags=CC_RGBINIT|CC_FULLOPEN|CC_ANYCOLOR;
	cc.lCustData=0;
	cc.lpfnHook=NULL;
	cc.lpTemplateName=NULL;
	BOOL ret=ChooseColor(&cc);
	if(ret)cr=cc.rgbResult;
	return ret==TRUE;
}

void	SetupDataForCopyright(HWND hWnd, CXYPlotImpl *plot)
{
	tstring str;
	str=CGlobal::GetVersion();
	SendDlgItemMessage(hWnd, IDC_PS_VERSION, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
	str=CGlobal::GetAuthorInfo();
	SendDlgItemMessage(hWnd, IDC_PS_AUTHOR, WM_SETTEXT, 0, (LPARAM)str.c_str()); 
}
void	SaveDataForCopyright(HWND hWnd, CXYPlotImpl *plot)
{
	return;
}
BOOL	CALLBACK PlotCopyrightDlgFunc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static CXYPlotImpl	*plot;
	
	// local varibles
	
	LPNMHDR pnmh;
	
	switch(message)
	{
	case WM_INITDIALOG:
		plot = (CXYPlotImpl *)((LPPROPSHEETPAGE)lparam)->lParam;
		SetupDataForCopyright(hWnd, plot);
		
		return TRUE;
		
	case WM_NOTIFY:
		pnmh = (LPNMHDR) lparam;
		switch(pnmh->code)
		{
		case PSN_APPLY:
			// Apply Changes
			SaveDataForCopyright(hWnd, plot);
		}
		break;
	}
	return FALSE;
}