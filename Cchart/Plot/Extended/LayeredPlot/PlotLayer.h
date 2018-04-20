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

#ifndef __PLOTLAYER_H_122333444455555__
#define __PLOTLAYER_H_122333444455555__

#include <windows.h>

class CLayeredPlot;

class CPlotLayer
{
protected:
	RECT m_LastPlotRect;
public:
	CPlotLayer(CLayeredPlot *pParent);
	virtual ~CPlotLayer();
	virtual CPlotLayer& operator=(const CPlotLayer& rhs);

	void		SetDefaults();
	void		CopySettings(const CPlotLayer *plot);

protected:
	CLayeredPlot	*m_pParent;
	int				m_nLayerIndex;
	bool			m_bTop;

public:
	inline	int		&GetLayerIndex(){return m_nLayerIndex;}
	inline	bool	IsTop(){return m_bTop;}
	inline	void	SetTop(bool top){m_bTop = top;}
};


#endif