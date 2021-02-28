/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

#include "types.h"

#include "directdraw.h"
#include "raster.h"
#include "matrix.h"
#include "readase.h"
#include "bmp.h"
#include "file.h"
#include "fog.h"

typedef struct tagBITMAPINFO16{
	BITMAPINFOHEADER bmiHeader;
	DWORD Bitmask[3];
}BITMAPINFO16;

int MAXVERTEX;
int MAXTRIANGLE;
int MAXLIGHT = 1;

VERTEX * VertexList;
VERTEX * ViewVertexList;
VERTEX * ScreenVertexList;

TRIANGLE * TriangleList;
MESH * MeshList;
MODEL * ModelList;

LIGHT * LightList;
LIGHT * ViewLightList;
LIGHT * ScreenLightList;

int *ZsortArray;

POINT3D ModelCoord, ViewCoord;
POINT3D ModelAngle, ViewAngle;

int TextureWidth = 256;
int TextureHeight = 256;
BYTE * pTexture;

HDC hScreenDC, hMemoryDC;
HBITMAP hBM;
HBITMAP hDefaultBitmap;

BYTE *Bits;

int swidth, sheight;
int scanline;
//int start_x = 0,  start_y = 0;

UINT frames;
int RENDERSTATE = 4;

BOOL bIsActive = FALSE;
BOOL bIsDIB = FALSE;

BOOL SetupSoftware(HWND hWnd);
BOOL CloseSoftware(HWND hWnd);

int GetFrames(void);
void SetFrames(int);

void UpdateFrame(void);
void RenderScene(void);

HBITMAP MakeDIBSection(HDC hDC, int width, int height, BYTE **pBits);
HBITMAP MakeDIBSection16(HDC hDC, int width, int height, BYTE **pBits);

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

BOOL SetupSoftware(HWND hWnd)
{
	hScreenDC = GetDC(hWnd);
	hMemoryDC = CreateCompatibleDC(hScreenDC);
	//hBM = MakeDIBSection(hScreenDC, 640, 480, &Bits);
	hBM = MakeDIBSection16(hScreenDC, 640, 480, &Bits);
	hDefaultBitmap = (HBITMAP)SelectObject(hMemoryDC, hBM);

	bIsActive = TRUE;

	return TRUE;
}

BOOL CloseSoftware(HWND hWnd)
{
	SelectObject(hMemoryDC, hDefaultBitmap);
	DeleteDC(hMemoryDC);
	DeleteObject(hDefaultBitmap);
	DeleteObject(hBM);
	ReleaseDC(hWnd, hScreenDC);
		
	ReleaseDDObjects();
	CloseWorld();

	return TRUE;
}

int GetFrames(void)
{
	return frames;
}

void SetFrames(int f)
{
	frames = f;
}

void UpdateFrame(void)
{
	char string[80];

	if ( bIsDIB == FALSE) return;
	if ( bIsASE == FALSE) return;

	RenderScene();

	//DrawBox(0, 0, swidth-1, sheight-1, (char)0x00);
	
	sprintf(string, "Rendering Method: %d",RENDERSTATE);
	TextOut(hMemoryDC, 400, 0, string, strlen(string));
	sprintf(string, "View-X: %4d, View-Y: %4d, View-Z: %4d  ",
			(int)ViewCoord.x,(int)ViewCoord.y,(int)ViewCoord.z);
	TextOut(hMemoryDC, 0, 0, string, strlen(string));

	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	if (FALSE)//g_pDDSPrimary->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK )
	{
		sprintf(string, "Surface Lock Suceed!");
		TextOut(hMemoryDC, 400, 0, string, strlen(string));

		BYTE *pScreen = (BYTE *)ddsd.lpSurface;

		int	srcwadd = scanline - swidth * 3;
		int	destwadd = (ddsd.lPitch - swidth * 2);
		int deststart = 0;

		_asm {
			xor		ebx, ebx
			mov		esi, Bits
			mov		edi, pScreen
			add		edi, deststart

			mov		ecx, sheight
		NEXTH:
			push	ecx
			mov		edx, swidth
		NEXTW:
			mov		ax, word ptr[esi]		// al = B, ah = G
			mov		bh, byte ptr[esi + 2]	// bh = R
			add		esi, 3

			and		bh, 11111000b			// RGB 565
			shr		ah, 2
			shr		ax, 3
			or		ax, bx 
			
			mov		word ptr[edi], ax
			add		edi, 2

			dec		edx
			jnz		NEXTW
			
			add		esi, srcwadd
			add		edi, destwadd

			pop		ecx
			dec		ecx
			jnz		NEXTH

		}
		g_pDDSPrimary->Unlock(NULL);
	}
	else // 
	{
		sprintf(string, "Surface Lock Failed!");
		//TextOut(hMemoryDC, 400, 0, string, strlen(string));

		//if ( g_pDDSPrimary->IsLost() == DDERR_SURFACELOST )
		//{
		//	g_pDDSPrimary->Restore();
		//}

		BitBlt(hScreenDC, 0, 0, 640, 480,
			hMemoryDC, 0, 0, SRCCOPY);
	}

	frames++;
}

