/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"

typedef struct tagBITMAPINFO16{
	BITMAPINFOHEADER bmiHeader;
	DWORD Bitmask[3];
}BITMAPINFO16;

extern int RENDERSTATE;

extern int MAXVERTEX;
extern int MAXTRIANGLE;
extern int MAXLIGHT;

extern int TextureWidth;
extern int TextureHeight;
extern BYTE * pTexture;

extern VERTEX * VertexList;
extern VERTEX * ViewVertexList;
extern VERTEX * ScreenVertexList;

extern TRIANGLE * TriangleList;
extern MESH * MeshList;
extern MODEL * ModelList;

extern BOOL bIsActive;
extern BOOL bIsDIB;

extern HDC hScreenDC, hMemoryDC;

extern POINT3D ViewCoord;
extern POINT3D ViewAngle;
extern POINT3D ModelCoord;
extern POINT3D ModelAngle;

BOOL SetupSoftware(HWND hWnd);
BOOL CloseSoftware(HWND hWnd);

int GetFrames(void);
void SetFrames(int);

void UpdateFrame(void);

//void World2View(void);
//void View2Screen(void);

void SetupWorld();
void CloseWorld();

void MakeFaceNormalFromWorldVertex(void);
void MakeFaceNormalFromViewVertex(void);
void MakeVertexNormalFromFaceNormal(void);

void BackFaceCulling(void);
void BackFaceCullingAtModel(void);

void FlatShading(void);
void VertexShading(void);
void VertexShadingAtModel(void);

void NearZCulling(float NearZ);

#endif