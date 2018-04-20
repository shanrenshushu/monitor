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

#ifndef __NLFIT_32167_H__
#define __NLFIT_32167_H__

#include <math.h>

double sign(double val);
//��Gauss-Jordan��ȥ�������Է�����
void gaussj(double *a,int n,double *b,int &flag);
//�в�ƽ���ͼ���
double sum(double *pA,int cnum,double *pX,double *pY,int len,double *pErr);
//���������������
void nlfit(double *pA,int cnum,double *pX,double *pY,int len,double err,int iter,int method,int &fail);

//�в�������
double remain(double *pA,int cnum,double *pX,double *pY,int no);
//ƫ�����������
void derivative(double *pA,double *pDiff,int cnum,double *pX,int no);

#endif