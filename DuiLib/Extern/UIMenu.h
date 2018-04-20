#ifndef __UIMENU_H__
#define __UIMENU_H__

#ifdef _MSC_VER
#pragma once
#endif
#include "UILib.h"
#include "observer_impl_base.hpp"
#define WM_MENUSELECTMSG   WM_USER+0x500

namespace DuiLib {

    /////////////////////////////////////////////////////////////////////////////////////
    //
    struct ContextMenuParam {
        // 1: remove all
        // 2: remove the sub menu
        WPARAM wParam;
        HWND hWnd;
    };

    enum MenuAlignment {
        eMenuAlignment_Left = 1 << 1,
        eMenuAlignment_Top = 1 << 2,
        eMenuAlignment_Right = 1 << 3,
        eMenuAlignment_Bottom = 1 << 4,
    };

    typedef class ObserverImpl<BOOL, ContextMenuParam> ContextMenuObserver;
    typedef class ReceiverImpl<BOOL, ContextMenuParam> ContextMenuReceiver;

    extern ContextMenuObserver s_context_menu_observer;

    // MenuUI
    extern const TCHAR *const kMenuUIClassName;// = _T("MenuUI");
    extern const TCHAR *const kMenuUIInterfaceName;// = _T("Menu");

    class CListUI;
    class UILIB_API CMenuUI : public DuiLib::CListUI {
    public:
        CMenuUI();
        ~CMenuUI();

        CDuiString GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);

        virtual void DoEvent(DuiLib::TEventUI &event);

        virtual bool Add(DuiLib::CControlUI *pControl);
        virtual bool AddAt(DuiLib::CControlUI *pControl, int iIndex);

        virtual int GetItemIndex(DuiLib::CControlUI *pControl) const;
        virtual bool SetItemIndex(DuiLib::CControlUI *pControl, int iIndex);
        virtual bool Remove(DuiLib::CControlUI *pControl);
        virtual bool SelectItemActivate(int iIndex);    // Ë«»÷Ñ¡ÖÐ

        SIZE EstimateSize(SIZE szAvailable);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    };

    /////////////////////////////////////////////////////////////////////////////////////
    //

    // MenuElementUI
    extern const TCHAR *const kMenuElementUIClassName;// = _T("MenuElementUI");
    extern const TCHAR *const kMenuElementUIInterfaceName;// = _T("MenuElement);

    class CMenuElementUI;
    class CMenuWnd : public DuiLib::CWindowWnd, public ContextMenuReceiver {
    public:
        CMenuWnd(HWND hParent = NULL);
        void Init(DuiLib::CMenuElementUI *pOwner, LPCTSTR xml, LPCTSTR pSkinType, CDuiRect rc);
        LPCTSTR GetWindowClassName() const;
        void OnFinalMessage(HWND hWnd);

        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

        BOOL Receive(ContextMenuParam param);

        bool InitMenuElem(CMenuUI *pOwner, STRINGorID xml, LPCTSTR pSkinType, int count);

    public:
        HWND m_hParent;
        POINT m_BasedPoint;
        //STRINGorID m_xml;
		CDuiString m_xml;
        CDuiString m_sType;
        CPaintManagerUI m_pm;
        CMenuElementUI *m_pOwner;
        CMenuUI *m_pLayout;

    private:
        void resize(CControlUI *pRoot);

    };

    class CListContainerElementUI;
    class CMenuElementUI : public DuiLib::CListContainerElementUI {
        friend CMenuWnd;
    public:
        CMenuElementUI();
        ~CMenuElementUI();

        CDuiString GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);

        void DoPaint(HDC hDC, const RECT &rcPaint);

        void DrawItemText(HDC hDC, const RECT &rcItem);

        SIZE EstimateSize(SIZE szAvailable);

        bool Activate();

        void DoEvent(TEventUI &event);


        CMenuWnd *GetMenuWnd();

        void CreateMenuWnd();

    protected:
        CMenuWnd *m_pWindow;
    };

} // namespace DuiLib

#endif // __UIMENU_H__