HBITMAP MakeDIBSection(HDC hDC, int width, int height, BYTE **pBits)
{
	HBITMAP hDIBitmap;
	BITMAPINFO BMInfo;

	BMInfo.bmiHeader.biSize				= sizeof (BITMAPINFOHEADER);
	BMInfo.bmiHeader.biWidth			= width;
	BMInfo.bmiHeader.biHeight			= -height;
	BMInfo.bmiHeader.biPlanes			= 1;
	BMInfo.bmiHeader.biBitCount			= 24;
	BMInfo.bmiHeader.biCompression		= BI_RGB;
	BMInfo.bmiHeader.biSizeImage		= 0;
	BMInfo.bmiHeader.biXPelsPerMeter	= 0;
	BMInfo.bmiHeader.biYPelsPerMeter	= 0;
	BMInfo.bmiHeader.biClrUsed			= 0;
	BMInfo.bmiHeader.biClrImportant		= 0;

	hDIBitmap = CreateDIBSection (hDC, (BITMAPINFO *)&BMInfo, DIB_RGB_COLORS, (VOID **)pBits, NULL, 0);

	scanline = (width * 3 + 3)/4 * 4;
	swidth = width;
	sheight = height;
	
	bIsDIB = TRUE;

	return (hDIBitmap);
}

HBITMAP MakeDIBSection16(HDC hDC, int width, int height, BYTE **pBits)
{
	HBITMAP hDIBitmap;
	BITMAPINFO16 BMInfo;

	BMInfo.bmiHeader.biSize				= sizeof (BITMAPINFOHEADER);
	BMInfo.bmiHeader.biWidth			= width;
	BMInfo.bmiHeader.biHeight			= -height;
	BMInfo.bmiHeader.biPlanes			= 1;
	BMInfo.bmiHeader.biBitCount			= 16;
	BMInfo.bmiHeader.biCompression		= BI_BITFIELDS;
	BMInfo.bmiHeader.biSizeImage		= 0;
	BMInfo.bmiHeader.biXPelsPerMeter	= 0;
	BMInfo.bmiHeader.biYPelsPerMeter	= 0;
	BMInfo.bmiHeader.biClrUsed			= 0;
	BMInfo.bmiHeader.biClrImportant		= 0;
	
	BMInfo.Bitmask[0] = 0xF800;//5 red
	BMInfo.Bitmask[1] = 0x07E0;//6 green
	BMInfo.Bitmask[2] = 0x001F;//5 blue

	hDIBitmap = CreateDIBSection (hDC, (BITMAPINFO *)&BMInfo, DIB_RGB_COLORS, (VOID **)pBits, NULL, 0);

	scanline = (width * 2 + 3)/4 * 4;
	swidth = width;
	sheight = height;
	
	bIsDIB = TRUE;

	return (hDIBitmap);
}

