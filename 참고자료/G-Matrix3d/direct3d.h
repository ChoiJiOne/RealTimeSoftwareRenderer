/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <d3dx.h>

extern BOOL bD3DXReady;
extern BOOL bActive;

extern ID3DXContext* pD3DX;

void InterpretD3DError(HRESULT hr);
HRESULT SetupDirect3D(HWND hWnd);
HRESULT CloseDirect3D(void);
HRESULT UpdateFrameD3D(void);

#endif
