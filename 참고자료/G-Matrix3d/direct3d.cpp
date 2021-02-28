/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

#define D3D_OVERLOADS
#include <d3dx.h>
#define RELEASENULL(object) if (object) {object->Release();}

#include "software.h"
#include "readase.h"
#include "matrix.h"
#include "fog.h"

BOOL bD3DXReady;
BOOL bActive;

LPDIRECTDRAW7 pDD;
LPDIRECT3DDEVICE7 pD3DDevice;
ID3DXContext* pD3DX;
LPDIRECTDRAWSURFACE7 pTex;

D3DLVERTEX vTriangle[3];

HWND hwndMain;

void InterpretD3DError(HRESULT hr);
HRESULT SetupDirect3D(HWND hWnd);
HRESULT CloseDirect3D(void);
HRESULT UpdateFrameD3D(void);
HRESULT SetRenderState(void);
HRESULT HandleWindowedModeChanges();

void InterpretD3DError(HRESULT hr)
{
    char errStr[100];
    D3DXGetErrorString(hr, 100, errStr );
    MessageBox(NULL,errStr,"D3DX Error",MB_OK);
}

HRESULT SetupDirect3D(HWND hWnd)
{
	HRESULT hr;

	if( FAILED(hr = D3DXInitialize()) )
		return hr;

	hr = D3DXCreateContext(D3DX_DEFAULT, // D3DX handle
                            0, // flags
                            hWnd,
                            D3DX_DEFAULT, // colorbits
                            D3DX_DEFAULT, // numdepthbits
                            &pD3DX // returned D3DX interface
                            );
	if( FAILED(hr) )
		return hr;

    hwndMain = hWnd;

    pD3DDevice = pD3DX->GetD3DDevice();
    if( pD3DDevice == NULL )
        return E_FAIL;

    pDD = pD3DX->GetDD();
    if( pDD == NULL )
        return E_FAIL;

	SetupWorld();/////////////////////////////////

    bD3DXReady = TRUE;
    bActive = TRUE;
	
	return SetRenderState();
}

HRESULT SetRenderState(void)
{
	HRESULT hr;

	// Dithering
	hr = pD3DDevice->SetRenderState( D3DRENDERSTATE_DITHERENABLE, TRUE );
    if ( FAILED(hr) )
        return hr;

    // Backface culling
	hr = pD3DDevice->SetRenderState( D3DRENDERSTATE_CULLMODE, D3DCULL_NONE );
    if ( FAILED(hr) )
        return hr;

    // Lighting
	hr = pD3DDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
    if ( FAILED(hr) )
        return hr;

	hr = pD3DDevice->SetRenderState( D3DRENDERSTATE_FOGENABLE, TRUE );
    if ( FAILED(hr) )
        return hr;

	hr = pD3DDevice->SetRenderState( D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_EXP );
    if ( FAILED(hr) )
        return hr;

	float fogdensity = 0.002f;
	hr = pD3DDevice->SetRenderState( D3DRENDERSTATE_FOGDENSITY, *((LPDWORD)(&fogdensity)) );
    if ( FAILED(hr) )
        return hr;

	hr = pD3DDevice->SetRenderState( D3DRENDERSTATE_FOGCOLOR, D3DRGB(0.0f, 0.0f, 1.0f) );
    if ( FAILED(hr) )
        return hr;
	
	DWORD mipmaps = 8;
	hr = D3DXCreateTextureFromFile( pD3DDevice,
                                    0,
                                    0,  // use texture w
                                    0,  // use texture h
                                    NULL,
                                    NULL,
                                    &pTex,
                                    &mipmaps,			// mipmap count
                                    "check.bmp",
                                    D3DX_FT_LINEAR);
	if ( FAILED(hr) ) 
		return hr;
    
	// Texture Filter
	hr = pD3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
	if ( FAILED(hr) )
		return hr;
	hr = pD3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFG_LINEAR);
	if ( FAILED(hr) )
		return hr;
	hr = pD3DDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFG_LINEAR);
	if ( FAILED(hr) )
		return hr;
	
	hr = pD3DDevice->SetTexture(0, pTex);
    if ( FAILED(hr) )
        return hr;
	
	//Z-buffer
	//hr = pD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, D3DZB_FALSE );
    //if ( FAILED(hr) )
    //    return hr;

    // background color
    hr = pD3DX->SetClearColor(D3DRGBA(0.0f,0.0f,1.0f,0));
    if( FAILED(hr) )
        return hr;

    hr = pD3DX->Clear(D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER);
    if ( FAILED(hr) )
        return hr;
	
    return S_OK;
}