int Zcompare(const void * element1, const void * element2)
{
	int index1 = *((int*)element1);
	int index2 = *((int*)element2);

	float zsum1 = ScreenVertexList[ TriangleList[index1].VertexIndex[0] ].w;
	zsum1 += ScreenVertexList[ TriangleList[index1].VertexIndex[1] ].w;
	zsum1 += ScreenVertexList[ TriangleList[index1].VertexIndex[2] ].w;
	
	float zsum2 = ScreenVertexList[ TriangleList[index2].VertexIndex[0] ].w;
	zsum2 += ScreenVertexList[ TriangleList[index2].VertexIndex[1] ].w;
	zsum2 += ScreenVertexList[ TriangleList[index2].VertexIndex[2] ].w;

	if (zsum1 < zsum2) return 1;
	if (zsum1 > zsum2) return -1;

	return 0;
}

void RenderScene(void)
{
	TRISTRUCT Tri;
	
	//MakeFaceNormalFromWorldVertex();
	BackFaceCullingAtModel();

	VertexShadingAtModel();
	
	SetActiveTM(CURRENTTM);
	LoadIdentity();
	
	RotateX(ModelAngle.x);
	RotateY(ModelAngle.y);
	RotateZ(ModelAngle.z);
	Translate(ModelCoord.x, ModelCoord.y, ModelCoord.z);
	
	Translate(-ViewCoord.x, -ViewCoord.y, -ViewCoord.z);
	RotateZ(-ViewAngle.z);
	RotateY(-ViewAngle.y);
	RotateX(-ViewAngle.x);

	for (int i = 0; i < MAXVERTEX; i++)
		TransformCTM(&ViewVertexList[i], &VertexList[i]);
	
	NearZCulling(50);
	
	SetFogColor(0, 0, 255);
	FogLinear(ViewVertexList, 100, 800);
	//FogExponent(density);
	//FogExponent2(density);

	PerspectiveFOV(640, 480, 320, 50, 1000);
	
	Scale(640.0f/2.0f, -480.0f/2.0f, 1.0f - 0.0f);	// Viewport transformation
	Translate(320, 240, 0.0f);						// (0, 0) -	(639, 479)
	
	for (i = 0; i < MAXVERTEX; i++)
		TransformCTM(&ScreenVertexList[i], &VertexList[i]);

	ZsortArray = (int*)malloc(MAXTRIANGLE * sizeof(int));
	
	for (i = 0; i < MAXTRIANGLE; i++)
		ZsortArray[i] = i;

	qsort(ZsortArray, MAXTRIANGLE, sizeof(int), Zcompare);
	
	ClearDIBSection16(0, 0, 255);
	
	int drawedtri = 0;
	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		int j = ZsortArray[i];
		
		if (TriangleList[j].IsVisible == FALSE) continue;

		VERTEX TVertex[3];
		
		int i0 = TriangleList[j].VertexIndex[0];
		int i1 = TriangleList[j].VertexIndex[1];
		int i2 = TriangleList[j].VertexIndex[2];
		
		TransformCTM(&TVertex[0], &VertexList[i0]);
		TransformCTM(&TVertex[1], &VertexList[i1]);
		TransformCTM(&TVertex[2], &VertexList[i2]);
		
		float RHW = 1.0f / TVertex[0].w;
		Tri.Vertex[0].x = RHW * TVertex[0].x;
		Tri.Vertex[0].y = RHW * TVertex[0].y;
		Tri.Vertex[0].z = TVertex[0].w;

		RHW = 1.0f / TVertex[1].w;
		Tri.Vertex[1].x = RHW * TVertex[1].x;
		Tri.Vertex[1].y = RHW * TVertex[1].y;
		Tri.Vertex[1].z = TVertex[1].w;

		RHW = 1.0f / TVertex[2].w;
		Tri.Vertex[2].x = RHW * TVertex[2].x;
		Tri.Vertex[2].y = RHW * TVertex[2].y;
		Tri.Vertex[2].z = TVertex[2].w;

		//char string[20];
		//sprintf(string, "Z: %f  ", TVertex[0].z * RHW);
		//TextOut(hMemoryDC, 0, 300, string, strlen(string));
		//sprintf(string, "W: %f  ", TVertex[0].w);
		//TextOut(hMemoryDC, 0, 320, string, strlen(string));
		
		drawedtri++;
		switch(RENDERSTATE)
		{
			case 0: DrawTri(&Tri, (char)255); break;
			case 1: FillTri(&Tri, TriangleList[j].VertexColor[1]); break;
			case 2: FillTriGouraud(&Tri, &TriangleList[j]); break;
			case 3: FillTriGouraudTexture(&Tri, &TriangleList[j]); break;
			case 4: FillTriGouraudPerspectTexturef(&Tri, &TriangleList[j]); break;
			case 5: FillTriGouraudPerspectTexturef16(&Tri, &TriangleList[j]); break;
			case 6: FillTriPerspectTexturef16(&Tri, &TriangleList[j]); break;
		}
	}

	char string[80];
	sprintf(string, "numDrawed: %d", drawedtri);
	TextOut(hMemoryDC, 0, 200, string, strlen(string));
	sprintf(string, "numTriangles: %d", MAXTRIANGLE);
	TextOut(hMemoryDC, 0, 220, string, strlen(string));

	free(ZsortArray);

}

