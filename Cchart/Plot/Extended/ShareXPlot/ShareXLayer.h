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

#ifndef __SHAREXLAYER_H_122333444455555__
#define __SHAREXLAYER_H_122333444455555__

#include <windows.h>

class CShareXPlot;

class CShareXLayer
{
protected:
	RECT		m_LastPlotRect;

	CShareXPlot	*m_pParent;
public:
	CShareXLayer(CShareXPlot *pParent);
	virtual ~CShareXLayer();
	virtual CShareXLayer& operator=(const CShareXLayer& rhs);

	void		SetDefaults();
	void		CopySettings(const CShareXLayer *plot);
};


#endif