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
#include "PlotData.h"
#include "../Accessary/MyString/MyString.h"

void GetDataPoint(tstring aline, double &result)
{
	tistringstream istr(aline);
	istr>>result;
}

void GetDataPoint(tstring aline, DataPoint2D &result)
{
	tistringstream istr(aline);
	istr>>result.x>>result.y;
}

void GetDataPoint(tstring aline, DataPoint3D &result)
{
	tistringstream istr(aline);
	istr>>result.x>>result.y>>result.z;
}

void GetDataLine(double datapoint, tstring &result)
{
	tostringstream ostr;
	ostr<<datapoint<<_TEXT("\n");
	result = ostr.str();
}

void GetDataLine(DataPoint2D datapoint, tstring &result)
{
	tostringstream ostr;
	ostr<<datapoint.x<<_TEXT("\t")<<datapoint.y<<_TEXT("\n");
	result = ostr.str();
}
void GetDataLine(DataPoint3D datapoint, tstring &result)
{
	tostringstream ostr;
	ostr<<datapoint.x<<_TEXT("\t")<<datapoint.y<<_TEXT("\t")<<datapoint.z<<_TEXT("\n");
	result = ostr.str();
}