void SetupWorld(void)
{
	LightList = (LIGHT *)malloc(MAXLIGHT * sizeof(LIGHT));
	ViewLightList = (LIGHT *)malloc(MAXLIGHT * sizeof(LIGHT));
	ScreenLightList = (LIGHT *)malloc(MAXLIGHT * sizeof(LIGHT));
	
	LightList[0].Type = 0;
	LightList[0].Location.x = 100;
	LightList[0].Location.y = 100;
	LightList[0].Location.z = -200;
	LightList[0].Intensity.r = 150;
	LightList[0].Intensity.g = 200;
	LightList[0].Intensity.b = 200;

	ViewCoord.x = 0.0f;
	ViewCoord.y = 0.0f;
	ViewCoord.z = -320.0f;
	
	ModelAngle.x = 0.0f;
	ModelAngle.y = 0.0f;
	ModelAngle.z = 0.0f;
	
	ModelCoord.x = 0.0f;
	ModelCoord.y = 0.0f;
	ModelCoord.z = 0.0f;

}

void CloseWorld(void)
{
	if (VertexList)	
		free(VertexList);
	if (ViewVertexList)	
		free(ViewVertexList);
	if (ScreenVertexList)	
		free(ScreenVertexList);
	
	if (TriangleList) 
		free(TriangleList);
	
	if (LightList) 
		free(LightList);
	if (ViewLightList) 
		free(ViewLightList);
	if (ScreenLightList) 
		free(ScreenLightList);
	
	if (pTexture) 
		free(pTexture);
}

