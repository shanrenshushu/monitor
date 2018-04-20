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

#ifndef __GLOBAL_H_122333444455555__
#define __GLOBAL_H_122333444455555__

#include "../Accessary/MyString/MyString.h"
#include <windows.h>

class CGlobal
{
public:
	// Global information
	static	tstring		GetVersion(){ return _TEXT("CChart Version 2.0.5.1"); }
	static	tstring		GetAuthorInfo(){ return _TEXT("Baita, Phd, vice professor, living in Mianyang, Sichuan, fan of Mr Fang Zhouzi"); }
	// For the context menu control
	static	bool		m_bMenuTitles,m_bMenuAxes,m_bMenuBackground,m_bMenuLegend,m_bMenuDatasets;
	// For the interface style
	static	bool		m_bNewInface;
	
	// For compilation of dll file
	static	bool		m_bInDll;
	static	tstring		m_strDllName;
	static	HMODULE		MyGetModuleHandle();
	
	// For file manipulation
	static	tstring		m_strFileSeparator;
};

#endif
