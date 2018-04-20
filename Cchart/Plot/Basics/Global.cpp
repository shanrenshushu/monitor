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

#include "Global.h"


/////////////////////////////////////////////////////////////////////////
// Class CGlobal static members

bool	CGlobal::m_bNewInface = true;

bool	CGlobal::m_bMenuTitles = true;
bool	CGlobal::m_bMenuAxes = true;
bool	CGlobal::m_bMenuBackground = true;
bool	CGlobal::m_bMenuLegend = true;
bool	CGlobal::m_bMenuDatasets = true;

bool	CGlobal::m_bInDll = false;
tstring	CGlobal::m_strDllName = _TEXT("PlotDll.dll");
tstring	CGlobal::m_strFileSeparator = _TEXT("//##//\r\n");
/////////////////////////////////////////////////////////////////////////
// Class CGlobal static members

HMODULE	CGlobal::MyGetModuleHandle()
{
	if(!m_bInDll)
	{
		return GetModuleHandle(NULL);
	}
	else
	{
		return GetModuleHandle(m_strDllName.c_str());
	}
}