/*
void World2View(void)
{
	int i;
	POINT3D temp1, temp2, temp3;

	// Transform all Vertices
	for ( i = 0; i < MAXVERTEX; i++)
	{
		temp1.x = VertexList[i].x * (float)cos(ViewAngle.y) - VertexList[i].z * (float)sin(ViewAngle.y);
		temp1.y = VertexList[i].y;
		temp1.z = VertexList[i].x * (float)sin(ViewAngle.y) + VertexList[i].z * (float)cos(ViewAngle.y);
			
		temp2.x = temp1.x;
		temp2.y = temp1.y * (float)cos(ViewAngle.x) + temp1.z * (float)sin(ViewAngle.x);
		temp2.z = -temp1.y * (float)sin(ViewAngle.x) + temp1.z * (float)cos(ViewAngle.x);
			
		temp3.x = temp2.x * (float)cos(ViewAngle.z) + temp2.y * (float)sin(ViewAngle.z);
		temp3.y = -temp2.x * (float)sin(ViewAngle.z) + temp2.y * (float)cos(ViewAngle.z);
		temp3.z = temp2.z;
			
		ViewVertexList[i].x = temp3.x - ViewCoord.x;
		ViewVertexList[i].y = temp3.y - ViewCoord.y;
		ViewVertexList[i].z = temp3.z - ViewCoord.z;
	}
	
	// Transform all Lights
	for ( i = 0; i < MAXLIGHT; i++)
	{
		
		temp1.x = LightList[i].Location.x * (float)cos(ViewAngle.y) - LightList[i].Location.z * (float)sin(ViewAngle.y);
		temp1.y = LightList[i].Location.y;
		temp1.z = LightList[i].Location.x * (float)sin(ViewAngle.y) + LightList[i].Location.z * (float)cos(ViewAngle.y);
			
		temp2.x = temp1.x;
		temp2.y = temp1.y * (float)cos(ViewAngle.x) + temp1.z * (float)sin(ViewAngle.x);
		temp2.z = -temp1.y * (float)sin(ViewAngle.x) + temp1.z * (float)cos(ViewAngle.x);
			
		temp3.x = temp2.x * (float)cos(ViewAngle.z) + temp2.y * (float)sin(ViewAngle.z);
		temp3.y = -temp2.x * (float)sin(ViewAngle.z) + temp2.y * (float)cos(ViewAngle.z);
		temp3.z = temp2.z;
			
		ViewLightList[i].Location.x = temp3.x - ViewCoord.x;
		ViewLightList[i].Location.y = temp3.y - ViewCoord.y;
		ViewLightList[i].Location.z = temp3.z - ViewCoord.z;

		ViewLightList[i].Intensity.r = LightList[i].Intensity.r;
		ViewLightList[i].Intensity.g = LightList[i].Intensity.g;
		ViewLightList[i].Intensity.b = LightList[i].Intensity.b;
	}

}

void View2Screen(void)
{
	int i;
	
	// Transform all Vertices
	for ( i = 0; i < MAXVERTEX; i++)
	{
		ScreenVertexList[i].x = 320 * ViewVertexList[i].x / ViewVertexList[i].z;
		ScreenVertexList[i].y = 320 * ViewVertexList[i].y / ViewVertexList[i].z;
		ScreenVertexList[i].z = ViewVertexList[i].z;
	}

	// Transform all Lights
	for ( i = 0; i < MAXLIGHT; i++)
	{
		ScreenLightList[i].Location.x = 320 * ViewLightList[i].Location.x / ViewLightList[i].Location.z;
		ScreenLightList[i].Location.y = 320 * ViewLightList[i].Location.y / ViewLightList[i].Location.z;
		ScreenLightList[i].Location.z = ViewLightList[i].Location.z;
	}

}
*/

void MakeFaceNormalFromViewVertex(void)
{
	int i;
	POINT3D temp1, temp2, temp3;
	float length;

	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		temp1.x = ViewVertexList[ TriangleList[i].VertexIndex[1] ].x - ViewVertexList[ TriangleList[i].VertexIndex[0] ].x;
		temp1.y = ViewVertexList[ TriangleList[i].VertexIndex[1] ].y - ViewVertexList[ TriangleList[i].VertexIndex[0] ].y;
		temp1.z = ViewVertexList[ TriangleList[i].VertexIndex[1] ].z - ViewVertexList[ TriangleList[i].VertexIndex[0] ].z;

		temp2.x = ViewVertexList[ TriangleList[i].VertexIndex[2] ].x - ViewVertexList[ TriangleList[i].VertexIndex[0] ].x;
		temp2.y = ViewVertexList[ TriangleList[i].VertexIndex[2] ].y - ViewVertexList[ TriangleList[i].VertexIndex[0] ].y;
		temp2.z = ViewVertexList[ TriangleList[i].VertexIndex[2] ].z - ViewVertexList[ TriangleList[i].VertexIndex[0] ].z;
		
		temp3.x = temp1.y * temp2.z - temp1.z * temp2.y;
		temp3.y = temp1.z * temp2.x - temp1.x * temp2.z;
		temp3.z = temp1.x * temp2.y - temp1.y * temp2.x;

		length = (float)sqrt(temp3.x * temp3.x + temp3.y * temp3.y + temp3.z * temp3.z);

		TriangleList[i].FaceNormal.x = temp3.x / length;
		TriangleList[i].FaceNormal.y = temp3.y / length;
		TriangleList[i].FaceNormal.z = temp3.z / length;
	}
}

