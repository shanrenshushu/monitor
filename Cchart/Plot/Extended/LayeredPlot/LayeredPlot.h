/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2012 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */

#ifndef __LAYEREDPLOT_H_122333444455555__
#define __LAYEREDPLOT_H_122333444455555__

#include "PlotLayerImpl.h"

#pragma warning(disable:4786)

#include <vector>
using namespace std;

class CLayeredPlot
{
public:
	CLayeredPlot();
	virtual ~CLayeredPlot();
	
	void		SetDefaults();
	void		CopySettings(const CLayeredPlot *plot);
	
	CPlotLayerImplI*		GetLayer(int nIndex);

public:
	void		ClearLayers(int nIndex);
	void		ClearLayers();
	int			GetLayersCount(){return m_vpLayers.size();}

	int			AddLayer();
	
	void		SetTop(int index, bool bTop);
	int			GetTop();
	void		UntopAll();
	
protected:
	vector<CPlotLayerImplI*>	m_vpLayers;
};

#endif