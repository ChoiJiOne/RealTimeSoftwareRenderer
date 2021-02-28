/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#ifndef RASTER_H
#define RASTER_H

void ClearDIBSection16(int r, int g, int b);

void PutPixel(int x, int y, char color);
void DrawBox(int x1, int y1, int x2, int y2, char color);

void Line(int x1, int y1, int x2, int y2, char color);
void LineClip(int x1, int y1, int x2, int y2, char color);
void LineFixed(int x1, int y1, int x2, int y2, char color);

void HLine(int x1, int x2, int y, COLORf24 color);
void HLineGouraud(int x1, int x2, int y, COLORi24 color1, COLORi24 color2);
void HLineGouraudTexture(int x1, int x2, int y, COLORi24 color1, COLORi24 color2, TEXCOORD tex1, TEXCOORD tex2);
void HLineGouraudPerspectTexturef(int x1, int x2, int y, COLORi24 color1, COLORi24 color2, TEXCOORDFLOAT tex1, TEXCOORDFLOAT tex2);
void HLineGouraudPerspectTexturef16(int x1, int x2, int y, COLORi24 color1, COLORi24 color2, TEXCOORDFLOAT tex1, TEXCOORDFLOAT tex2);
void HLinePerspectTexturef16(int x1, int x2, int y, TEXCOORDFLOAT tex1, TEXCOORDFLOAT tex2);
void HLineFogGouraudPerspectTexturef(int x1, int x2, int y, COLORi24 color1, COLORi24 color2, TEXCOORDFLOAT tex1, TEXCOORDFLOAT tex2, int fog1, int fog2);

void DrawTri(TRISTRUCT * pTri, char color);
void FillTri(TRISTRUCT * pTri, COLORf24 color);
void FillTriGouraud(TRISTRUCT * pTri, TRIANGLE * pTriangle);
void FillTriGouraudTexture(TRISTRUCT * pTri, TRIANGLE * pTriangle);
void FillTriGouraudPerspectTexturef(TRISTRUCT * pTri, TRIANGLE * pTriangle);
void FillTriGouraudPerspectTexturef16(TRISTRUCT * pTri, TRIANGLE * pTriangle);
void FillTriPerspectTexturef16(TRISTRUCT * pTri, TRIANGLE * pTriangle);

#endif