void MakeFaceNormalFromWorldVertex(void)
{
	int i;
	POINT3D temp1, temp2, temp3;
	float length;

	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		temp1.x = VertexList[ TriangleList[i].VertexIndex[1] ].x - VertexList[ TriangleList[i].VertexIndex[0] ].x;
		temp1.y = VertexList[ TriangleList[i].VertexIndex[1] ].y - VertexList[ TriangleList[i].VertexIndex[0] ].y;
		temp1.z = VertexList[ TriangleList[i].VertexIndex[1] ].z - VertexList[ TriangleList[i].VertexIndex[0] ].z;

		temp2.x = VertexList[ TriangleList[i].VertexIndex[2] ].x - VertexList[ TriangleList[i].VertexIndex[0] ].x;
		temp2.y = VertexList[ TriangleList[i].VertexIndex[2] ].y - VertexList[ TriangleList[i].VertexIndex[0] ].y;
		temp2.z = VertexList[ TriangleList[i].VertexIndex[2] ].z - VertexList[ TriangleList[i].VertexIndex[0] ].z;
		
		temp3.x = temp1.y * temp2.z - temp1.z * temp2.y;
		temp3.y = temp1.z * temp2.x - temp1.x * temp2.z;
		temp3.z = temp1.x * temp2.y - temp1.y * temp2.x;

		length = (float)sqrt(temp3.x * temp3.x + temp3.y * temp3.y + temp3.z * temp3.z);

		TriangleList[i].FaceNormal.x = temp3.x / length;
		TriangleList[i].FaceNormal.y = temp3.y / length;
		TriangleList[i].FaceNormal.z = temp3.z / length;
	}
}

void MakeVertexNormalFromFaceNormal(void)
{
	int i;
	//int j;
	int k;
	//int l; 
	int count;
	
	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		for ( k = 0; k < 3; k++)
		{
			//TriangleList[i].VertexNormal[k].x = 0;
			//TriangleList[i].VertexNormal[k].y = 0;
			//TriangleList[i].VertexNormal[k].z = 0;

			//TriangleList[i].VertexNormal[k].x += TriangleList[i].FaceNormal.x;
			//TriangleList[i].VertexNormal[k].y += TriangleList[i].FaceNormal.y;
			//TriangleList[i].VertexNormal[k].z += TriangleList[i].FaceNormal.z;
			TriangleList[i].VertexNormal[k].x = TriangleList[i].FaceNormal.x;
			TriangleList[i].VertexNormal[k].y = TriangleList[i].FaceNormal.y;
			TriangleList[i].VertexNormal[k].z = TriangleList[i].FaceNormal.z;
			count = 1;
/*		
			// Search for comman Face
			for ( j = 0; j < MAXTRIANGLE; j++)
			{
				if ( i == j ) continue;
			
				// Search for comman Vertex
				for ( l = 0; l < 3; l++ )
				{
					if ( TriangleList[i].VertexIndex[k] == TriangleList[j].VertexIndex[l] ) 
					{
						TriangleList[i].VertexNormal[k].x += TriangleList[j].FaceNormal.x;
						TriangleList[i].VertexNormal[k].y += TriangleList[j].FaceNormal.y;
						TriangleList[i].VertexNormal[k].z += TriangleList[j].FaceNormal.z;
						count++;
						break;
					}
				}
			}

			TriangleList[i].VertexNormal[k].x /= count;
			TriangleList[i].VertexNormal[k].y /= count;
			TriangleList[i].VertexNormal[k].z /= count;
*/
		}

	}

}

void BackFaceCulling(void)
{
	int i;
	float Visible;

	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		TRIANGLE *temp = &TriangleList[i];
		// dot product for visible test
		Visible = (-ViewVertexList[ temp->VertexIndex[0] ].x) * TriangleList[i].FaceNormal.x 
			+ (-ViewVertexList[ temp->VertexIndex[0] ].y) * TriangleList[i].FaceNormal.y 
			+ (-ViewVertexList[ temp->VertexIndex[0] ].z) * TriangleList[i].FaceNormal.z; 

		if ( Visible > 0.0f ) temp->IsVisible = TRUE;
		else temp->IsVisible = FALSE;
	}
}

