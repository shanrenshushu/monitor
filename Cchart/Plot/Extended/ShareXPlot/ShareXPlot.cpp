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
#include "ShareXLayerImpl.h"
#include "ShareXPlot.h"

CShareXPlot::CShareXPlot()
{
	SetDefaults();
}

CShareXPlot::~CShareXPlot()
{

}

void	CShareXPlot::SetDefaults()
{

}

void	CShareXPlot::CopySettings(const CShareXPlot *plot)
{

}

CShareXLayerImplI*		CShareXPlot::GetLayer(int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vpLayers.size())return NULL;
	return m_vpLayers[nIndex];
}

int		CShareXPlot::AddLayer()
{
	CShareXLayerImplI *tmp = new CShareXLayerImplI(this);
	m_vpLayers.push_back(tmp);	
	return m_vpLayers.size();
}

void	CShareXPlot::ClearLayers(int nIndex)
{
	if(nIndex < 0 || nIndex >= GetLayersCount() )return;
	if(m_vpLayers[nIndex])delete m_vpLayers[nIndex];
	m_vpLayers.erase(m_vpLayers.begin()+nIndex);
}

void	CShareXPlot::ClearLayers()
{
	for(int i=m_vpLayers.size()-1; i>=0; i--)
	{
		if(m_vpLayers[i])delete m_vpLayers[i];
		m_vpLayers.pop_back();
	}
}

int		CShareXPlot::SetLayersCount( int num )
{
	ClearLayers();
	if(num > 0)
	{
		m_vpLayers.resize(num);
		for(int i=0; i<(int)m_vpLayers.size(); i++)
		{
			m_vpLayers[i] = new CShareXLayerImplI(this);
		}
	}
	return m_vpLayers.size();
}