HRESULT CloseDirect3D(void)
{
    RELEASENULL(pTex);
    RELEASENULL(pDD);
    RELEASENULL(pD3DDevice);
    RELEASENULL(pD3DX);
	
	bD3DXReady = FALSE;
    
	D3DXUninitialize();

	CloseWorld();
    
	return S_OK;
}

HRESULT UpdateFrameD3D(void)
{
    HRESULT hr;

	if (bIsASE == FALSE) return S_OK;
	if (bD3DXReady == FALSE) return E_FAIL;
	if (bActive == FALSE) return S_OK;

	int frames = GetFrames();
	SetFrames(++frames);

	if (FOG_ENABLE)
		pD3DDevice->SetRenderState( D3DRENDERSTATE_FOGENABLE, TRUE );
	else
		pD3DDevice->SetRenderState( D3DRENDERSTATE_FOGENABLE, FALSE );

	BackFaceCullingAtModel();
	VertexShadingAtModel();
	
	D3DMATRIX world, view, projection;

	SetActiveTM(CURRENTTM);
	LoadIdentity();
	
	RotateX(ModelAngle.x);
	RotateY(ModelAngle.y);
	RotateZ(ModelAngle.z);
	Translate(ModelCoord.x, ModelCoord.y, ModelCoord.z);

	world._11 = CTM.m11; world._12 = CTM.m12; world._13 = CTM.m13; world._14 = CTM.m14;
	world._21 = CTM.m21; world._22 = CTM.m22; world._23 = CTM.m23; world._24 = CTM.m24;
	world._31 = CTM.m31; world._32 = CTM.m32; world._33 = CTM.m33; world._34 = CTM.m34;
	world._41 = CTM.m41; world._42 = CTM.m42; world._43 = CTM.m43; world._44 = CTM.m44;
	
	SetActiveTM(CURRENTTM);
	LoadIdentity();
	Translate(-ViewCoord.x, -ViewCoord.y, -ViewCoord.z);
	RotateZ(-ViewAngle.z);
	RotateY(-ViewAngle.y);
	RotateX(-ViewAngle.x);

	view._11 = CTM.m11; view._12 = CTM.m12; view._13 = CTM.m13; view._14 = CTM.m14;
	view._21 = CTM.m21; view._22 = CTM.m22; view._23 = CTM.m23; view._24 = CTM.m24;
	view._31 = CTM.m31; view._32 = CTM.m32; view._33 = CTM.m33; view._34 = CTM.m34;
	view._41 = CTM.m41; view._42 = CTM.m42; view._43 = CTM.m43; view._44 = CTM.m44;
	
	SetActiveTM(CURRENTTM);
	LoadIdentity();
	PerspectiveFOV(640, 480, 320, 50, 1000);

	projection._11 = CTM.m11; projection._12 = CTM.m12; projection._13 = CTM.m13; projection._14 = CTM.m14;
	projection._21 = CTM.m21; projection._22 = CTM.m22; projection._23 = CTM.m23; projection._24 = CTM.m24;
	projection._31 = CTM.m31; projection._32 = CTM.m32; projection._33 = CTM.m33; projection._34 = CTM.m34;
	projection._41 = CTM.m41; projection._42 = CTM.m42; projection._43 = CTM.m43; projection._44 = CTM.m44;

	pD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &world);
	pD3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &view);
	pD3DDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &projection);

	pD3DX->Clear(D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER);
    
	hr = pD3DDevice->BeginScene();
    if( SUCCEEDED( hr ) )
    {
		for (int i = 0; i < MAXTRIANGLE; i++)
		{
		
			if (TriangleList[i].IsVisible == FALSE) continue;

			int i0 = TriangleList[i].VertexIndex[0];
			int i1 = TriangleList[i].VertexIndex[1];
			int i2 = TriangleList[i].VertexIndex[2];
		
			BYTE R0 = (BYTE)TriangleList[i].VertexColor[0].r;
			BYTE G0 = (BYTE)TriangleList[i].VertexColor[0].g;
			BYTE B0 = (BYTE)TriangleList[i].VertexColor[0].b;
			BYTE R1 = (BYTE)TriangleList[i].VertexColor[1].r;
			BYTE G1 = (BYTE)TriangleList[i].VertexColor[1].g;
			BYTE B1 = (BYTE)TriangleList[i].VertexColor[1].b;
			BYTE R2 = (BYTE)TriangleList[i].VertexColor[2].r;
			BYTE G2 = (BYTE)TriangleList[i].VertexColor[2].g;
			BYTE B2 = (BYTE)TriangleList[i].VertexColor[2].b;

			vTriangle[0].x = VertexList[i0].x;
			vTriangle[0].y = VertexList[i0].y;
			vTriangle[0].z = VertexList[i0].z;
			vTriangle[0].color = RGB_MAKE(R0,G0,B0);
			vTriangle[0].tu = TriangleList[i].VertexTexture[0].u; 
			vTriangle[0].tv = 1.0f - TriangleList[i].VertexTexture[0].v;
			
			vTriangle[1].x = VertexList[i1].x;
			vTriangle[1].y = VertexList[i1].y;
			vTriangle[1].z = VertexList[i1].z;
			vTriangle[1].color = RGB_MAKE(R1,G1,B1);
			vTriangle[1].tu = TriangleList[i].VertexTexture[1].u; 
			vTriangle[1].tv = 1.0f - TriangleList[i].VertexTexture[1].v;
			
			vTriangle[2].x = VertexList[i2].x;
			vTriangle[2].y = VertexList[i2].y;
			vTriangle[2].z = VertexList[i2].z;
			vTriangle[2].color = RGB_MAKE(R2,G2,B2);
			vTriangle[2].tu = TriangleList[i].VertexTexture[2].u; 
			vTriangle[2].tv = 1.0f - TriangleList[i].VertexTexture[2].v;
			
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_LVERTEX, vTriangle, 3, D3DDP_WAIT);
        }
		pD3DDevice->EndScene();
    }
	hr = pD3DX->UpdateFrame( 0 );
    if ( hr == DDERR_SURFACELOST || hr == DDERR_SURFACEBUSY )
        hr = HandleWindowedModeChanges();

	return hr;
}

