#ifndef LISTEX_H
#define LISTEX_H
#pragma once

namespace DuiLib
{
	class UILIB_API CListUIEx : public CListUI
	{
	public:
		CListUIEx();
		virtual ~CListUIEx();

		/**
		* ������,�վ�,��0��ʼ
		* 
		* @param	nCol					����,��Ҫ��0��ʼ���β���
		* @param	lpszColumnHeaderStr		ÿһ�б���
		* @param	uTextStyle				�������ֶ��뷽ʽ
		* @param	nWidth					�п�
		* 
		* @return	����TrueΪ�ɹ�,FalseΪʧ��
		*/
		BOOL InsertColumn(int nCol, LPCTSTR lpszColumnHeaderStr, UINT uTextStyle = DT_CENTER, int nWidth = -1);

		BOOL InsertColumn( int nCol, DuiLib::CListHeaderItemUI *pHeaderItem );

		/**
		* ������,�վ�,��0��ʼ
		* 
		* @param	nItem					����,��Ҫ��0��ʼ���β���
		*
		* @return	�ɹ��򷵻�����,���򷵻�-1
		*/
		int InsertItem(int nItem, int nHeight = 35);

		int InsertItem(int nItem, int nHeight, DuiLib::CListContainerElementUI *pListItem);

		void SetItemData(int nItem, int nColumn, LPCTSTR Text, LPCTSTR Name, UINT uTextStyle/* = DT_CENTER*/);
		void SetItemData(int nItem, int nColumn, LPCTSTR Text);
		void SetItemData(int nItem, int nColumn, CControlUI* pControl);



		/**
		* ����������ȡ�пؼ�
		* 
		* @param	iIndex					����,��0���������
		* 
		* @return	�ɹ�ʱ�����ӿؼ���ַ,���򷵻�NULL
		*/
		DuiLib::CListContainerElementUI* GetListItem(int iIndex);

		/**
		* ��ȡ����λ�õĿؼ�
		* 
		* @param	iIndex					����,��0���������
		* @param	iSubIndex					����,��0���������
		* 
		* @return	�ɹ�ʱ�����ӿؼ���ַ,���򷵻�NULL
		*/
		DuiLib::CHorizontalLayoutUI* GetListSubItem(int iIndex, int iSubIndex);

		BOOL SetHeaderItemData(int nColumn, DuiLib::CControlUI* pControl);
	};
}
#endif