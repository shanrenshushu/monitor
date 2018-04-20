#include "gridcell.h"
#include "../../../Accessary/MyString/MyString.h"

CGridCell::CGridCell()
{
}

CGridCell::~CGridCell()
{
}

void CGridCell::Draw(HDC hdc, RECT& rc, int x, int y, DWORD dwTick)
{
	DrawVariant(hdc, rc);
}

void CGridCell::DrawVariant(HDC hdc, RECT& rc)
{
	//TCHAR sz[50];
	tostringstream ostr;
	tstring sz;
	switch (GetType())
	{
	case MYVT_NONE:
		break;
	case MYVT_BOOL:
		/*		if(vt.vt_bool)
						DrawText(hdc, _TEXT("Yes"), 3, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					else
						DrawText(hdc, _TEXT("No"), 2, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/
		CGridCheckBox::Draw(hdc, rc, vt.vt_bool);
		break; 
	case MYVT_CHAR:
		ostr<<vt.vt_char;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%d"), vt.vt_char);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_UCHAR:
		ostr<<vt.vt_uchar;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%d"), vt.vt_uchar);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_SHORT:
		ostr<<vt.vt_short;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%d"), vt.vt_short);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_USHORT:
		ostr<<vt.vt_ushort;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%d"), vt.vt_ushort);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_INT:
		ostr<<vt.vt_int;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%d"), vt.vt_int);
		//DrawText(hdc, sz, _tcslen(sz), &rc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_UINT:
		ostr<<vt.vt_uint;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%d"), vt.vt_uint);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_LONG:
		ostr<<vt.vt_long;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%d"), vt.vt_long);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_ULONG:
		ostr<<vt.vt_ulong;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%d"), vt.vt_ulong);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_FLOAT:
		ostr<<vt.vt_float;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%.3f"), vt.vt_float);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_DOUBLE:
		ostr<<vt.vt_double;
		sz = ostr.str();
		DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//_stprintf(sz, _TEXT("%.3f"), vt.vt_double);
		//DrawText(hdc, sz, _tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_PTCHAR:
		DrawText(hdc, vt.vt_ptchar, _tcslen(vt.vt_ptchar), &rc,	DT_LEFT | DT_VCENTER | DT_WORD_ELLIPSIS);
		break;
	case MYVT_LPARAM:
		break;
	}
}
