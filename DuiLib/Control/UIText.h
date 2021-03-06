#ifndef __UITEXT_H__
#define __UITEXT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CTextUI : public CLabelUI
	{
	public:
		CTextUI();
		~CTextUI();

		CDuiString GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		CDuiString* GetLinkContent(int iIndex);

		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);

		void PaintText(HDC hDC);

	protected:
		//enum { MAX_LINK = 8 };
		enum { MAX_LINK = 15 };
		int m_nLinks;
		RECT m_rcLinks[MAX_LINK];
		CDuiString m_sLinks[MAX_LINK];
		int m_nHoverLink;
	};

} // namespace DuiLib

#endif //__UITEXT_H__