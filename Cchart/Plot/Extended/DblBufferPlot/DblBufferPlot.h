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

#ifndef __DBLBUFFERPLOT_H_122333444455555__
#define __DBLBUFFERPLOT_H_122333444455555__

#include "../../Basics/XYPlotImpl.h"

// class CPlot is a basement class
class CDblBufferPlot
{
public:
	CDblBufferPlot();
	virtual ~CDblBufferPlot();

	void			SetDefaults( );
	void			CopySettings( const CDblBufferPlot *plot );

protected:
	HWND	m_hWnd;

	//virtual window parameters
	HDC		m_hMemDC;
	int		m_maxX,m_maxY;

protected:
	virtual	void	CreateVirtualWindow();
	void			ReleaseVirtualWindow();

	bool			m_bRectConfinedClient;
	RECT			m_rectConfinedClient;
	bool			m_bForCtrl;
public:
	bool			IsClientConfined() { return m_bRectConfinedClient; }
	RECT			GetConfinedClient() { return m_rectConfinedClient; }
	void			SetForCtrl(bool bSet) { m_bForCtrl = bSet; }

};

#endif