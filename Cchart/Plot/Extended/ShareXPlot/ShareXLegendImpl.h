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

#ifndef __SHAREXLEGENDIMPL_H_122333444455555__
#define __SHAREXLEGENDIMPL_H_122333444455555__

#include "../../Basics/LegendImpl.h"
#include "ShareXLegend.h"
class CShareXPlotImpl;

class CShareXLegendImpl: public CShareXLegend, public CLegendImpl
{
public:
	CShareXLegendImpl(CShareXPlotImpl *pPlot);
	virtual ~CShareXLegendImpl();

	void		OnDraw( HDC hDC, RECT plotRect);
	int			RegionIdentify(HDC hDC,POINT point);
};

#endif