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
#include "PlotLayerImpl.h"
#include "LayeredPlot.h"

CLayeredPlot::CLayeredPlot()
{
	SetDefaults();
}

CLayeredPlot::~CLayeredPlot()
{

}

void	CLayeredPlot::SetDefaults()
{

}

void	CLayeredPlot::CopySettings(const CLayeredPlot *plot)
{

}

CPlotLayerImplI*	CLayeredPlot::GetLayer(int nIndex)
{
	if(nIndex<0 || nIndex>=(int)m_vpLayers.size())return NULL;
	return m_vpLayers[nIndex];
}


void	CLayeredPlot::ClearLayers(int nIndex)
{
	if(nIndex < 0 || nIndex >= GetLayersCount() )return;
	if(m_vpLayers[nIndex])delete m_vpLayers[nIndex];
	m_vpLayers.erase(m_vpLayers.begin()+nIndex);
	for(int i=nIndex; i<GetLayersCount(); i++)
	{
		GetLayer(i)->GetLayerIndex()--;
	}
}

void	CLayeredPlot::ClearLayers()
{
	for(int i=m_vpLayers.size()-1; i>=0; i--)
	{
		if(m_vpLayers[i])delete m_vpLayers[i];
		m_vpLayers.pop_back();
	}
}

int		CLayeredPlot::AddLayer()
{
	CPlotLayerImplI *tmp = new CPlotLayerImplI(this);
	tmp->GetLayerIndex() = (int)m_vpLayers.size();
	m_vpLayers.push_back(tmp);	
	SetTop((int)m_vpLayers.size()-1, true);
	return m_vpLayers.size();
}

void	CLayeredPlot::SetTop(int index, bool bTop)
{
	int now;
	now= GetTop();
	if(index == now)
	{
		return;
	}
	else if(index >= 0 && index < GetLayersCount())
	{
		if(now >= 0 && now < GetLayersCount())m_vpLayers[now]->SetTop(false);
		m_vpLayers[index]->SetTop(bTop);
	}
}

int		CLayeredPlot::GetTop()
{
	for(int i=0; i<GetLayersCount(); i++)
	{
		if(m_vpLayers[i]->IsTop())return i;
	}
	return -1;
}

void	CLayeredPlot::UntopAll()
{
	for(int i=0; i<GetLayersCount(); i++)
	{
		m_vpLayers[i]->SetTop(false);
	}
}