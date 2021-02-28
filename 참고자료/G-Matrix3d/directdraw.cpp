/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

// DirectDraw Globals
LPDIRECTDRAW4 g_pDD; 
LPDIRECTDRAWSURFACE4 g_pDDSPrimary;
LONG g_lPitch;

HRESULT SetupDirectDraw(HWND hWnd);
void ReleaseDDObjects(void);

HRESULT SetupDirectDraw(HWND hWnd)
{
	HRESULT ddrval;
	LPDIRECTDRAW lpDD; 
	LPDIRECTDRAW4 lpDD4; 

	ddrval = DirectDrawCreate(NULL, &lpDD, NULL);
	if ( ddrval != DD_OK )
	{
		MessageBox(NULL, "Create DirectDraw Failed!", "DirectDraw Error", MB_OK | MB_TOPMOST);
		return ddrval;
	}
	
	ddrval = lpDD->QueryInterface(IID_IDirectDraw4, (VOID **)&lpDD4); 
	if ( ddrval != DD_OK )
	{
		MessageBox(NULL, "Query IDirectDraw4 Failed!", "DirectDraw Error", MB_OK | MB_TOPMOST);
		return ddrval;
	}
	g_pDD = lpDD4;

	lpDD->Release();

	ddrval = lpDD4->SetCooperativeLevel(hWnd, DDSCL_FULLSCREEN 
											| DDSCL_EXCLUSIVE 
											| DDSCL_NOWINDOWCHANGES);
	if ( ddrval != DD_OK )
	{
		MessageBox(NULL, "Set Cooperative Level Failed!", "DirectDraw Error", MB_OK | MB_TOPMOST);
		return ddrval;
	}

	ddrval = lpDD4->SetDisplayMode(800, 600, 16, 0, 0);
	if ( ddrval != DD_OK )
	{
		MessageBox(NULL, "Set display Mode Failed!", "DirectDraw Error", MB_OK | MB_TOPMOST);
		return ddrval;
	}
/*
	DDSURFACEDESC2 ddsd;
	LPDIRECTDRAWSURFACE4 lpDDSPrimary;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	ddrval = lpDD4->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
	if ( ddrval != DD_OK ) 
	{
		MessageBox(NULL, "Create Primary Surface Failed!", "DirectDraw Error", MB_OK | MB_TOPMOST);
		return ddrval;
	}
	g_pDDSPrimary = lpDDSPrimary;

	//DDBLTFX ddbltfx;
	//memset(&ddbltfx, 0, sizeof(ddbltfx));
	//ddbltfx.dwSize = sizeof(ddbltfx);
	//ddbltfx.dwFillColor = NULL;
	//lpDDSPrimary->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddrval = lpDDSPrimary->GetSurfaceDesc(&ddsd);
	if ( ddrval != DD_OK ) 
	{
		MessageBox(NULL, "GetSurfaceDesc Failed!", "DirectDraw Error", MB_OK | MB_TOPMOST);
		return ddrval;
	}
*/	
	return DD_OK;
}

void ReleaseDDObjects(void)
{
    if (g_pDD != NULL)
    {
        if (g_pDDSPrimary != NULL)
        {
            g_pDDSPrimary->Release();
            g_pDDSPrimary = NULL;
        }
        g_pDD->Release();
        g_pDD = NULL;
    }
}

