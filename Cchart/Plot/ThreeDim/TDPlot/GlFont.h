#ifndef	__GLFONT_32167_H__
#define	__GLFONT_32167_H__

#include <tchar.h>

class CGlFont                                                            
{
protected:
	HFONT	m_hFont;
	float	m_fExtrusion;
public:
	CGlFont();
	~CGlFont();

	void	DeleteFont();
	bool	InitFont(const TCHAR *fontName);
	bool	InitFont(LOGFONT &lf);

	HFONT	GetFontHandle(){return m_hFont;}
	LOGFONT	GetLogFont();

	// Get font bitmap from gdi,then draw in the scene
	void	Draw2DTextByGdi(const TCHAR *string, float x = 0.0, float y = 0.0);
	// Get font bitmap use wglUseFontBitmaps, then draw in the scene
	void	Draw2DText(const TCHAR *string, float x = 0.0, float y = 0.0);
	// Use wglUseFontOutlines,return the length of the string
	float	Draw3DText(const TCHAR *string);

	float	Get3DLength(const TCHAR *string);
	float	GetExtrusion() {return m_fExtrusion;}
	void	SetExtrusion(float extrusion) {m_fExtrusion = extrusion;}
};

#endif    // __GLFONT_32167_H__ 
