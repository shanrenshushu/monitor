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

#ifndef __SHAREXPLOT_H_122333444455555__
#define __SHAREXPLOT_H_122333444455555__

class CShareXLayerImplI;

class CShareXPlot
{
public:
	CShareXPlot();
	virtual ~CShareXPlot();

	void		SetDefaults();
	void		CopySettings(const CShareXPlot *plot);
	
	CShareXLayerImplI*		GetLayer(int nIndex);

	int			AddLayer();
	
	void		ClearLayers(int nIndex);
	void		ClearLayers();

	int			GetLayersCount(){return m_vpLayers.size();}
	int			SetLayersCount( int num );

	void		DeleteAllData( void ){ClearLayers();}

protected:
	vector<CShareXLayerImplI*>	m_vpLayers;
};


#endif