HRESULT HandleWindowedModeChanges()
{
    HRESULT hr;
    hr = pDD->TestCooperativeLevel();

    if( SUCCEEDED( hr ) )
    {
        // This means that mode changes had taken place, surfaces
        // were lost but still we are in the original mode, so we
        // simply restore all surfaces and keep going.
        if( FAILED( pDD->RestoreAllSurfaces() ) )
            return hr;
    }
    else if( hr == DDERR_WRONGMODE )
    {
        // This means that the desktop mode has changed
        // we can destroy and recreate everything back again.
        if(FAILED(hr = CloseDirect3D()))
            return hr;
        if(FAILED(hr = SetupDirect3D(hwndMain)))
            return hr;
    }
    else if( hr == DDERR_EXCLUSIVEMODEALREADYSET )
    {
        // This means that some app took exclusive mode access
        // we need to sit in a loop till we get back to the right mode.
        do
        {
            Sleep( 500 );
        } while( DDERR_EXCLUSIVEMODEALREADYSET == 
                 (hr = pDD->TestCooperativeLevel()) );
        if( SUCCEEDED( hr ) )
        {
            // This means that the exclusive mode app relinquished its 
            // control and we are back to the safe mode, so simply restore
            if( FAILED( pDD->RestoreAllSurfaces() ) )
                return hr;
        }
        else if( DDERR_WRONGMODE == hr )
        {
            // This means that the exclusive mode app relinquished its 
            // control BUT we are back to some strange mode, so destroy
            // and recreate.
            if(FAILED(hr = CloseDirect3D()))
                return hr;
            if(FAILED(hr = SetupDirect3D(hwndMain)))
                return hr;
        }
        else
        {
            // Busted!!
            return hr;
        }
    }
    else
    {
        // Busted!!
        return hr;
    }
    return S_OK;
}
