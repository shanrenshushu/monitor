#ifndef _COLORCONTROL_H
#define _COLORCONTROL_H

#include <windows.h>

#define WM_GETCOLOR WM_USER+500
#define WM_SETCOLOR WM_USER+501

ATOM RegisterColorControl();

#endif