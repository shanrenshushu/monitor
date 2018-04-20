#include "stdafx.h"
#include "ListUIEx.h"

using namespace DuiLib;
/************************** CListUIEx Start **************************/
CListUIEx::CListUIEx()
{
}

CListUIEx::~CListUIEx()
{
}

//ͨ�����봴����  
BOOL CListUIEx::InsertColumn(int nCol, LPCTSTR lpszColumnHeaderStr, UINT uTextStyle/* = DT_CENTER*/, int nWidth/* = -1*/)
{
	CListHeaderItemUI *pHeaderItem = new CListHeaderItemUI;
	pHeaderItem->SetTextStyle(uTextStyle|DT_VCENTER|DT_SINGLELINE);
	pHeaderItem->SetText(lpszColumnHeaderStr);

	pHeaderItem->SetAttribute(_T("sepimage"), _T("Headerctrl_Sperator.bmp"));
	pHeaderItem->SetAttribute(_T("sepwidth"), _T("1"));
	pHeaderItem->SetAttribute(_T("pushedimage"), _T("headerctrl_down.bmp"));
	pHeaderItem->SetAttribute(_T("hotimage"), _T("headerctrl_hot.bmp"));
	pHeaderItem->SetAttribute(_T("normalimage"), _T("headerctrl_normal.bmp"));
	pHeaderItem->SetFixedWidth(nWidth);

	return InsertColumn(nCol, pHeaderItem);
}


BOOL CListUIEx::InsertColumn(int nCol, CListHeaderItemUI *pHeaderItem)
{
	CListHeaderUI *pHeader = CListUI::GetHeader();
	if (pHeader == NULL)
	{
		return FALSE;
	}

	if (pHeader->AddAt(pHeaderItem, nCol))
	{		
		return TRUE;
	}

	delete pHeaderItem;
	pHeaderItem = NULL;
	return FALSE;
}

BOOL CListUIEx::SetHeaderItemData(int nColumn, CControlUI* pControl)
{
	CListHeaderUI *pHeader = CListUI::GetHeader();
	if (pHeader == NULL)
	{
		return FALSE;
	}
	CListHeaderItemUI *pHeaderItem = (CListHeaderItemUI *)pHeader->GetItemAt(nColumn);
	pHeaderItem->Add(pControl);
	return TRUE;
}

int CListUIEx::InsertItem(int nItem, int nHeight)
{
	CListContainerElementUI *pListItem = new CListContainerElementUI;
	pListItem->SetFixedHeight(nHeight);		/*�̶�һ���и�*/

	pListItem->m_pHeader = CListUI::GetHeader();
	if (NULL != pListItem->m_pHeader)
	{
		int nHeaderCount = pListItem->m_pHeader->GetCount();
		for (int i = 0; i < nHeaderCount; i++)
		{
			pListItem->Add(new CHorizontalLayoutUI);
		}
	}
	if ( !CListUI::AddAt(pListItem, nItem) )
	{
		delete pListItem;
		pListItem = NULL;
		return -1;
	}
	return nItem;
}


int CListUIEx::InsertItem(int nItem, int nHeight, CListContainerElementUI *pListItem)
{
	pListItem->SetFixedHeight(nHeight);	
	pListItem->m_pHeader =  CListUI::GetHeader();
	if ( !CListUI::AddAt(pListItem, nItem) )
	{
		delete pListItem;
		pListItem = NULL;
		return -1;
	}

	return nItem;
}

void CListUIEx::SetItemData(int nItem, int nColumn, LPCTSTR Text, LPCTSTR Name, UINT uTextStyle/* = DT_CENTER*/)
{
	CLabelUI *pLabel = new CLabelUI;
	pLabel->SetText(Text);		//�ؼ����Ծ͸����������ð�,�Ҽ�����һ��
	pLabel->SetTextStyle(uTextStyle|DT_VCENTER|DT_SINGLELINE);
	pLabel->SetAttribute(_T("endellipsis"), _T("true"));
	pLabel->SetName(Name);
	pLabel->SetAttribute(_T("bordercolor"), _T("#FFCCCCCC"));
	pLabel->SetAttribute(_T("bordersize"), _T("1,0,0,2"));
	pLabel->SetAttribute(_T("font"), _T("5"));
	SetItemData(nItem, nColumn, pLabel);		//��ӵ����ؼ�
}

void CListUIEx::SetItemData(int nItem, int nColumn, LPCTSTR Text)
{
	CHorizontalLayoutUI *pSubItem = GetListSubItem(nItem, nColumn);
	if(pSubItem)
	{
		static_cast<CContainerUI*>(pSubItem)->GetItemAt(0)->SetText(Text);
	}
}

void CListUIEx::SetItemData(int nItem, int nColumn,	CControlUI* pControl)
{
	CHorizontalLayoutUI *pSubHor = GetListSubItem(nItem, nColumn);
	//pSubHor->SetAttribute(_T("inset"), _T("3,1,3,1"));
	pSubHor->Add(pControl);		//��ӵ����ؼ�
}

CListContainerElementUI* CListUIEx::GetListItem(int iIndex)
{
	return static_cast<CListContainerElementUI*>(CListUI::GetItemAt(iIndex));
}

CHorizontalLayoutUI* CListUIEx::GetListSubItem(int iIndex, int iSubIndex)
{
	//��ȡ�����пؼ�
	CListContainerElementUI *pListItem = static_cast<CListContainerElementUI*>(CListUI::GetItemAt(iIndex));
	if (pListItem == NULL)
		return NULL;
	return static_cast<CHorizontalLayoutUI*>(pListItem->GetItemAt(iSubIndex));
}
