#pragma once
#include "stdafx.h"
#include "../Cchart/Plot/PlotInterface.h"

typedef void (*FUN_PAINTVIDEO)(HDC,RECT);

class CCharTUI: public CControlUI
{  
public:  
	CCharTUI();
	~CCharTUI();
	virtual void DoPaint(HDC hDC, const RECT& rcPaint);

	enum
	{
		VIDEO_TIMER_ID = 25
	};
	void SetTime();
	void PaintVideo(HDC hDC);
	void SetFun(FUN_PAINTVIDEO fun);
protected:   
	FUN_PAINTVIDEO m_funPaintVideo;
};  