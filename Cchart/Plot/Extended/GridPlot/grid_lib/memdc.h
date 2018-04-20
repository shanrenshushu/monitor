#ifndef _CMEMDC_H
#define _CMEMDC_H

#define _WINSOCKAPI_
#include <windows.h>

class CMemDC
{
public:
	CMemDC(HDC hdc, HBRUSH hBkgnd = NULL);
	virtual ~CMemDC();

	void			SetBkgBrush(HBRUSH hBrush);

	HDC				m_hdc;
protected:
	RECT			m_rc;
	HDC				m_hOriginDC;
	HBITMAP			m_bmp;
	HBRUSH			m_hBkgnd;
};

#endif