void BackFaceCullingAtModel(void)
{
	int i;
	float Visible;

	VERTEX Camera = {ViewCoord.x - ModelCoord.x,
					 ViewCoord.y - ModelCoord.y, 
					 ViewCoord.z - ModelCoord.z, 1,};
	VERTEX CameraM;

	SetActiveTM(VIEWTM);
	LoadIdentity();
	RotateZ(-ModelAngle.z);
	RotateY(-ModelAngle.y);
	RotateX(-ModelAngle.x);
	TransformVTM(&CameraM, &Camera);

	for ( i = 0; i < MAXVERTEX; i++)
	{
		VertexList[i].tflag = FALSE;
		//VertexList[i].tflag = TRUE;
	}
	
	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		TRIANGLE *temp = &TriangleList[i];
		// dot product for visible test
		Visible = (CameraM.x - VertexList[ temp->VertexIndex[0] ].x) * TriangleList[i].FaceNormal.x 
			+ (CameraM.y - VertexList[ temp->VertexIndex[0] ].y) * TriangleList[i].FaceNormal.y 
			+ (CameraM.z - VertexList[ temp->VertexIndex[0] ].z) * TriangleList[i].FaceNormal.z; 

		if ( Visible > 0.0f ) 
		{
			temp->IsVisible = TRUE;
			VertexList[ temp->VertexIndex[0] ].tflag = TRUE;
			VertexList[ temp->VertexIndex[1] ].tflag = TRUE;
			VertexList[ temp->VertexIndex[2] ].tflag = TRUE;
		}
		else temp->IsVisible = FALSE;
	}

}

void FlatShading(void)
{
	int i;
	POINT3D Light;
	float Length, Inner;
	COLORf24 Ambient = { 50, 0, 50 };

	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		if ( TriangleList[i].IsVisible == FALSE) continue;

		Light.x = LightList[0].Location.x - ViewVertexList[TriangleList[i].VertexIndex[0]].x;
		Light.y = LightList[0].Location.y - ViewVertexList[TriangleList[i].VertexIndex[0]].y;
		Light.z = LightList[0].Location.z - ViewVertexList[TriangleList[i].VertexIndex[0]].z;
		
		Length = (float)sqrt(Light.x * Light.x + Light.y * Light.y + Light.z * Light.z);
		
		// Normalize Light vector
		Light.x /= Length;
		Light.y /= Length;
		Light.z /= Length;
		
		Inner = (TriangleList[i].FaceNormal.x * Light.x
				+ TriangleList[i].FaceNormal.y * Light.y
				+ TriangleList[i].FaceNormal.z * Light.z);
		
		if (Inner < 0 ) Inner = 0;

		TriangleList[i].FaceColor.r = Ambient.r + LightList[0].Intensity.r * Inner;
		TriangleList[i].FaceColor.g = Ambient.g + LightList[0].Intensity.g * Inner;
		TriangleList[i].FaceColor.b = Ambient.b + LightList[0].Intensity.b * Inner;

		if (TriangleList[i].FaceColor.r > 255) TriangleList[i].FaceColor.r = 255; 
		if (TriangleList[i].FaceColor.g > 255) TriangleList[i].FaceColor.g = 255; 
		if (TriangleList[i].FaceColor.b > 255) TriangleList[i].FaceColor.b = 255; 
	}
}

