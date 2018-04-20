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
#include "ShareXLayer.h"

CShareXLayer::CShareXLayer(CShareXPlot *pParent):m_pParent(pParent)
{
	SetDefaults();
}

CShareXLayer::~CShareXLayer()
{
	
}

CShareXLayer& CShareXLayer::operator=(const CShareXLayer& rhs)
{
	CopySettings(&rhs);
	return *this;
}

void	CShareXLayer::SetDefaults()
{
	
}

void	CShareXLayer::CopySettings(const CShareXLayer *plot)
{
	
}

