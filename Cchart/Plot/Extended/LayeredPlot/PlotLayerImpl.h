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

#ifndef __PLOTLAYERIMPL_H_122333444455555__
#define __PLOTLAYERIMPL_H_122333444455555__

#include "../../Basics/XYPlotImpl.h"
#include "../../Basics/XYHandler.h"
#include "PlotLayer.h"

class CPlotLayerImpl : public CPlotLayer, public CXYPlotImpl
{
public:
	CPlotLayerImpl(CLayeredPlot *pParent);
	virtual ~CPlotLayerImpl();

	CPlotLayerImpl& operator=(const CPlotLayerImpl& rhs);
	
	int				RegionIdentify( HDC hDC, POINT point );
	bool			NeedDrawing(){return !IsEmpty();}

	SIZE			GetAxisSize( HDC hDC, int location );
	void			OnDraw( HDC hDC, RECT destRect );
	void			OnDraw( HWND hWnd );

	void			DataToLP( HDC hDC, double *data, LPPOINT point );
	void			LPToData( HDC hDC, LPPOINT point, double *data );
private:
	RECT			GetPseudoPlotRect( HDC hDC, RECT clientRect );
};

class CPlotLayerImplI : public CPlotLayerImpl, public CXYHandler<CPlotLayerImplI>
{
public:
	CPlotLayerImplI(CLayeredPlot *pParent):CPlotLayerImpl(pParent){};
};

#endif