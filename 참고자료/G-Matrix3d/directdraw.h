/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#ifndef DIRECTDRAW_H
#define DIRECTDRAW_H

#include <ddraw.h>

// DirectDraw Globals
extern LPDIRECTDRAW4 g_pDD; 
extern LPDIRECTDRAWSURFACE4 g_pDDSPrimary;
extern LONG g_lPitch;

HRESULT SetupDirectDraw(HWND hWnd);
void ReleaseDDObjects(void);

#endif