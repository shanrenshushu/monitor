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
#include "PlotLayer.h"

CPlotLayer::CPlotLayer(CLayeredPlot *pParent): m_pParent(pParent)
{
	SetDefaults();
}

CPlotLayer::~CPlotLayer()
{
	
}

CPlotLayer& CPlotLayer::operator=(const CPlotLayer& rhs)
{
	CopySettings(&rhs);
	return *this;
}

void	CPlotLayer::SetDefaults()
{
	m_bTop = false;
}

void	CPlotLayer::CopySettings(const CPlotLayer *plot)
{
	
}

