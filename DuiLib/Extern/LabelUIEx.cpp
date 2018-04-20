#include "stdafx.h"
#include "LabelUIEx.h"

namespace DuiLib
{
	CLabelUIEx::CLabelUIEx(void)
	{
	}


	CLabelUIEx::~CLabelUIEx(void)
	{
	}
	Color CLabelUIEx::_MakeRGB(int a, Color cl)  
	{  
		return Color(a, cl.GetR(), cl.GetG(), cl.GetB());  
	}  

	Color CLabelUIEx::_MakeRGB(int r, int g, int b)  
	{  
		return Color(255, r, g, b);  
	}  

	void CLabelUIEx::SetPos(RECT rc)  
	{  
		__super::SetPos(rc);  
	}  

	void CLabelUIEx::PaintText(HDC hDC)  
	{  
		UINT uStyle = GetTextStyle();
		uStyle |= DT_WORDBREAK;
		uStyle |= DT_CALCRECT;
		uStyle |= DT_NOCLIP;
		uStyle |= DT_EXTERNALLEADING;
		if (uStyle & DT_CENTER)
			uStyle &= (~DT_CENTER);
		m_uTextStyle = uStyle;
		if( m_dwTextColor == 0 )
			m_dwTextColor = m_pManager->GetDefaultFontColor();  
		if( m_dwDisabledTextColor == 0 )
			m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();  

		RECT rc = m_rcItem;  
		rc.left += m_rcTextPadding.left;  
		rc.right -= m_rcTextPadding.right;  
		rc.top += m_rcTextPadding.top;  
		rc.bottom -= m_rcTextPadding.bottom;  

		if(!GetEnabledEffect())  
		{  
			if( m_sText.IsEmpty() ) return;  
			int nLinks = 0;  
			if( IsEnabled() )
			{  
				if( m_bShowHtml )
				{
					CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwTextColor,NULL, NULL, nLinks, m_uTextStyle);
				}
				else
				{
					CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwTextColor,m_iFont,m_uTextStyle);
				}
			}  
			else 
			{  
				if( m_bShowHtml ) 
				{
					CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor,NULL, NULL, nLinks, m_uTextStyle);
				}	  
				else
				{
					CRenderEngine::DrawText(hDC, m_pManager, rc, m_sText, m_dwDisabledTextColor,m_iFont,m_uTextStyle);
				}
			}  
		}  
		else  
		{  
			Font    nFont(hDC,m_pManager->GetFont(GetFont()));  

			Graphics nGraphics(hDC);  
			nGraphics.SetTextRenderingHint(m_TextRenderingHintAntiAlias);  

			StringFormat format;  
			format.SetAlignment((StringAlignment)m_hAlign);  
			format.SetLineAlignment((StringAlignment)m_vAlign);  

			RectF nRc((float)rc.left,(float)rc.top,(float)rc.right-rc.left,(float)rc.bottom-rc.top);  
			RectF nShadowRc = nRc;  
			nShadowRc.X += m_ShadowOffset.X;  
			nShadowRc.Y += m_ShadowOffset.Y;  

			int nGradientLength = GetGradientLength();  

			if(nGradientLength == 0)  
				nGradientLength = (rc.bottom-rc.top);  

			LinearGradientBrush nLineGrBrushA(Point(GetGradientAngle(), 0),Point(0,nGradientLength),_MakeRGB(GetTransShadow(),GetTextShadowColorA()),_MakeRGB(GetTransShadow1(),GetTextShadowColorB() == -1?GetTextShadowColorA():GetTextShadowColorB()));  
			LinearGradientBrush nLineGrBrushB(Point(GetGradientAngle(), 0),Point(0,nGradientLength),_MakeRGB(GetTransText(),GetTextColor()),_MakeRGB(GetTransText1(),GetTextColor1() == -1?GetTextColor():GetTextColor1()));  

			if(GetEnabledStroke() && GetStrokeColor() > 0)  
			{  
				LinearGradientBrush nLineGrBrushStroke(Point(GetGradientAngle(),0),Point(0,rc.bottom-rc.top+2),_MakeRGB(GetTransStroke(),GetStrokeColor()),_MakeRGB(GetTransStroke(),GetStrokeColor()));  

#ifdef _UNICODE  
				nRc.Offset(-1,0);  
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushStroke);  
				nRc.Offset(2,0);  
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushStroke);  
				nRc.Offset(-1,-1);  
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushStroke);  
				nRc.Offset(0,2);  
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushStroke);  
				nRc.Offset(0,-1);  
#else  
				USES_CONVERSION;  
				wstring mTextValue = A2W(m_TextValue.GetData());  

				nRc.Offset(-1,0);  
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushStroke);  
				nRc.Offset(2,0);  
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushStroke);  
				nRc.Offset(-1,-1);  
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushStroke);  
				nRc.Offset(0,2);  
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushStroke);  
				nRc.Offset(0,-1);  
#endif  

			}  
#ifdef _UNICODE  
			if(GetEnabledShadow() && (GetTextShadowColorA() > 0 || GetTextShadowColorB() > 0))  
				nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nShadowRc,&format,&nLineGrBrushA);  

			nGraphics.DrawString(m_TextValue,m_TextValue.GetLength(),&nFont,nRc,&format,&nLineGrBrushB);  
#else  
			USES_CONVERSION;  
			wstring mTextValue = A2W(m_TextValue.GetData());  

			if(GetEnabledShadow() && (GetTextShadowColorA() > 0 || GetTextShadowColorB() > 0))  
				nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nShadowRc,&format,&nLineGrBrushA);  

			nGraphics.DrawString(mTextValue.c_str(),mTextValue.length(),&nFont,nRc,&format,&nLineGrBrushB);  
#endif  

		}  
	}

	void CLabelUIEx::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETCURSOR )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		else if( event.Type == UIEVENT_BUTTONUP )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) )
			{
				if( m_pManager != NULL )
					m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
			}
			return;
		}
		CLabelUI::DoEvent(event);
	}

	UINT CLabelUIEx::GetControlFlags() const
	{
		if( IsEnabled() )
			return UIFLAG_SETCURSOR;
		return 0;
	}
}