void VertexShading(void)
{
	int i, j;
	POINT3D Light;
	float Length, Inner;
	COLORf24 Ambient = { 50, 0, 50 };

	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		if ( TriangleList[i].IsVisible == FALSE) continue;

		for ( j = 0; j < 3; j ++)
		{
			Light.x = ViewVertexList[TriangleList[i].VertexIndex[j]].x - LightList[0].Location.x;
			Light.y = ViewVertexList[TriangleList[i].VertexIndex[j]].y - LightList[0].Location.y;
			Light.z = ViewVertexList[TriangleList[i].VertexIndex[j]].z - LightList[0].Location.z;
		
			Length = (float)sqrt(Light.x * Light.x + Light.y * Light.y + Light.z * Light.z);
		
			// Normalize Light vector
			Light.x /= Length;
			Light.y /= Length;
			Light.z /= Length;
		
			Inner = -(TriangleList[i].VertexNormal[j].x * Light.x
					+ TriangleList[i].VertexNormal[j].y * Light.y
					+ TriangleList[i].VertexNormal[j].z * Light.z);
		
			if (Inner < 0 ) Inner = 0;

			TriangleList[i].VertexColor[j].r = Ambient.r + LightList[0].Intensity.r * Inner;
			TriangleList[i].VertexColor[j].g = Ambient.g + LightList[0].Intensity.g * Inner;
			TriangleList[i].VertexColor[j].b = Ambient.b + LightList[0].Intensity.b * Inner;

			if (TriangleList[i].VertexColor[j].r > 255) TriangleList[i].VertexColor[j].r = 255; 
			if (TriangleList[i].VertexColor[j].g > 255) TriangleList[i].VertexColor[j].g = 255; 
			if (TriangleList[i].VertexColor[j].b > 255) TriangleList[i].VertexColor[j].b = 255;
		} 
	}
}

void VertexShadingAtModel(void)
{
	int i, j;
	POINT3D Light;
	float Length, Inner;
	COLORf24 Ambient = { 10, 10, 10 };

	VERTEX light = {LightList[0].Location.x - ModelCoord.x,
					LightList[0].Location.y - ModelCoord.y, 
					LightList[0].Location.z - ModelCoord.z, 1,};
	VERTEX lightM;

	SetActiveTM(VIEWTM);
	LoadIdentity();
	RotateZ(-ModelAngle.z);
	RotateY(-ModelAngle.y);
	RotateX(-ModelAngle.x);
	TransformVTM(&lightM, &light);


	for ( i = 0; i < MAXTRIANGLE; i++)
	{
		if ( TriangleList[i].IsVisible == FALSE) continue;

		for ( j = 0; j < 3; j ++)
		{
			Light.x = lightM.x - VertexList[TriangleList[i].VertexIndex[j]].x;
			Light.y = lightM.y - VertexList[TriangleList[i].VertexIndex[j]].y;
			Light.z = lightM.z - VertexList[TriangleList[i].VertexIndex[j]].z;
		
			Length = (float)sqrt(Light.x * Light.x + Light.y * Light.y + Light.z * Light.z);
		
			// Normalize Light vector
			Light.x /= Length;
			Light.y /= Length;
			Light.z /= Length;
		
			Inner = (TriangleList[i].VertexNormal[j].x * Light.x
					+ TriangleList[i].VertexNormal[j].y * Light.y
					+ TriangleList[i].VertexNormal[j].z * Light.z);
		
			if (Inner < 0 ) Inner = 0;

			TriangleList[i].VertexColor[j].r = Ambient.r + LightList[0].Intensity.r * Inner;
			TriangleList[i].VertexColor[j].g = Ambient.g + LightList[0].Intensity.g * Inner;
			TriangleList[i].VertexColor[j].b = Ambient.b + LightList[0].Intensity.b * Inner;

			if (TriangleList[i].VertexColor[j].r > 255) TriangleList[i].VertexColor[j].r = 255; 
			if (TriangleList[i].VertexColor[j].g > 255) TriangleList[i].VertexColor[j].g = 255; 
			if (TriangleList[i].VertexColor[j].b > 255) TriangleList[i].VertexColor[j].b = 255;
		} 
	}
}

void NearZCulling(float NearZ)
{	
	for( int i = 0; i < MAXTRIANGLE; i++)
	{
		if (TriangleList[i].IsVisible == FALSE) continue;

		if ( ViewVertexList[ TriangleList[i].VertexIndex[0] ].z < NearZ
			||ViewVertexList[ TriangleList[i].VertexIndex[1] ].z < NearZ
			||ViewVertexList[ TriangleList[i].VertexIndex[2] ].z < NearZ)
			TriangleList[i].IsVisible = FALSE;
	}
}