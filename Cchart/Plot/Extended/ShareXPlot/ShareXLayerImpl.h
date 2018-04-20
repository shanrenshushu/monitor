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

#ifndef __SHAREXLAYERIMPL_H_122333444455555__
#define __SHAREXLAYERIMPL_H_122333444455555__

#include "../../Basics/XYPlotImpl.h"
#include "../../Basics/XYHandler.h"
#include "ShareXLayer.h"

class CShareXLayerImpl : public CShareXLayer, public CXYPlotImpl
{
public:
	CShareXLayerImpl(CShareXPlot *pParent);
	virtual ~CShareXLayerImpl();

	CShareXLayerImpl& operator=(const CShareXLayerImpl& rhs);

	//bool		IsEmpty(){return m_vXYDataInfo.size()<=0;}
	bool		NeedDrawing(){return !IsEmpty();}

	SIZE		GetAxisSize( HDC hDC, int location );
	void		OnDraw( HDC hDC, RECT destRect );
	void		OnDraw( HWND hWnd );

	void		DataToLP( HDC hDC, double *data, LPPOINT point );
	void		LPToData( HDC hDC, LPPOINT point, double *data );

protected:
	//void		SwitchSelection();
	//void		SwitchSelAxes();
	//void		SwitchSelDatasets();
	//void		SwitchSelTitle();
};

class CShareXLayerImplI : public CShareXLayerImpl, public CXYHandler<CShareXLayerImplI>
{
public:
	CShareXLayerImplI::CShareXLayerImplI(CShareXPlot *pParent):CShareXLayerImpl(pParent){};
};

#endif