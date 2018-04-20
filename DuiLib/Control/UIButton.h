#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CButtonUI : public CLabelUI
	{
	public:
		CButtonUI();

		CDuiString GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);

		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetPushedImage();
		void SetPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		LPCTSTR GetForeImage();
		void SetForeImage(LPCTSTR pStrImage);
		LPCTSTR GetHotForeImage();
		void SetHotForeImage(LPCTSTR pStrImage);

        void SetHotBkColor(DWORD dwColor);
        DWORD GetHotBkColor() const;
		void SetNormalBkColor(DWORD dwColor);
		DWORD GetNormalBkColor() const;
		void SetPushedBkColor(DWORD dwColor);
		DWORD GetPushedBkColor() const;
		void SetDisabledBkColor(DWORD dwColor);
		DWORD GetDisabledBkColor() const;

		void SetNormalBorderColor(DWORD dwColor);
		DWORD GetNormalBorderColor() const;
		void SetHotBorderColor(DWORD dwColor);
		DWORD GetHotBorderColor() const;
		void SetPushedBorderColor(DWORD dwColor);
		DWORD GetPushedBorderColor() const;
		void SetDisabledBorderColor(DWORD dwColor);
		DWORD GetDisabledBorderColor() const;

        void SetHotTextColor(DWORD dwColor);
        DWORD GetHotTextColor() const;
        void SetPushedTextColor(DWORD dwColor);
        DWORD GetPushedTextColor() const;
        void SetFocusedTextColor(DWORD dwColor);
        DWORD GetFocusedTextColor() const;
        SIZE EstimateSize(SIZE szAvailable);
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(HDC hDC);
		void PaintStatusImage(HDC hDC);
		void PaintBorder(HDC hDC);	//add cwy 2015/5/15
		virtual void PaintBkImage(HDC hDC);

	protected:
		UINT m_uButtonState;

        DWORD m_dwNormalBkColor;
		DWORD m_dwHotBkColor;
		DWORD m_dwPushedBkColor;
		DWORD m_dwDisabledBkColor;

        DWORD m_dwHotTextColor;
        DWORD m_dwPushedTextColor;
        DWORD m_dwFocusedTextColor;


		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sHotForeImage;
		CDuiString m_sPushedImage;
		CDuiString m_sPushedForeImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;

		/* 增加边框颜色属性 added by cwy 2015/5/15 */
		DWORD m_dwNormalBorderColor;
		DWORD m_dwHotBorderColor;
		DWORD m_dwPushedBorderColor;
		DWORD m_dwDisabledBorderColor;

	};

}	// namespace DuiLib

#endif // __UIBUTTON_H__