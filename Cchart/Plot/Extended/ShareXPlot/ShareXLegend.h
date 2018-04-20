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

#ifndef __SHAREXLEGEND_H_122333444455555__
#define __SHAREXLEGEND_H_122333444455555__

class CShareXLegend
{
public:
	CShareXLegend();
	virtual ~CShareXLegend();

	void		SetDefaults();
	void		CopySettings(const CShareXLegend *legend);
};

#endif