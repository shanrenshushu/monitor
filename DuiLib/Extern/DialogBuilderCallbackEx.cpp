#include "StdAfx.h"
#include "DialogBuilderCallbackEx.h"

using namespace DuiLib;

CDialogBuilderCallbackEx::CDialogBuilderCallbackEx(CPaintManagerUI *ppm)
{
    m_pm = ppm;
}


CDialogBuilderCallbackEx::~CDialogBuilderCallbackEx(void) 
{

}

CControlUI *CDialogBuilderCallbackEx::CreateControl( LPCTSTR pstrClass ) 
{
    return NULL;
}
