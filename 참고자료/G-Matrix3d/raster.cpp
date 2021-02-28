/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

#include "types.h"
#include "software.h"
#include "fog.h"

extern BYTE *Bits;

extern int swidth, sheight;
extern int scanline;

extern int TextureWidth, TextureHeight;
extern BYTE * pTexture;


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

void ClearDIBSection16(int r, int g, int b)
{
	int i;

	WORD color = ((r>>3)<<11) | ((g>>2)<<5) | (b>>3);
	
	for (i = 0; i < swidth; i++)
		*(((WORD*)Bits) + i) = color;
	
	for (i = 1; i < sheight; i++)
		memcpy(Bits + scanline * i, Bits, scanline);
}

void PutPixel(int x, int y, char color)
{
	unsigned off;

	off = y * scanline + x * 2;// 16bit color
	//off = y * scanline + x * 3;//24bit color
	//*(Bits + off + 0) = color;
	//*(Bits + off + 1) = color;
	//*(Bits + off + 2) = color;

	_asm {
		mov		edi, Bits
		add		edi, off
		mov		al, color
		mov		ah, al
		mov		bl, al
		mov		word ptr[edi], ax
	//	mov		byte ptr[edi + 2], bl
	}
}

void DrawBox(int x1, int y1, int x2, int y2, char color)
{
	int i, j;

	for ( i = x1; i <= x2; i++ )
		PutPixel(i, y1, color);
	for ( i = x1; i <= x2; i++ )
		PutPixel(i, y2, color);
	for ( j = y1; j <= y2; j++ )
		PutPixel(x1, j, color);
	for ( j = y1; j <= y2; j++ )
		PutPixel(x2, j, color);
}

void LineClip(int x1, int y1, int x2, int y2, char color)
{
	float t;
	float fx1, fy1;
	float fx2, fy2;
	float fx1c, fy1c;
	float fx2c, fy2c;

	if ( (x1 >= 0) && (x1 < swidth) && (y1 >= 0) && (y1 < sheight)
	  && (x2 >= 0) && (x2 < swidth) && (y2 >= 0) && (y2 < sheight) ) 
	{
		Line(x1, y1, x2, y2, color);
		return;
	}

	if ( x1 < 0 && x2 < 0 ) return;
	if ( y1 < 0 && y2 < 0 ) return;
	if ( x1 > (swidth - 1) && x2 > (swidth - 1) ) return;
	if ( y1 > (sheight - 1) && y2 > (sheight - 1) ) return;

	fx1c = fx1 = (float)x1;
	fx2c = fx2 = (float)x2;
	fy1c = fy1 = (float)y1;
	fy2c = fy2 = (float)y2;

	if ( (int)fx1c < 0 )
	{
		t = -fx1c / ( fx2 - fx1 );
		fx1c = fx1 + ( fx2 - fx1 ) * t;
		fy1c = fy1 + ( fy2 - fy1 ) * t;
	}
	if ( (int)fy1c < 0 )
	{
		t = -fy1c / ( fy2 - fy1c );
		fx1c = fx1c + ( fx2 - fx1c ) * t;
		fy1c = fy1c + ( fy2 - fy1c ) * t;
	}
	if ( (int)fx1c > swidth - 1 )
	{
		t = (swidth - 1 - fx1c) / ( fx2 - fx1c );
		fx1c = fx1c + ( fx2 - fx1c ) * t;
		fy1c = fy1c + ( fy2 - fy1c ) * t;
	}
	if ( (int)fy1c > sheight - 1 )
	{
		t = (sheight - 1 - fy1c) / ( fy2 - fy1c );
		fx1c = fx1c + ( fx2 - fx1c ) * t;
		fy1c = fy1c + ( fy2 - fy1c ) * t;
	}

	if ( (int)fx2c < 0 )
	{
		t = -fx1c / ( fx2 - fx1c );
		fx2c = fx1c + ( fx2 - fx1c ) * t;
		fy2c = fy1c + ( fy2 - fy1c ) * t;
	}
	if ( (int)fy2c < 0 )
	{
		t = -fy1c / ( fy2c - fy1c );
		fx2c = fx1c + ( fx2c - fx1c ) * t;
		fy2c = fy1c + ( fy2c - fy1c ) * t;
	}			
	if ( (int)fx2c > swidth - 1 )
	{
		t = (swidth - 1 - fx1c) / ( fx2c - fx1c );
		fx2c = fx1c + ( fx2c - fx1c ) * t;
		fy2c = fy1c + ( fy2c - fy1c ) * t;
	}
	if ( (int)fy2c > sheight - 1 )
	{
		t = (sheight - 1 - fy1c) / ( fy2c - fy1c );
		fx2c = fx1c + ( fx2c - fx1c ) * t;
		fy2c = fy1c + ( fy2c - fy1c ) * t;
	}
	
	x1 = (int)fx1c;
	x2 = (int)fx2c;
	y1 = (int)fy1c;
	y2 = (int)fy2c;

	if ( (x1 >= 0) && (x1 < swidth) && (y1 >= 0) && (y1 < sheight)
	  && (x2 >= 0) && (x2 < swidth) && (y2 >= 0) && (y2 < sheight) ) 
	{
		Line(x1, y1, x2, y2, color);
		return;
	}
}

void LineFixed(int x1, int y1, int x2, int y2, char color)
{
	int y, yd;
	int temp, i;
	int delta_x, delta_y;
	
	if ( (x1 == x2) && (y1 == y2) ) return;
	
	if ( x1 > x2 )
	{
		temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	delta_x = x2 - x1;
	delta_y = y2 - y1;

	if ( delta_x < abs(delta_y) )
	{
		temp = x1;
		x1 = y1;
		y1 = temp;

		temp = x2;
		x2 = y2;
		y2 = temp;

		if ( x1 > x2 )
		{
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}

		delta_x = x2 - x1;
		delta_y = y2 - y1;

		y = y1 << 16;

		delta_y <<= 16;
		yd = delta_y / delta_x;

		for ( i = x1; i <= x2; i++ )
		{
			PutPixel(y>>16, i, color);
			y += yd;
		}
	}
	else
	{
		y = y1 << 16;

		delta_y <<= 16;
		yd = delta_y / delta_x;

		for ( i = x1; i <= x2; i++ )
		{
			PutPixel(i, y>>16, color);
			y += yd;
		}
	}
}

void HLine(int x1, int x2, int y, COLORf24 color)
{
	int temp;
	register int off;
	char r, g, b;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if ( x1 < 0 ) x1 = 0;
	if ( x2 > 639 ) x2 = 639;

	off = x1 * 2 + y * scanline;
	temp = x2 * 2 + y * scanline;
/*
	while ( off <= temp )
	{
		Bits[off + 0] = (BYTE)color.b;
		Bits[off + 1] = (BYTE)color.g;
		Bits[off + 2] = (BYTE)color.r;
		off += 3;
	}
*/
	r = (BYTE)color.r;
	g = (BYTE)color.g;
	b = (BYTE)color.b;

	_asm {
		mov		edi, Bits
		mov		edx, off
	
		mov		al, b
		mov		ah, g
		mov		bh, r

		and		bx, 1111100000000000b
		shr		ah, 2
		shr		ax, 3
		or		ax, bx
		//mov		cx, ax
		//shl		eax, 16
		//mov		ax, cx	
	HERE:	
		cmp		edx, temp
		jg		REMAIN
		mov		word ptr[edi + edx], ax
		add		edx, 2
		jmp		HERE

	REMAIN:
		//sub		edx, 4
		//cmp		edx, temp
		//jz		QUIT
		//sub		edx, 2
		//mov		word ptr[edi + edx], ax
	//QUIT:
	}
}

void HLineGouraud(int x1, int x2, int y, COLORi24 color1, COLORi24 color2)
{
	int temp;
	register int off;
	COLORi24 Color, dColor;
	//int r, g, b, delta_r, delta_g, delta_b;

	if (x1 == x2) return;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	
		temp = color1.r;
		color1.r = color2.r;
		color2.r = temp;

		temp = color1.g;
		color1.g = color2.g;
		color2.g = temp;

		temp = color1.b;
		color1.b = color2.b;
		color2.b = temp;
	}

	Color.r = color1.r;
	Color.g = color1.g;
	Color.b = color1.b;

	dColor.r = (color2.r - color1.r) / (x2 - x1);
	dColor.g = (color2.g - color1.g) / (x2 - x1);
	dColor.b = (color2.b - color1.b) / (x2 - x1);
	
	if ( x1 < 0 ) 
	{
		Color.r += dColor.r * (0 - x1);
		Color.g += dColor.g * (0 - x1);
		Color.b += dColor.b * (0 - x1);
		x1 = 0;
	}

	if ( x2 > 639 ) x2 = 639;

	off = x1 * 2 + y * scanline;
	temp = x2 * 2 + y * scanline;
/*	
	while ( off <= temp )
	{
		Bits[off + 0] = (BYTE)(Color.b>>16);
		Bits[off + 1] = (BYTE)(Color.g>>16);
		Bits[off + 2] = (BYTE)(Color.r>>16);
		
		Color.r += dColor.r;
		Color.g += dColor.g;
		Color.b += dColor.b;
		
		off += 3;
	}
*/
	//r = Color.r;
	//g = Color.g;
	//b = Color.b;

	//delta_r = dColor.r;
	//delta_g = dColor.g;
	//delta_b = dColor.b;
	
	temp += (DWORD)Bits;

	_asm {
		mov		edi, Bits
		add		edi, off

		mov		eax, Color.r
		mov		ebx, Color.g
		mov		ecx, Color.b		

	HERE:	
		cmp		edi, temp
		jg		QUIT

		ror		eax, 16
		ror		ebx, 16
		ror		ecx, 16

		mov		dl, cl// b
		mov		dh, bl// g
		mov		si, ax// r
		and		si, 0000000011111000b
		shl		si, 8
		shr		dh, 2
		shr		dx, 3
		or		dx, si

		mov		word ptr[edi], dx

		rol		eax, 16
		rol		ebx, 16
		rol		ecx, 16

		add		eax, dColor.r
		add		ebx, dColor.g
		add		ecx, dColor.b

		add		edi, 2

		jmp		HERE
	
	QUIT:
	
	}

}

void HLineGouraudTexture(int x1, int x2, int y, COLORi24 color1, COLORi24 color2, TEXCOORD tex1, TEXCOORD tex2)
{
	int temp;
	register int off;
	UINT toff;
	COLORi24 Color, dColor;
	TEXCOORD Tex, dTex;

	if (x1 == x2) return;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	
		temp = color1.r;
		color1.r = color2.r;
		color2.r = temp;

		temp = color1.g;
		color1.g = color2.g;
		color2.g = temp;

		temp = color1.b;
		color1.b = color2.b;
		color2.b = temp;
		
		temp = tex1.u;
		tex1.u = tex2.u;
		tex2.u = temp;

		temp = tex1.v;
		tex1.v = tex2.v;
		tex2.v = temp;
	}

	Color.r = color1.r;
	Color.g = color1.g;
	Color.b = color1.b;

	dColor.r = (color2.r - color1.r) / (x2 - x1);
	dColor.g = (color2.g - color1.g) / (x2 - x1);
	dColor.b = (color2.b - color1.b) / (x2 - x1);
	
	Tex.u = tex1.u;
	Tex.v = tex1.v;

	dTex.u = (tex2.u - tex1.u) / (x2 - x1);
	dTex.v = (tex2.v - tex1.v) / (x2 - x1);
	
	if ( x1 < 0 ) 
	{
		Color.r += dColor.r * (0 - x1);
		Color.g += dColor.g * (0 - x1);
		Color.b += dColor.b * (0 - x1);
		Tex.u += dTex.u * (0 - x1);
		Tex.v += dTex.v * (0 - x1);
		x1 = 0;
	}

	if ( x2 > 639 ) x2 = 639;

	off = x1 * 2 + y * scanline;
	temp = x2 * 2 + y * scanline;
	
	while ( off <= temp )
	{
		toff = ((Tex.u>>16) & (TextureWidth-1)) * 2 + ((Tex.v>>16) & (TextureHeight-1)) * 2 * TextureWidth;

		WORD TexColor = *((WORD *)(pTexture + toff));
		TexColor &= 0xF7DE; //1111 0111 1101 1110b
		TexColor >>= 1;
		
		WORD VColor = (BYTE)(Color.r>>20);//16 + 3 + 1
		VColor <<= 11;
		
		WORD VColorTemp = (BYTE)(Color.g>>19);//16 + 2 + 1
		VColor |= VColorTemp << 5;
		
		VColor |= (BYTE)(Color.b>>20);//16 + 3 + 1
		
		VColor += TexColor;

		*((WORD*)(Bits + off)) = VColor;

		//Bits[off + 0] = (BYTE)((Color.b>>17) + (pTexture[toff + 0]>>1));
		//Bits[off + 1] = (BYTE)((Color.g>>17) + (pTexture[toff + 1]>>1));
		//Bits[off + 2] = (BYTE)((Color.r>>17) + (pTexture[toff + 2]>>1));
		
		Color.r += dColor.r;
		Color.g += dColor.g;
		Color.b += dColor.b;
		
		Tex.u += dTex.u;		
		Tex.v += dTex.v;		

		off += 2;
	}

}

void HLineGouraudPerspectTexturef(int x1, int x2, int y, COLORi24 color1, COLORi24 color2, TEXCOORDFLOAT tex1, TEXCOORDFLOAT tex2)
{
	int temp;
	float tempf;
	register int off;
	UINT toff;
	COLORi24 Color, dColor;
	TEXCOORDFLOAT Tex, dTex;

	if (x1 == x2) return;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	
		temp = color1.r;
		color1.r = color2.r;
		color2.r = temp;

		temp = color1.g;
		color1.g = color2.g;
		color2.g = temp;

		temp = color1.b;
		color1.b = color2.b;
		color2.b = temp;
		
		tempf = tex1.uz;
		tex1.uz = tex2.uz;
		tex2.uz = tempf;

		tempf = tex1.vz;
		tex1.vz = tex2.vz;
		tex2.vz = tempf;

		tempf = tex1.oz;
		tex1.oz = tex2.oz;
		tex2.oz = tempf;
	}

	Color.r = color1.r;
	Color.g = color1.g;
	Color.b = color1.b;

	dColor.r = (color2.r - color1.r) / (x2 - x1);
	dColor.g = (color2.g - color1.g) / (x2 - x1);
	dColor.b = (color2.b - color1.b) / (x2 - x1);
	
	Tex.uz = tex1.uz;
	Tex.vz = tex1.vz;
	Tex.oz = tex1.oz;

	dTex.uz = (tex2.uz - tex1.uz) / (x2 - x1);
	dTex.vz = (tex2.vz - tex1.vz) / (x2 - x1);
	dTex.oz = (tex2.oz - tex1.oz) / (x2 - x1);
	
	if ( x1 < 0 ) 
	{
		Color.r += dColor.r * (0 - x1);
		Color.g += dColor.g * (0 - x1);
		Color.b += dColor.b * (0 - x1);
		Tex.uz += dTex.uz * (0 - x1);
		Tex.vz += dTex.vz * (0 - x1);
		Tex.oz += dTex.oz * (0 - x1);
		x1 = 0;
	}

	if ( x2 > 639 ) x2 = 639;

	off = x1 * 2 + y * scanline;
	temp = x2 * 2 + y * scanline;
	
	while ( off <= temp )
	{
		int su, sv;
		float z;

		z = 1 / Tex.oz;
		//su = (int)(Tex.uz * z);
		//sv = (int)(Tex.vz * z);
		_asm {
			fld		Tex.uz
			fmul	z
			fistp	su
			
			fld		Tex.vz
			fmul	z
			fistp	sv
		}

		toff = (su & (TextureWidth-1)) * 2 + (sv & (TextureHeight-1))  * 2 * TextureWidth;

		WORD TexColor = *((WORD *)(pTexture + toff));
		TexColor &= 0xF7DE; //1111 0111 1101 1110b
		TexColor >>= 1;
		
		WORD VColor = (BYTE)(Color.r>>20);//16 + 3 + 1
		VColor <<= 11;
		
		WORD VColorTemp = (BYTE)(Color.g>>19);//16 + 2 + 1
		VColor |= VColorTemp << 5;
		
		VColor |= (BYTE)(Color.b>>20);//16 + 3 + 1
		
		VColor += TexColor;

		*((WORD*)(Bits + off)) = VColor;

		//Bits[off + 0] = (BYTE)((Color.b>>17) + (pTexture[toff + 0]>>1));
		//Bits[off + 1] = (BYTE)((Color.g>>17) + (pTexture[toff + 1]>>1));
		//Bits[off + 2] = (BYTE)((Color.r>>17) + (pTexture[toff + 2]>>1));
		
		Color.r += dColor.r;
		Color.g += dColor.g;
		Color.b += dColor.b;
		
		Tex.uz += dTex.uz;		
		Tex.vz += dTex.vz;		
		Tex.oz += dTex.oz;		

		off += 2;
	}

}

void HLineFogGouraudPerspectTexturef(int x1, int x2, int y, COLORi24 color1, COLORi24 color2, TEXCOORDFLOAT tex1, TEXCOORDFLOAT tex2, int fog1, int fog2)
{
	int temp;
	float tempf;
	register int off;
	UINT toff;
	COLORi24 Color, dColor;
	TEXCOORDFLOAT Tex, dTex;
	int Fog, dFog;
	FIXED16 xf1, xf2;

	xf1 = x1;
	xf2 = x2;

	x1 = (x1+65535)>>16;
	x2 = (x2+65535)>>16;
	
	if (x1 == x2) return;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
		
		temp = xf1;
		xf1 = xf2;
		xf2 = temp;
	
		temp = color1.r;
		color1.r = color2.r;
		color2.r = temp;

		temp = color1.g;
		color1.g = color2.g;
		color2.g = temp;

		temp = color1.b;
		color1.b = color2.b;
		color2.b = temp;
		
		tempf = tex1.uz;
		tex1.uz = tex2.uz;
		tex2.uz = tempf;

		tempf = tex1.vz;
		tex1.vz = tex2.vz;
		tex2.vz = tempf;

		tempf = tex1.oz;
		tex1.oz = tex2.oz;
		tex2.oz = tempf;

		temp = fog1;
		fog1 = fog2;
		fog2 = temp;
	}

	Color.r = color1.r;
	Color.g = color1.g;
	Color.b = color1.b;

	dColor.r = (color2.r - color1.r) / (x2 - x1);
	dColor.g = (color2.g - color1.g) / (x2 - x1);
	dColor.b = (color2.b - color1.b) / (x2 - x1);
	
	Tex.uz = tex1.uz;
	Tex.vz = tex1.vz;
	Tex.oz = tex1.oz;

	dTex.uz = (tex2.uz - tex1.uz) / (x2 - x1);
	dTex.vz = (tex2.vz - tex1.vz) / (x2 - x1);
	dTex.oz = (tex2.oz - tex1.oz) / (x2 - x1);

	Fog = fog1;
	dFog = (fog2 - fog1) / (x2 - x1);
	
	if ( x1 < 0 ) 
	{
		Color.r += dColor.r * (0 - x1);
		Color.g += dColor.g * (0 - x1);
		Color.b += dColor.b * (0 - x1);
		Tex.uz += dTex.uz * (0 - x1);
		Tex.vz += dTex.vz * (0 - x1);
		Tex.oz += dTex.oz * (0 - x1);
		Fog += dFog * (0 - x1);
		x1 = 0;
	}

	if ( x2 > 639 ) x2 = 639;
	
	off = x1 * 2 + y * scanline;
	temp = x2 * 2 + y * scanline;

	while ( off <= temp )
	{
		int su, sv;
		float z;

		z = 1 / Tex.oz;
		//su = (int)(Tex.uz * z);
		//sv = (int)(Tex.vz * z);
		_asm {
			fld		Tex.uz
			fmul	z
			fistp	su
			
			fld		Tex.vz
			fmul	z
			fistp	sv
		}

		toff = (su & (TextureWidth-1)) * 2 + (sv & (TextureHeight-1)) * 2 * TextureWidth;

		WORD TexColor = *((WORD *)(pTexture + toff));
		
		int TexB = (TexColor & 0x001F);
		int TexG = (TexColor & 0x07E0)>>5;
		int TexR = (TexColor & 0xF800)>>11;
		
		int VR = Color.r >> 19;
		int VG = Color.g >> 18;
		int VB = Color.b >> 19;

		int MR = (VR * TexR)>>5;
		int MG = (VG * TexG)>>6;
		int MB = (VB * TexB)>>5;
		
		//WORD PColor = (MR << 11) | (MG << 5) | MB;
		
		int FF = Fog >> 16;
		int R = ((FF * (FogColorR - MR))>>8) + MR;
		int G = ((FF * (FogColorG - MG))>>8) + MG;
		int B = ((FF * (FogColorB - MB))>>8) + MB;

		WORD PColor = (R << 11) | (G << 5) | B;
		
		//TexColor &= 0xF7DE; //1111 0111 1101 1110b
		//TexColor >>= 1;
		
		//WORD VColor = (BYTE)(Color.r>>20);//16 + 3 + 1
		//VColor <<= 11;
		
		//WORD VColorTemp = (BYTE)(Color.g>>19);//16 + 2 + 1
		//VColor |= VColorTemp << 5;
		
		//VColor |= (BYTE)(Color.b>>20);//16 + 3 + 1
		
		//VColor += TexColor;

		*((WORD*)(Bits + off)) = PColor;

		//Bits[off + 0] = (BYTE)((Color.b>>17) + (pTexture[toff + 0]>>1));
		//Bits[off + 1] = (BYTE)((Color.g>>17) + (pTexture[toff + 1]>>1));
		//Bits[off + 2] = (BYTE)((Color.r>>17) + (pTexture[toff + 2]>>1));
		
		Color.r += dColor.r;
		Color.g += dColor.g;
		Color.b += dColor.b;
		
		Tex.uz += dTex.uz;		
		Tex.vz += dTex.vz;		
		Tex.oz += dTex.oz;		

		Fog += dFog;		

		off += 2;
	}

}

void HLineGouraudPerspectTexturef16(int x1, int x2, int y, COLORi24 color1, COLORi24 color2, TEXCOORDFLOAT tex1, TEXCOORDFLOAT tex2)
{
	int temp;
	float tempf;
	register int off, Eoff;
	UINT toff;
	COLORi24 Color, dColor;
	TEXCOORDFLOAT Tex, dTex;

	if (x1 == x2) return;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	
		temp = color1.r;
		color1.r = color2.r;
		color2.r = temp;

		temp = color1.g;
		color1.g = color2.g;
		color2.g = temp;

		temp = color1.b;
		color1.b = color2.b;
		color2.b = temp;
		
		tempf = tex1.uz;
		tex1.uz = tex2.uz;
		tex2.uz = tempf;

		tempf = tex1.vz;
		tex1.vz = tex2.vz;
		tex2.vz = tempf;

		tempf = tex1.oz;
		tex1.oz = tex2.oz;
		tex2.oz = tempf;
	}

	Color.r = color1.r;
	Color.g = color1.g;
	Color.b = color1.b;

	dColor.r = (color2.r - color1.r) / (x2 - x1);
	dColor.g = (color2.g - color1.g) / (x2 - x1);
	dColor.b = (color2.b - color1.b) / (x2 - x1);
	
	Tex.uz = tex1.uz;
	Tex.vz = tex1.vz;
	Tex.oz = tex1.oz;

	dTex.uz = (tex2.uz - tex1.uz) / (x2 - x1);
	dTex.vz = (tex2.vz - tex1.vz) / (x2 - x1);
	dTex.oz = (tex2.oz - tex1.oz) / (x2 - x1);
	
	if ( x1 < 0 ) 
	{
		Color.r += dColor.r * (0 - x1);
		Color.g += dColor.g * (0 - x1);
		Color.b += dColor.b * (0 - x1);
		Tex.uz += dTex.uz * (0 - x1);
		Tex.vz += dTex.vz * (0 - x1);
		Tex.oz += dTex.oz * (0 - x1);
		x1 = 0;
	}

	if ( x2 > 639 ) x2 = 639;

	int spancount, spanmod;

	spancount = (x2 - x1) / 16;
	spanmod = (x2 - x1) % 16;
	
	float z1, z2;
	int su1, sv1, su, sv;
	int su2, sv2;
	int dsu, dsv;

	int mw = TextureWidth - 1;
	int mh = TextureHeight - 1;
	int SpanlineBytes = TextureWidth * 2;

	for ( int i = 0; i < spancount; i++ )
	{
		z1 = 1 / Tex.oz;
		su1 = (int)(Tex.uz * z1 * 65536 + 32767);
		sv1 = (int)(Tex.vz * z1 * 65536 + 32767);

		z2 = 1 / (Tex.oz + dTex.oz * 16);
		su2 = (int)((Tex.uz + dTex.uz * 16) * z2 * 65536 + 32767);
		sv2 = (int)((Tex.vz + dTex.vz * 16) * z2 * 65536 + 32767);

		dsu = (su2 - su1) / 16;
		dsv = (sv2 - sv1) / 16;

		su = su1; sv = sv1;

		off = x1 * 2 + y * scanline + i * 16 * 2;
		Eoff = x1 * 2 + y * scanline + (i+1) * 16 * 2;
		
		while ( off <= Eoff )
		{
			toff = ((su>>16) & mw) * 2 + ((sv>>16) & mh) * SpanlineBytes;
		
			WORD TexColor = *((WORD *)(pTexture + toff));
			TexColor &= 0xF7DE; //1111 0111 1101 1110b
			TexColor >>= 1;
		
			WORD VColor = (BYTE)(Color.r>>20);//16 + 3 + 1
			VColor <<= 11;
		
			WORD VColorTemp = (BYTE)(Color.g>>19);//16 + 2 + 1
			VColor |= VColorTemp << 5;
		
			VColor |= (BYTE)(Color.b>>20);//16 + 3 + 1
		
			VColor += TexColor;

			*((WORD*)(Bits + off)) = VColor;

			//Bits[off + 0] = (BYTE)((Color.b>>17) + (pTexture[toff + 0]>>1));
			//Bits[off + 1] = (BYTE)((Color.g>>17) + (pTexture[toff + 1]>>1));
			//Bits[off + 2] = (BYTE)((Color.r>>17) + (pTexture[toff + 2]>>1));
		
			Color.r += dColor.r;
			Color.g += dColor.g;
			Color.b += dColor.b;

			su += dsu;
			sv += dsv;

			off += 2;
		}

		Tex.uz += dTex.uz * 16;		
		Tex.vz += dTex.vz * 16;		
		Tex.oz += dTex.oz * 16;		

	}

	if ( spanmod == 0 ) return;

	z1 = 1 / Tex.oz;
	su1 = (int)(Tex.uz * z1 * 65536 + 32767);
	sv1 = (int)(Tex.vz * z1 * 65536 + 32767);

	z2 = 1 / (Tex.oz + dTex.oz * spanmod);
	su2 = (int)((Tex.uz + dTex.u * spanmod) * z2 * 65536 + 32767);
	sv2 = (int)((Tex.vz + dTex.v * spanmod) * z2 * 65536 + 32767);

	dsu = (su2 - su1) / spanmod;
	dsv = (sv2 - sv1) / spanmod;

	su = su1; sv = sv1;
	
	off = x1 * 2 + y * scanline + spancount * 16 * 2;
	Eoff = x1 * 2 + y * scanline + (spancount * 16 + spanmod) * 2;
		
	while ( off <= Eoff )
	{
		
		toff = ((su>>16) & mw) * 2 + ((sv>>16) & mh) * SpanlineBytes;

		WORD TexColor = *((WORD *)(pTexture + toff));
		TexColor &= 0xF7DE; //1111 0111 1101 1110b
		TexColor >>= 1;
		
		WORD VColor = (BYTE)(Color.r>>20);//16 + 3 + 1
		VColor <<= 11;
		
		WORD VColorTemp = (BYTE)(Color.g>>19);//16 + 2 + 1
		VColor |= VColorTemp << 5;
		
		VColor |= (BYTE)(Color.b>>20);//16 + 3 + 1
		
		VColor += TexColor;

		*((WORD*)(Bits + off)) = VColor;

		//Bits[off + 0] = (BYTE)((Color.b>>17) + (pTexture[toff + 0]>>1));
		//Bits[off + 1] = (BYTE)((Color.g>>17) + (pTexture[toff + 1]>>1));
		//Bits[off + 2] = (BYTE)((Color.r>>17) + (pTexture[toff + 2]>>1));
		
		Color.r += dColor.r;
		Color.g += dColor.g;
		Color.b += dColor.b;

		su += dsu;
		sv += dsv;

		off += 2;
	}

}

void HLinePerspectTexturef16(int x1, int x2, int y, TEXCOORDFLOAT tex1, TEXCOORDFLOAT tex2)
{
	int temp;
	float tempf;
	register int off, Eoff;
	UINT toff;
	TEXCOORDFLOAT Tex, dTex;

	if (x1 == x2) return;

	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	
		tempf = tex1.uz;
		tex1.uz = tex2.uz;
		tex2.uz = tempf;

		tempf = tex1.vz;
		tex1.vz = tex2.vz;
		tex2.vz = tempf;

		tempf = tex1.oz;
		tex1.oz = tex2.oz;
		tex2.oz = tempf;
	}

	Tex.uz = tex1.uz;
	Tex.vz = tex1.vz;
	Tex.oz = tex1.oz;

	dTex.uz = (tex2.uz - tex1.uz) / (x2 - x1);
	dTex.vz = (tex2.vz - tex1.vz) / (x2 - x1);
	dTex.oz = (tex2.oz - tex1.oz) / (x2 - x1);
	
	if ( x1 < 0 ) 
	{
		Tex.uz += dTex.uz * (0 - x1);
		Tex.vz += dTex.vz * (0 - x1);
		Tex.oz += dTex.oz * (0 - x1);
		x1 = 0;
	}

	if ( x2 > 639 ) x2 = 639;

	int spancount, spanmod;

	spancount = (x2 - x1) / 16;
	spanmod = (x2 - x1) % 16;
	
	float z1, z2;
	int su1, sv1, su, sv;
	int su2, sv2;
	int dsu, dsv;

	int mw = TextureWidth - 1;
	int mh = TextureHeight - 1;
	int SpanlineBytes = TextureWidth * 2;

	for ( int i = 0; i < spancount; i++ )
	{
		z1 = 1 / Tex.oz;
		su1 = (int)(Tex.uz * z1 * 65536 + 32767);
		sv1 = (int)(Tex.vz * z1 * 65536 + 32767);

		z2 = 1 / (Tex.oz + dTex.oz * 16);
		su2 = (int)((Tex.uz + dTex.uz * 16) * z2 * 65536 + 32767);
		sv2 = (int)((Tex.vz + dTex.vz * 16) * z2 * 65536 + 32767);

		dsu = (su2 - su1) / 16;
		dsv = (sv2 - sv1) / 16;

		su = su1; sv = sv1;
		
		off = x1 * 2 + y * scanline + i * 16 * 2;
		Eoff = x1 * 2 + y * scanline + (i+1) * 16 * 2;
/*		
		while ( off <= Eoff)
		{
			toff = ((su>>16) & mw) * 3 + ((sv>>16) & mh) * SpanlineBytes;

			Bits[off] = pTexture[toff];
			Bits[off + 1] = pTexture[toff + 1];
			Bits[off + 2] = pTexture[toff + 2];
		
			su += dsu;
			sv += dsv;

			off += 3;
		}
*/
		_asm {
			//mov		ecx, su
			mov		esi, pTexture
			mov		edi, Bits
			mov		edx, off
WHILE:		
			cmp		edx, Eoff
			jg		NEXT

			mov		eax, su
			sar		eax, 16
			and		eax, mw
			lea		eax, [eax * 2]
			
			mov		ebx, sv
			sar		ebx, 16
			and		ebx, mh

			lea		ebx, [ebx * 2]
			sal		ebx, 8//*256

			add		eax, ebx
			
			mov		cx, word ptr[esi + eax]		// cl = B, ch = G
			mov		word ptr[edi + edx], cx
			
			mov		ebx, dsu
			add		su, ebx
			
			mov		eax, dsv
			add		sv, eax

			add		edx, 2
			
			jmp		WHILE
NEXT:
		}
		
		Tex.uz += dTex.uz * 16;		
		Tex.vz += dTex.vz * 16;		
		Tex.oz += dTex.oz * 16;		

	}

	if ( spanmod == 0 ) return;

	z1 = 1 / Tex.oz;
	su1 = (int)(Tex.uz * z1 * 65536 + 32767);
	sv1 = (int)(Tex.vz * z1 * 65536 + 32767);

	z2 = 1 / (Tex.oz + dTex.oz * spanmod);
	su2 = (int)((Tex.uz + dTex.uz * spanmod) * z2 * 65536 + 32767);
	sv2 = (int)((Tex.vz + dTex.vz * spanmod) * z2 * 65536 + 32767);

	dsu = (su2 - su1) / spanmod;
	dsv = (sv2 - sv1) / spanmod;

	su = su1; sv = sv1;
	
	off = x1 * 2 + y * scanline + spancount * 16 * 2;
	Eoff = x1 * 2 + y * scanline + spancount * 16 * 2 + spanmod * 2;

	while ( off <= Eoff )
	{
		
		toff = ((su>>16) & mw) * 2 + ((sv>>16) & mh) * SpanlineBytes;

		Bits[off + 0] = pTexture[toff + 0];
		Bits[off + 1] = pTexture[toff + 1];
		//Bits[off + 2] = pTexture[toff + 2];
		
		su += dsu;
		sv += dsv;

		off += 2;
	}

}

void Line(int x1, int y1, int x2, int y2, char color)
{
	int delta_x, delta_y;
	int D, DincE, DincNE, i, dy_offset;
	UINT offset;

	if ( x1 <= x2 )
	{
		delta_x = x2 - x1;
		delta_y = y2 - y1;
		if ( abs(delta_y) <= delta_x )  // -1 <= m <= 1
		{
			D = 2 * abs(delta_y) - delta_x;
			DincE = 2 * abs(delta_y);
			DincNE = 2 * (abs(delta_y) - delta_x);

			i = x1;
			offset = x1 * 2 + y1 * scanline;
			if ( delta_y > 0 )
				dy_offset = scanline + 2;
			else
				dy_offset = 2 - scanline;

			*(Bits + offset) = color;
			*(Bits + offset + 1) = color;
			//*(Bits + offset + 2) = color;

			while ( i++ < x2 )
			{
				if ( D <= 0 )
				{
					D += DincE;
					offset += 2;
				}
				else
				{
					D += DincNE;
					offset += dy_offset;
				}

				*(Bits + offset) = color;
				*(Bits + offset + 1) = color;
				//*(Bits + offset + 2) = color;
			}

		}
		else
		{
			if ( y1 <= y2 )
			{
				delta_x = y2 - y1;
				delta_y = x2 - x1;

				D = 2 * abs(delta_y) - delta_x;
				DincE = 2 * abs(delta_y);
				DincNE = 2 * (abs(delta_y) - delta_x);

				i = y1;
				offset = x1 * 2 + y1 * scanline;
				if ( delta_y > 0 )
					dy_offset = scanline + 2;
				else
					dy_offset = scanline - 2;

				*(Bits + offset) = color;
				*(Bits + offset + 1) = color;
				//*(Bits + offset + 2) = color;

				while ( i++ < y2 )
				{
					if ( D <= 0 )
					{
						D += DincE;
						offset += scanline;
					}
					else
					{
						D += DincNE;
						offset += dy_offset;
					}
					*(Bits + offset) = color;
					*(Bits + offset + 1) = color;
					//*(Bits + offset + 2) = color;
				}
			}
			else // y2 < y1
			{
				delta_x = y1 - y2;
				delta_y = x1 - x2;

				D = 2 * abs(delta_y) - delta_x;
				DincE = 2 * abs(delta_y);
				DincNE = 2 * (abs(delta_y) - delta_x);

				i = y2;
				offset = x2 * 2 + y2 * scanline;
				if ( delta_y > 0 )
					dy_offset = scanline + 2;
				else
					dy_offset = scanline - 2 ;

				*(Bits + offset) = color;
				*(Bits + offset + 1) = color;
				//*(Bits + offset + 2) = color;

				while ( i++ < y1 )
				{
					if ( D <= 0 )
					{
						D += DincE;
						offset += scanline;
					}
					else
					{
						D += DincNE;
						offset += dy_offset;
					}
					*(Bits + offset) = color;
					*(Bits + offset + 1) = color;
					//*(Bits + offset + 2) = color;
				}
			}
		}
	}

	else  // x1 > x2

	{
		delta_x = x1 - x2;
		delta_y = y1 - y2;
		if ( abs(delta_y) <= delta_x )  // -1 <= m <= 1
		{
			D = 2 * abs(delta_y) - delta_x;
			DincE = 2 * abs(delta_y);
			DincNE = 2 * (abs(delta_y) - delta_x);

			i = x2;
			offset = x2 * 2 + y2 * scanline;
			if ( delta_y > 0 )
				dy_offset = scanline + 2;
			else
				dy_offset = 2 - scanline;

			*(Bits + offset) = color;
			*(Bits + offset + 1) = color;
			//*(Bits + offset + 2) = color;

			while ( i++ < x1 )
			{
				if ( D <= 0 )
				{
					D += DincE;
					offset += 2;
				}
				else
				{
					D += DincNE;
					offset += dy_offset;
				}

				*(Bits + offset) = color;
				*(Bits + offset + 1) = color;
				//*(Bits + offset + 2) = color;
			}

		}
		else
		{
			if ( y1 >= y2 )
			{
				delta_x = y1 - y2;
				delta_y = x1 - x2;

				D = 2 * abs(delta_y) - delta_x;
				DincE = 2 * abs(delta_y);
				DincNE = 2 * (abs(delta_y) - delta_x);

				i = y2;
				offset = x2 * 2 + y2 * scanline;

				if ( delta_y > 0 )
					dy_offset = scanline + 2;
				else
					dy_offset = scanline - 2;

				*(Bits + offset) = color;
				*(Bits + offset + 1) = color;
				//*(Bits + offset + 2) = color;

				while ( i++ < y1 )
				{
					if ( D <= 0 )
					{
						D += DincE;
						offset += scanline;
					}
					else
					{
						D += DincNE;
						offset += dy_offset;
					}
					*(Bits + offset) = color;
					*(Bits + offset + 1) = color;
					//*(Bits + offset + 2) = color;
				}
			}
			else

			{
				delta_x = y2 - y1;
				delta_y = x2 - x1;

				D = 2 * abs(delta_y) - delta_x;
				DincE = 2 * abs(delta_y);
				DincNE = 2 * (abs(delta_y) - delta_x);

				i = y1;
				offset = x1 * 2 + y1 * scanline;
				if ( delta_y > 0 )
					dy_offset = scanline + 2;
				else
					dy_offset = scanline - 2;

				*(Bits + offset) = color;
				*(Bits + offset + 1) = color;
				//*(Bits + offset + 2) = color;

				while ( i++ < y2 )
				{
					if ( D <= 0 )
					{
						D += DincE;
						offset += scanline;
					}
					else
					{
						D += DincNE;
						offset += dy_offset;
					}
					*(Bits + offset) = color;
					*(Bits + offset + 1) = color;
					//*(Bits + offset + 2) = color;
				}
			}

		}
	}
}
void DrawTri(TRISTRUCT * pTri, char color)
{
	LineClip((int)pTri->Vertex[0].x, (int)pTri->Vertex[0].y,
		(int)pTri->Vertex[1].x, (int)pTri->Vertex[1].y, color);

	LineClip((int)pTri->Vertex[1].x, (int)pTri->Vertex[1].y,
		(int)pTri->Vertex[2].x, (int)pTri->Vertex[2].y, color);
	
	LineClip((int)pTri->Vertex[2].x, (int)pTri->Vertex[2].y,
		(int)pTri->Vertex[0].x, (int)pTri->Vertex[0].y, color);
}

void FillTri(TRISTRUCT * pTri, COLORf24 color)
{
	POINT2D Top, Middle, Bottom;
	int xTB, xTMB;
	int xdTB, xdTM, xdMB;
	int i, TopIndex, MiddleIndex, BottomIndex;
	int y;

	// Sorting Triangle Vertex
	Top.x = (int)(pTri->Vertex[0].x);
	Top.y = (int)(pTri->Vertex[0].y);
	TopIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Top.y > (int)pTri->Vertex[i].y )
		{
			Top.x = (int)pTri->Vertex[i].x;
			Top.y = (int)pTri->Vertex[i].y;
			TopIndex = i;
		}
	}

	Bottom.x = (int)(pTri->Vertex[0].x);
	Bottom.y = (int)(pTri->Vertex[0].y);
	BottomIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Bottom.y < (int)pTri->Vertex[i].y )
		{
			Bottom.x = (int)pTri->Vertex[i].x;
			Bottom.y = (int)pTri->Vertex[i].y;
			BottomIndex = i;
		}
	}

	MiddleIndex = 3 - (TopIndex + BottomIndex);
	Middle.x = (int)(pTri->Vertex[MiddleIndex].x);
	Middle.y = (int)(pTri->Vertex[MiddleIndex].y);

	y = Top.y;

	xTB = Top.x<<16;
	xTMB = Top.x<<16;

	if (Bottom.y == Top.y) return;
	xdTB = ((Bottom.x - Top.x)<<16) / (Bottom.y - Top.y);
	
	if (Middle.y == Top.y) { xTMB = Middle.x<<16; goto MIDDLETOBOTTOM; }
	xdTM = ((Middle.x - Top.x)<<16) / (Middle.y - Top.y);
	
	if ( Middle.y > 479 ) Middle.y = 479;

	if ( Top.y < 0 ) 
	{
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * ( 0 - Top.y));
		y = 0;
	}
	if ( Middle. y < 0 ) goto MIDDLETOBOTTOM;
	while( y < Middle.y )
	{
		//HLine( (xTB+32768)>>16, (xTMB + 32768 )>>16, y, color);
		HLine( (xTB)>>16, (xTMB + 32768)>>16, y, color);
		
		xTB += xdTB;
		xTMB += xdTM;

		y++;
	}

MIDDLETOBOTTOM:	

	if (Bottom.y == Middle.y) return;
	xdMB = ((Bottom.x - Middle.x)<<16) / (Bottom.y - Middle.y);
	if ( Bottom.y > 479 ) Bottom.y = 479;
	
	if ( Middle.y < 0 ) 
	{
		xTB = Top.x <<16;
		if ( Middle.y != Top.y ) xTMB = Top.x <<16;
		else xTMB = Middle.x <<16;
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * (Middle.y - Top.y));
		xTMB += (xdMB * (0 - Middle.y));
		y = 0;
	}
	while( y < Bottom.y )
	{
		//HLine( (xTB+32768)>>16, (xTMB + 32768)>>16, y, color);
		HLine( (xTB)>>16, (xTMB)>>16, y, color);
		
		xTB += xdTB;
		xTMB += xdMB;

		y++;
	}
}

void FillTriGouraud(TRISTRUCT *pTri, TRIANGLE *pTriangle)
{
	POINT2D Top, Middle, Bottom;
	int xTB, xTMB;
	int xdTB, xdTM, xdMB;
	int i, TopIndex, MiddleIndex, BottomIndex;
	int y;

	int TopColorR, TopColorG, TopColorB;
	int MiddleColorR, MiddleColorG, MiddleColorB; 
	int BottomColorR, BottomColorG, BottomColorB;
	
	COLORi24 cTB, cTMB;
	COLORi24 cdTB, cdTM, cdMB;
	
	// Sorting Triangle Vertex
	Top.x = (int)pTri->Vertex[0].x;
	Top.y = (int)pTri->Vertex[0].y;
	TopIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Top.y > (int)pTri->Vertex[i].y )
		{
			Top.x = (int)pTri->Vertex[i].x;
			Top.y = (int)pTri->Vertex[i].y;
			TopIndex = i;
		}
	}

	Bottom.x = (int)pTri->Vertex[0].x;
	Bottom.y = (int)pTri->Vertex[0].y;
	BottomIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Bottom.y < (int)pTri->Vertex[i].y )
		{
			Bottom.x = (int)pTri->Vertex[i].x;
			Bottom.y = (int)pTri->Vertex[i].y;
			BottomIndex = i;
		}
	}

	MiddleIndex = 3 - (TopIndex + BottomIndex);
	Middle.x = (int)pTri->Vertex[MiddleIndex].x;
	Middle.y = (int)pTri->Vertex[MiddleIndex].y;

	// Vertex Color
	TopColorR = (int)pTriangle->VertexColor[TopIndex].r;
	TopColorG = (int)pTriangle->VertexColor[TopIndex].g;
	TopColorB = (int)pTriangle->VertexColor[TopIndex].b;

	MiddleColorR = (int)pTriangle->VertexColor[MiddleIndex].r;
	MiddleColorG = (int)pTriangle->VertexColor[MiddleIndex].g;
	MiddleColorB = (int)pTriangle->VertexColor[MiddleIndex].b;

	BottomColorR = (int)pTriangle->VertexColor[BottomIndex].r;
	BottomColorG = (int)pTriangle->VertexColor[BottomIndex].g;
	BottomColorB = (int)pTriangle->VertexColor[BottomIndex].b;

	y = Top.y;

	xTB = Top.x<<16;
	xTMB = Top.x<<16;

	cTB.r = TopColorR<<16;
	cTB.g = TopColorG<<16;
	cTB.b = TopColorB<<16;
	cTMB.r = TopColorR<<16;
	cTMB.g = TopColorG<<16;
	cTMB.b = TopColorB<<16;

	if (Bottom.y == Top.y) return;
	xdTB = ((Bottom.x - Top.x)<<16) / (Bottom.y - Top.y);
	cdTB.r = ((BottomColorR - TopColorR)<<16) / (Bottom.y - Top.y);
	cdTB.g = ((BottomColorG - TopColorG)<<16) / (Bottom.y - Top.y);
	cdTB.b = ((BottomColorB - TopColorB)<<16) / (Bottom.y - Top.y);
	
	if (Middle.y == Top.y) 
	{ 
		xTMB = Middle.x<<16;
		cTMB.r = MiddleColorR<<16; 
		cTMB.g = MiddleColorG<<16; 
		cTMB.b = MiddleColorB<<16; 
		goto MIDDLETOBOTTOM; 
	}
	xdTM = ((Middle.x - Top.x)<<16) / (Middle.y - Top.y);
	cdTM.r = ((MiddleColorR - TopColorR)<<16) / (Middle.y - Top.y);
	cdTM.g = ((MiddleColorG - TopColorG)<<16) / (Middle.y - Top.y);
	cdTM.b = ((MiddleColorB - TopColorB)<<16) / (Middle.y - Top.y);
	
	if ( Middle.y > 479 ) Middle.y = 479;

	if ( Top.y < 0 ) 
	{
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * ( 0 - Top.y));
		
		cTB.r += (cdTB.r * (0 - Top.y));
		cTB.g += (cdTB.g * (0 - Top.y));
		cTB.b += (cdTB.b * (0 - Top.y));
		cTMB.r += (cdTM.r * ( 0 - Top.y));
		cTMB.g += (cdTM.g * ( 0 - Top.y));
		cTMB.b += (cdTM.b * ( 0 - Top.y));
		
		y = 0;
	}
	if ( Middle. y < 0 ) goto MIDDLETOBOTTOM;
	while( y < Middle.y )
	{
		
		//HLineGouraud( (xTB+32768)>>16, (xTMB + 32768 )>>16, y, cTB, cTMB);
		HLineGouraud( (xTB)>>16, (xTMB)>>16, y, cTB, cTMB);
		
		xTB += xdTB;
		xTMB += xdTM;
		
		cTB.r += cdTB.r;
		cTB.g += cdTB.g;
		cTB.b += cdTB.b;
		cTMB.r += cdTM.r;
		cTMB.g += cdTM.g;
		cTMB.b += cdTM.b;

		y++;
	}

MIDDLETOBOTTOM:	

	if (Bottom.y == Middle.y) return;
	xdMB = ((Bottom.x - Middle.x)<<16) / (Bottom.y - Middle.y);
	cdMB.r = ((BottomColorR - MiddleColorR)<<16) / (Bottom.y - Middle.y);
	cdMB.g = ((BottomColorG - MiddleColorG)<<16) / (Bottom.y - Middle.y);
	cdMB.b = ((BottomColorB - MiddleColorB)<<16) / (Bottom.y - Middle.y);
	if ( Bottom.y > 479 ) Bottom.y = 479;
	
	if ( Middle.y < 0 ) 
	{
		xTB = Top.x <<16;
		cTB.r = TopColorR <<16;
		cTB.g = TopColorG <<16;
		cTB.b = TopColorB <<16;
		if ( Middle.y != Top.y ) 
		{
			xTMB = Top.x <<16;
			cTMB.r = TopColorR <<16;
			cTMB.g = TopColorG <<16;
			cTMB.b = TopColorB <<16;
		}
		else
		{ 
			xTMB = Middle.x <<16;
			cTMB.r = MiddleColorR <<16;
			cTMB.g = MiddleColorG <<16;
			cTMB.b = MiddleColorB <<16;
		}
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * (Middle.y - Top.y));
		xTMB += (xdMB * (0 - Middle.y));
		
		cTB.r += (cdTB.r * (0 - Top.y));
		cTB.g += (cdTB.g * (0 - Top.y));
		cTB.b += (cdTB.b * (0 - Top.y));
		cTMB.r += (cdTM.r * (Middle.y - Top.y));
		cTMB.g += (cdTM.g * (Middle.y - Top.y));
		cTMB.b += (cdTM.b * (Middle.y - Top.y));
		cTMB.r += (cdMB.r * (0 - Middle.y));
		cTMB.g += (cdMB.g * (0 - Middle.y));
		cTMB.b += (cdMB.b * (0 - Middle.y));
		
		y = 0;
	}
	while( y < Bottom.y )
	{
		//HLineGouraud( (xTB+32768)>>16, (xTMB + 32768)>>16, y, cTB, cTMB);
		HLineGouraud( (xTB)>>16, (xTMB)>>16, y, cTB, cTMB);
		
		xTB += xdTB;
		xTMB += xdMB;
		
		cTB.r += cdTB.r;
		cTB.g += cdTB.g;
		cTB.b += cdTB.b;
		cTMB.r += cdMB.r;
		cTMB.g += cdMB.g;
		cTMB.b += cdMB.b;

		y++;
	}
}

void FillTriGouraudTexture(TRISTRUCT *pTri, TRIANGLE *pTriangle)
{
	POINT2D Top, Middle, Bottom;
	int xTB, xTMB;
	int xdTB, xdTM, xdMB;
	int i, TopIndex, MiddleIndex, BottomIndex;
	int y;

	int TopColorR, TopColorG, TopColorB;
	int MiddleColorR, MiddleColorG, MiddleColorB; 
	int BottomColorR, BottomColorG, BottomColorB;
	
	int TopU, TopV;
	int MiddleU, MiddleV; 
	int BottomU, BottomV;
	
	COLORi24 cTB, cTMB;
	COLORi24 cdTB, cdTM, cdMB;

	TEXCOORD tTB, tTMB;
	TEXCOORD tdTB, tdTM, tdMB;
	
	// Sorting Triangle Vertex
	Top.x = (int)pTri->Vertex[0].x;
	Top.y = (int)pTri->Vertex[0].y;
	TopIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Top.y > (int)pTri->Vertex[i].y )
		{
			Top.x = (int)pTri->Vertex[i].x;
			Top.y = (int)pTri->Vertex[i].y;
			TopIndex = i;
		}
	}

	Bottom.x = (int)pTri->Vertex[0].x;
	Bottom.y = (int)pTri->Vertex[0].y;
	BottomIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Bottom.y < (int)pTri->Vertex[i].y )
		{
			Bottom.x = (int)pTri->Vertex[i].x;
			Bottom.y = (int)pTri->Vertex[i].y;
			BottomIndex = i;
		}
	}

	MiddleIndex = 3 - (TopIndex + BottomIndex);
	Middle.x = (int)pTri->Vertex[MiddleIndex].x;
	Middle.y = (int)pTri->Vertex[MiddleIndex].y;

	// Vertex Color
	TopColorR = (int)pTriangle->VertexColor[TopIndex].r;
	TopColorG = (int)pTriangle->VertexColor[TopIndex].g;
	TopColorB = (int)pTriangle->VertexColor[TopIndex].b;

	MiddleColorR = (int)pTriangle->VertexColor[MiddleIndex].r;
	MiddleColorG = (int)pTriangle->VertexColor[MiddleIndex].g;
	MiddleColorB = (int)pTriangle->VertexColor[MiddleIndex].b;

	BottomColorR = (int)pTriangle->VertexColor[BottomIndex].r;
	BottomColorG = (int)pTriangle->VertexColor[BottomIndex].g;
	BottomColorB = (int)pTriangle->VertexColor[BottomIndex].b;

	// Texture coordinate
	TopU = (int)(pTriangle->VertexTexture[TopIndex].u * (TextureWidth-1));
	TopV = (int)(pTriangle->VertexTexture[TopIndex].v * (TextureHeight-1));

	MiddleU = (int)(pTriangle->VertexTexture[MiddleIndex].u * (TextureWidth-1));
	MiddleV = (int)(pTriangle->VertexTexture[MiddleIndex].v * (TextureHeight-1));

	BottomU = (int)(pTriangle->VertexTexture[BottomIndex].u * (TextureWidth-1));
	BottomV = (int)(pTriangle->VertexTexture[BottomIndex].v * (TextureHeight-1));

	y = Top.y;

	xTB = Top.x<<16;
	xTMB = Top.x<<16;

	cTB.r = TopColorR<<16;
	cTB.g = TopColorG<<16;
	cTB.b = TopColorB<<16;
	cTMB.r = TopColorR<<16;
	cTMB.g = TopColorG<<16;
	cTMB.b = TopColorB<<16;

	tTB.u = TopU<<16;
	tTB.v = TopV<<16;
	tTMB.u = TopU<<16;
	tTMB.v = TopV<<16;

	if (Bottom.y == Top.y) return;
	xdTB = ((Bottom.x - Top.x)<<16) / (Bottom.y - Top.y);
	cdTB.r = ((BottomColorR - TopColorR)<<16) / (Bottom.y - Top.y);
	cdTB.g = ((BottomColorG - TopColorG)<<16) / (Bottom.y - Top.y);
	cdTB.b = ((BottomColorB - TopColorB)<<16) / (Bottom.y - Top.y);
	tdTB.u = ((BottomU - TopU)<<16) / (Bottom.y - Top.y);
	tdTB.v = ((BottomV - TopV)<<16) / (Bottom.y - Top.y);
	
	if (Middle.y == Top.y) 
	{ 
		xTMB = Middle.x<<16;
		cTMB.r = MiddleColorR<<16; 
		cTMB.g = MiddleColorG<<16; 
		cTMB.b = MiddleColorB<<16; 
		//tTMB.u = MiddleU<<16; 
		//tTMB.v = MiddleV<<16; 
		tTMB.u = MiddleU<<16; 
		tTMB.v = MiddleV<<16; 
		goto MIDDLETOBOTTOM; 
	}
	xdTM = ((Middle.x - Top.x)<<16) / (Middle.y - Top.y);
	cdTM.r = ((MiddleColorR - TopColorR)<<16) / (Middle.y - Top.y);
	cdTM.g = ((MiddleColorG - TopColorG)<<16) / (Middle.y - Top.y);
	cdTM.b = ((MiddleColorB - TopColorB)<<16) / (Middle.y - Top.y);
	tdTM.u = ((MiddleU - TopU)<<16) / (Middle.y - Top.y);
	tdTM.v = ((MiddleV - TopV)<<16) / (Middle.y - Top.y);
	
	if ( Middle.y > 479 ) Middle.y = 479;

	if ( Top.y < 0 ) 
	{
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * ( 0 - Top.y));
		
		cTB.r += (cdTB.r * (0 - Top.y));
		cTB.g += (cdTB.g * (0 - Top.y));
		cTB.b += (cdTB.b * (0 - Top.y));
		cTMB.r += (cdTM.r * ( 0 - Top.y));
		cTMB.g += (cdTM.g * ( 0 - Top.y));
		cTMB.b += (cdTM.b * ( 0 - Top.y));
		tTB.u += (tdTB.u * (0 - Top.y));
		tTB.v += (tdTB.v * (0 - Top.y));
		tTMB.u += (tdTM.u * ( 0 - Top.y));
		tTMB.v += (tdTM.v * ( 0 - Top.y));
		
		y = 0;
	}
	if ( Middle. y < 0 ) goto MIDDLETOBOTTOM;
	while( y < Middle.y )
	{
		
		//HLineGouraudTexture( (xTB+32768)>>16, (xTMB + 32768 )>>16, y, cTB, cTMB, tTB, tTMB);
		HLineGouraudTexture( (xTB)>>16, (xTMB)>>16, y, cTB, cTMB, tTB, tTMB);
		
		xTB += xdTB;
		xTMB += xdTM;
		
		cTB.r += cdTB.r;
		cTB.g += cdTB.g;
		cTB.b += cdTB.b;
		cTMB.r += cdTM.r;
		cTMB.g += cdTM.g;
		cTMB.b += cdTM.b;
		tTB.u += tdTB.u;
		tTB.v += tdTB.v;
		tTMB.u += tdTM.u;
		tTMB.v += tdTM.v;

		y++;
	}

MIDDLETOBOTTOM:	

	if (Bottom.y == Middle.y) return;
	xdMB = ((Bottom.x - Middle.x)<<16) / (Bottom.y - Middle.y);
	cdMB.r = ((BottomColorR - MiddleColorR)<<16) / (Bottom.y - Middle.y);
	cdMB.g = ((BottomColorG - MiddleColorG)<<16) / (Bottom.y - Middle.y);
	cdMB.b = ((BottomColorB - MiddleColorB)<<16) / (Bottom.y - Middle.y);
	tdMB.u = ((BottomU - MiddleU)<<16) / (Bottom.y - Middle.y);
	tdMB.v = ((BottomV - MiddleV)<<16) / (Bottom.y - Middle.y);
	if ( Bottom.y > 479 ) Bottom.y = 479;
	
	if ( Middle.y < 0 ) 
	{
		xTB = Top.x <<16;
		cTB.r = TopColorR <<16;
		cTB.g = TopColorG <<16;
		cTB.b = TopColorB <<16;
		tTB.u = TopU <<16;
		tTB.v = TopV <<16;
		if ( Middle.y != Top.y ) 
		{
			xTMB = Top.x <<16;
			cTMB.r = TopColorR <<16;
			cTMB.g = TopColorG <<16;
			cTMB.b = TopColorB <<16;
			tTMB.u = TopU <<16;
			tTMB.v = TopV <<16;
		}
		else
		{ 
			xTMB = Middle.x <<16;
			cTMB.r = MiddleColorR <<16;
			cTMB.g = MiddleColorG <<16;
			cTMB.b = MiddleColorB <<16;
			tTMB.u = MiddleU <<16;
			tTMB.v = MiddleV <<16;
		}
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * (Middle.y - Top.y));
		xTMB += (xdMB * (0 - Middle.y));
		
		cTB.r += (cdTB.r * (0 - Top.y));
		cTB.g += (cdTB.g * (0 - Top.y));
		cTB.b += (cdTB.b * (0 - Top.y));
		cTMB.r += (cdTM.r * (Middle.y - Top.y));
		cTMB.g += (cdTM.g * (Middle.y - Top.y));
		cTMB.b += (cdTM.b * (Middle.y - Top.y));
		cTMB.r += (cdMB.r * (0 - Middle.y));
		cTMB.g += (cdMB.g * (0 - Middle.y));
		cTMB.b += (cdMB.b * (0 - Middle.y));
		tTB.u += (tdTB.u * (0 - Top.y));
		tTB.v += (tdTB.v * (0 - Top.y));
		tTMB.u += (tdTM.u * (Middle.y - Top.y));
		tTMB.v += (tdTM.v * (Middle.y - Top.y));
		tTMB.u += (tdMB.u * (0 - Middle.y));
		tTMB.v += (tdMB.v * (0 - Middle.y));
		
		y = 0;
	}
	while( y < Bottom.y )
	{
		//HLineGouraudTexture( (xTB+32768)>>16, (xTMB + 32768)>>16, y, cTB, cTMB, tTB, tTMB);
		HLineGouraudTexture( (xTB)>>16, (xTMB)>>16, y, cTB, cTMB, tTB, tTMB);
		
		xTB += xdTB;
		xTMB += xdMB;
		
		cTB.r += cdTB.r;
		cTB.g += cdTB.g;
		cTB.b += cdTB.b;
		cTMB.r += cdMB.r;
		cTMB.g += cdMB.g;
		cTMB.b += cdMB.b;
		tTB.u += tdTB.u;
		tTB.v += tdTB.v;
		tTMB.u += tdMB.u;
		tTMB.v += tdMB.v;

		y++;
	}
}

void FillTriGouraudPerspectTexturef(TRISTRUCT *pTri, TRIANGLE *pTriangle)
{
	POINT2D Top, Middle, Bottom;
	int xTB, xTMB;
	int xdTB, xdTM, xdMB;
	int i, TopIndex, MiddleIndex, BottomIndex;
	int y;

	int TopColorR, TopColorG, TopColorB;
	int MiddleColorR, MiddleColorG, MiddleColorB; 
	int BottomColorR, BottomColorG, BottomColorB;
	
	int TopFog, MiddleFog, BottomFog;
	int fTB, fTMB;
	int fdTB, fdTM, fdMB;
	
	float TopZ, TopU, TopV;
	float MiddleZ, MiddleU, MiddleV; 
	float BottomZ, BottomU, BottomV;
	
	float TopOZ, TopUZ, TopVZ;
	float MiddleOZ, MiddleUZ, MiddleVZ; 
	float BottomOZ, BottomUZ, BottomVZ;

	COLORi24 cTB, cTMB;
	COLORi24 cdTB, cdTM, cdMB;

	TEXCOORDFLOAT tTB, tTMB;
	TEXCOORDFLOAT tdTB, tdTM, tdMB;
	
	// Sorting Triangle Vertex
	Top.x = (int)(pTri->Vertex[0].x * 65536);
	Top.y = (int)(pTri->Vertex[0].y);
	TopIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Top.y > (int)pTri->Vertex[i].y )
		{
			Top.x = (int)(pTri->Vertex[i].x * 65536);
			Top.y = (int)(pTri->Vertex[i].y);
			TopIndex = i;
		}
	}

	Bottom.x = (int)(pTri->Vertex[0].x * 65536);
	Bottom.y = (int)(pTri->Vertex[0].y);
	BottomIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Bottom.y < (int)pTri->Vertex[i].y )
		{
			Bottom.x = (int)(pTri->Vertex[i].x * 65536);
			Bottom.y = (int)(pTri->Vertex[i].y);
			BottomIndex = i;
		}
	}
	
	if (TopIndex == BottomIndex) return;
	MiddleIndex = 3 - (TopIndex + BottomIndex);
	Middle.x = (int)(pTri->Vertex[MiddleIndex].x * 65536);
	Middle.y = (int)(pTri->Vertex[MiddleIndex].y);

	// Vertex Color
	TopColorR = (int)pTriangle->VertexColor[TopIndex].r;
	TopColorG = (int)pTriangle->VertexColor[TopIndex].g;
	TopColorB = (int)pTriangle->VertexColor[TopIndex].b;

	MiddleColorR = (int)pTriangle->VertexColor[MiddleIndex].r;
	MiddleColorG = (int)pTriangle->VertexColor[MiddleIndex].g;
	MiddleColorB = (int)pTriangle->VertexColor[MiddleIndex].b;

	BottomColorR = (int)pTriangle->VertexColor[BottomIndex].r;
	BottomColorG = (int)pTriangle->VertexColor[BottomIndex].g;
	BottomColorB = (int)pTriangle->VertexColor[BottomIndex].b;

	//Fog Factor
	TopFog = (int)(ViewVertexList[pTriangle->VertexIndex[TopIndex]].fog_factor * 256);
	MiddleFog = (int)(ViewVertexList[pTriangle->VertexIndex[MiddleIndex]].fog_factor * 256);
	BottomFog = (int)(ViewVertexList[pTriangle->VertexIndex[BottomIndex]].fog_factor * 256);

	// Texture coordinate
	TopU = (pTriangle->VertexTexture[TopIndex].u * (TextureWidth-1));
	TopV = (pTriangle->VertexTexture[TopIndex].v * (TextureHeight-1));
	TopZ = pTri->Vertex[TopIndex].z;//(ScreenVertexList[pTriangle->VertexIndex[TopIndex]].z);

	MiddleU = (pTriangle->VertexTexture[MiddleIndex].u * (TextureWidth-1));
	MiddleV = (pTriangle->VertexTexture[MiddleIndex].v * (TextureHeight-1));
	MiddleZ = pTri->Vertex[MiddleIndex].z;//(ScreenVertexList[ pTriangle->VertexIndex[MiddleIndex] ].z);

	BottomU = (pTriangle->VertexTexture[BottomIndex].u * (TextureWidth-1));
	BottomV = (pTriangle->VertexTexture[BottomIndex].v * (TextureHeight-1));
	BottomZ = pTri->Vertex[BottomIndex].z;//(ScreenVertexList[pTriangle->VertexIndex[BottomIndex]].z);

	TopOZ = 1 / TopZ;
	MiddleOZ = 1 / MiddleZ;
	BottomOZ = 1 / BottomZ;

	TopUZ = (TopU) / TopZ;
	TopVZ = (TopV) / TopZ;

	MiddleUZ = (MiddleU) / MiddleZ;
	MiddleVZ = (MiddleV) / MiddleZ;

	BottomUZ = (BottomU) / BottomZ;
	BottomVZ = (BottomV) / BottomZ;

	y = Top.y;

	xTB = Top.x;
	xTMB = Top.x;

	cTB.r = TopColorR<<16;
	cTB.g = TopColorG<<16;
	cTB.b = TopColorB<<16;
	cTMB.r = TopColorR<<16;
	cTMB.g = TopColorG<<16;
	cTMB.b = TopColorB<<16;

	fTB = TopFog<<16;
	fTMB = TopFog<<16;
	
	tTB.uz = TopUZ;
	tTB.vz = TopVZ;
	tTB.oz = TopOZ;
	tTMB.uz = TopUZ;
	tTMB.vz = TopVZ;
	tTMB.oz = TopOZ;

	if (Bottom.y == Top.y) return;
	xdTB = (Bottom.x - Top.x) / (Bottom.y - Top.y);
	cdTB.r = ((BottomColorR - TopColorR)<<16) / (Bottom.y - Top.y);
	cdTB.g = ((BottomColorG - TopColorG)<<16) / (Bottom.y - Top.y);
	cdTB.b = ((BottomColorB - TopColorB)<<16) / (Bottom.y - Top.y);
	tdTB.uz = (BottomUZ - TopUZ) / (Bottom.y - Top.y);
	tdTB.vz = (BottomVZ - TopVZ) / (Bottom.y - Top.y);
	tdTB.oz = (BottomOZ - TopOZ) / (Bottom.y - Top.y);
	fdTB = ((BottomFog - TopFog)<<16) / (Bottom.y - Top.y);
	
	if (Middle.y == Top.y) 
	{ 
		xTMB = Middle.x;
		cTMB.r = MiddleColorR<<16; 
		cTMB.g = MiddleColorG<<16; 
		cTMB.b = MiddleColorB<<16; 
		tTMB.uz = MiddleUZ; 
		tTMB.vz = MiddleVZ; 
		tTMB.oz = MiddleOZ; 
		fTMB = MiddleFog<<16; 
		goto MIDDLETOBOTTOM; 
	}
	xdTM = (Middle.x - Top.x) / (Middle.y - Top.y);
	cdTM.r = ((MiddleColorR - TopColorR)<<16) / (Middle.y - Top.y);
	cdTM.g = ((MiddleColorG - TopColorG)<<16) / (Middle.y - Top.y);
	cdTM.b = ((MiddleColorB - TopColorB)<<16) / (Middle.y - Top.y);
	tdTM.uz = (MiddleUZ - TopUZ) / (Middle.y - Top.y);
	tdTM.vz = (MiddleVZ - TopVZ) / (Middle.y - Top.y);
	tdTM.oz = (MiddleOZ - TopOZ) / (Middle.y - Top.y);
	fdTM = ((MiddleFog - TopFog)<<16) / (Middle.y - Top.y);
	
	if ( Middle.y > 479 ) Middle.y = 479;

	if ( Top.y < 0 ) 
	{
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * ( 0 - Top.y));
		
		cTB.r += (cdTB.r * (0 - Top.y));
		cTB.g += (cdTB.g * (0 - Top.y));
		cTB.b += (cdTB.b * (0 - Top.y));
		cTMB.r += (cdTM.r * ( 0 - Top.y));
		cTMB.g += (cdTM.g * ( 0 - Top.y));
		cTMB.b += (cdTM.b * ( 0 - Top.y));
		tTB.uz += (tdTB.uz * (0 - Top.y));
		tTB.vz += (tdTB.vz * (0 - Top.y));
		tTB.oz += (tdTB.oz * (0 - Top.y));
		tTMB.uz += (tdTM.uz * ( 0 - Top.y));
		tTMB.vz += (tdTM.vz * ( 0 - Top.y));
		tTMB.oz += (tdTM.oz * ( 0 - Top.y));
		fTB += (fdTB * ( 0 - Top.y));
		fTMB += (fdTM * ( 0 - Top.y));
		
		y = 0;
	}
	if ( Middle. y < 0 ) goto MIDDLETOBOTTOM;
	while( y < Middle.y )
	{
		
		HLineFogGouraudPerspectTexturef( xTB, xTMB, y, cTB, cTMB, tTB, tTMB, fTB, fTMB);
		
		xTB += xdTB;
		xTMB += xdTM;
		
		cTB.r += cdTB.r;
		cTB.g += cdTB.g;
		cTB.b += cdTB.b;
		cTMB.r += cdTM.r;
		cTMB.g += cdTM.g;
		cTMB.b += cdTM.b;
		tTB.uz += tdTB.uz;
		tTB.vz += tdTB.vz;
		tTB.oz += tdTB.oz;
		tTMB.uz += tdTM.uz;
		tTMB.vz += tdTM.vz;
		tTMB.oz += tdTM.oz;
		fTB += fdTB;
		fTMB += fdTM;
		
		y++;
	}

MIDDLETOBOTTOM:	

	if (Bottom.y == Middle.y) return;
	xdMB = (Bottom.x - Middle.x) / (Bottom.y - Middle.y);
	cdMB.r = ((BottomColorR - MiddleColorR)<<16) / (Bottom.y - Middle.y);
	cdMB.g = ((BottomColorG - MiddleColorG)<<16) / (Bottom.y - Middle.y);
	cdMB.b = ((BottomColorB - MiddleColorB)<<16) / (Bottom.y - Middle.y);
	tdMB.uz = (BottomUZ - MiddleUZ) / (Bottom.y - Middle.y);
	tdMB.vz = (BottomVZ - MiddleVZ) / (Bottom.y - Middle.y);
	tdMB.oz = (BottomOZ - MiddleOZ) / (Bottom.y - Middle.y);
	fdMB = ((BottomFog - MiddleFog)<<16) / (Bottom.y - Middle.y);
	if ( Bottom.y > 479 ) Bottom.y = 479;
	
	if ( Middle.y < 0 ) 
	{
		xTB = Top.x;
		cTB.r = TopColorR<<16;
		cTB.g = TopColorG<<16;
		cTB.b = TopColorB<<16;
		tTB.uz = TopUZ;
		tTB.vz = TopVZ;
		tTB.oz = TopOZ;
		fTB = TopFog<<16;
		if ( Middle.y != Top.y ) 
		{
			xTMB = Top.x;
			cTMB.r = TopColorR <<16;
			cTMB.g = TopColorG <<16;
			cTMB.b = TopColorB <<16;
			tTMB.uz = TopUZ;
			tTMB.vz = TopVZ;
			tTMB.oz = TopOZ;
			fTMB = TopFog<<16;
		}
		else
		{ 
			xTMB = Middle.x;
			cTMB.r = MiddleColorR <<16;
			cTMB.g = MiddleColorG <<16;
			cTMB.b = MiddleColorB <<16;
			tTMB.uz = MiddleUZ;
			tTMB.vz = MiddleVZ;
			tTMB.oz = MiddleOZ;
			fTMB = MiddleFog<<16;
		}
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * (Middle.y - Top.y));
		xTMB += (xdMB * (0 - Middle.y));
		
		cTB.r += (cdTB.r * (0 - Top.y));
		cTB.g += (cdTB.g * (0 - Top.y));
		cTB.b += (cdTB.b * (0 - Top.y));
		cTMB.r += (cdTM.r * (Middle.y - Top.y));
		cTMB.g += (cdTM.g * (Middle.y - Top.y));
		cTMB.b += (cdTM.b * (Middle.y - Top.y));
		cTMB.r += (cdMB.r * (0 - Middle.y));
		cTMB.g += (cdMB.g * (0 - Middle.y));
		cTMB.b += (cdMB.b * (0 - Middle.y));
		tTB.uz += (tdTB.uz * (0 - Top.y));
		tTB.vz += (tdTB.vz * (0 - Top.y));
		tTB.oz += (tdTB.oz * (0 - Top.y));
		tTMB.uz += (tdTM.uz * (Middle.y - Top.y));
		tTMB.vz += (tdTM.vz * (Middle.y - Top.y));
		tTMB.oz += (tdTM.oz * (Middle.y - Top.y));
		tTMB.uz += (tdMB.uz * (0 - Middle.y));
		tTMB.vz += (tdMB.vz * (0 - Middle.y));
		tTMB.oz += (tdMB.oz * (0 - Middle.y));
		fTB += (fdTB * (0 - Top.y));
		fTMB += (fdTM * (Middle.y - Top.y));
		fTMB += (fdMB * (0 - Middle.y));
		
		y = 0;
	}
	while( y < Bottom.y )
	{
		HLineFogGouraudPerspectTexturef( xTB, xTMB, y, cTB, cTMB, tTB, tTMB, fTB, fTMB);
		
		xTB += xdTB;
		xTMB += xdMB;
		
		cTB.r += cdTB.r;
		cTB.g += cdTB.g;
		cTB.b += cdTB.b;
		cTMB.r += cdMB.r;
		cTMB.g += cdMB.g;
		cTMB.b += cdMB.b;
		tTB.uz += tdTB.uz;
		tTB.vz += tdTB.vz;
		tTB.oz += tdTB.oz;
		tTMB.uz += tdMB.uz;
		tTMB.vz += tdMB.vz;
		tTMB.oz += tdMB.oz;
		fTB += fdTB;
		fTMB += fdMB;

		y++;
	}
}

void FillTriGouraudPerspectTexturef16(TRISTRUCT *pTri, TRIANGLE *pTriangle)
{
	POINT2D Top, Middle, Bottom;
	int xTB, xTMB;
	int xdTB, xdTM, xdMB;
	int i, TopIndex, MiddleIndex, BottomIndex;
	int y;

	int TopColorR, TopColorG, TopColorB;
	int MiddleColorR, MiddleColorG, MiddleColorB; 
	int BottomColorR, BottomColorG, BottomColorB;
	
	float TopZ, TopU, TopV;
	float MiddleZ, MiddleU, MiddleV; 
	float BottomZ, BottomU, BottomV;
	
	float TopOZ, TopUZ, TopVZ;
	float MiddleOZ, MiddleUZ, MiddleVZ; 
	float BottomOZ, BottomUZ, BottomVZ;

	COLORi24 cTB, cTMB;
	COLORi24 cdTB, cdTM, cdMB;

	TEXCOORDFLOAT tTB, tTMB;
	TEXCOORDFLOAT tdTB, tdTM, tdMB;
	
	// Sorting Triangle Vertex
	Top.x = (int)pTri->Vertex[0].x;
	Top.y = (int)pTri->Vertex[0].y;
	TopIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Top.y > (int)pTri->Vertex[i].y )
		{
			Top.x = (int)pTri->Vertex[i].x;
			Top.y = (int)pTri->Vertex[i].y;
			TopIndex = i;
		}
	}

	Bottom.x = (int)pTri->Vertex[0].x;
	Bottom.y = (int)pTri->Vertex[0].y;
	BottomIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Bottom.y < (int)pTri->Vertex[i].y )
		{
			Bottom.x = (int)pTri->Vertex[i].x;
			Bottom.y = (int)pTri->Vertex[i].y;
			BottomIndex = i;
		}
	}
	
	if (TopIndex == BottomIndex) return;
	MiddleIndex = 3 - (TopIndex + BottomIndex);
	Middle.x = (int)pTri->Vertex[MiddleIndex].x;
	Middle.y = (int)pTri->Vertex[MiddleIndex].y;

	// Vertex Color
	TopColorR = (int)pTriangle->VertexColor[TopIndex].r;
	TopColorG = (int)pTriangle->VertexColor[TopIndex].g;
	TopColorB = (int)pTriangle->VertexColor[TopIndex].b;

	MiddleColorR = (int)pTriangle->VertexColor[MiddleIndex].r;
	MiddleColorG = (int)pTriangle->VertexColor[MiddleIndex].g;
	MiddleColorB = (int)pTriangle->VertexColor[MiddleIndex].b;

	BottomColorR = (int)pTriangle->VertexColor[BottomIndex].r;
	BottomColorG = (int)pTriangle->VertexColor[BottomIndex].g;
	BottomColorB = (int)pTriangle->VertexColor[BottomIndex].b;

	// Texture coordinate
	TopU = (pTriangle->VertexTexture[TopIndex].u * (TextureWidth-1));
	TopV = (pTriangle->VertexTexture[TopIndex].v * (TextureHeight-1));
	TopZ = pTri->Vertex[TopIndex].z;//(ScreenVertexList[pTriangle->VertexIndex[TopIndex]].z);

	MiddleU = (pTriangle->VertexTexture[MiddleIndex].u * (TextureWidth-1));
	MiddleV = (pTriangle->VertexTexture[MiddleIndex].v * (TextureHeight-1));
	MiddleZ = pTri->Vertex[MiddleIndex].z;//(ScreenVertexList[ pTriangle->VertexIndex[MiddleIndex] ].z);

	BottomU = (pTriangle->VertexTexture[BottomIndex].u * (TextureWidth-1));
	BottomV = (pTriangle->VertexTexture[BottomIndex].v * (TextureHeight-1));
	BottomZ = pTri->Vertex[BottomIndex].z;//(ScreenVertexList[pTriangle->VertexIndex[BottomIndex]].z);

	TopOZ = 1 / TopZ;
	MiddleOZ = 1 / MiddleZ;
	BottomOZ = 1 / BottomZ;

	TopUZ = (TopU) / TopZ;
	TopVZ = (TopV) / TopZ;

	MiddleUZ = (MiddleU) / MiddleZ;
	MiddleVZ = (MiddleV) / MiddleZ;

	BottomUZ = (BottomU) / BottomZ;
	BottomVZ = (BottomV) / BottomZ;

	y = Top.y;

	xTB = Top.x<<16;
	xTMB = Top.x<<16;

	cTB.r = TopColorR<<16;
	cTB.g = TopColorG<<16;
	cTB.b = TopColorB<<16;
	cTMB.r = TopColorR<<16;
	cTMB.g = TopColorG<<16;
	cTMB.b = TopColorB<<16;

	tTB.uz = TopUZ;
	tTB.vz = TopVZ;
	tTB.oz = TopOZ;
	tTMB.uz = TopUZ;
	tTMB.vz = TopVZ;
	tTMB.oz = TopOZ;

	if (Bottom.y == Top.y) return;
	xdTB = ((Bottom.x - Top.x)<<16) / (Bottom.y - Top.y);
	cdTB.r = ((BottomColorR - TopColorR)<<16) / (Bottom.y - Top.y);
	cdTB.g = ((BottomColorG - TopColorG)<<16) / (Bottom.y - Top.y);
	cdTB.b = ((BottomColorB - TopColorB)<<16) / (Bottom.y - Top.y);
	tdTB.uz = (BottomUZ - TopUZ) / (Bottom.y - Top.y);
	tdTB.vz = (BottomVZ - TopVZ) / (Bottom.y - Top.y);
	tdTB.oz = (BottomOZ - TopOZ) / (Bottom.y - Top.y);
	
	if (Middle.y == Top.y) 
	{ 
		xTMB = Middle.x<<16;
		cTMB.r = MiddleColorR<<16; 
		cTMB.g = MiddleColorG<<16; 
		cTMB.b = MiddleColorB<<16; 
		tTMB.uz = MiddleUZ; 
		tTMB.vz = MiddleVZ; 
		tTMB.oz = MiddleOZ; 
		goto MIDDLETOBOTTOM; 
	}
	xdTM = ((Middle.x - Top.x)<<16) / (Middle.y - Top.y);
	cdTM.r = ((MiddleColorR - TopColorR)<<16) / (Middle.y - Top.y);
	cdTM.g = ((MiddleColorG - TopColorG)<<16) / (Middle.y - Top.y);
	cdTM.b = ((MiddleColorB - TopColorB)<<16) / (Middle.y - Top.y);
	tdTM.uz = (MiddleUZ - TopUZ) / (Middle.y - Top.y);
	tdTM.vz = (MiddleVZ - TopVZ) / (Middle.y - Top.y);
	tdTM.oz = (MiddleOZ - TopOZ) / (Middle.y - Top.y);
	
	if ( Middle.y > 479 ) Middle.y = 479;

	if ( Top.y < 0 ) 
	{
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * ( 0 - Top.y));
		
		cTB.r += (cdTB.r * (0 - Top.y));
		cTB.g += (cdTB.g * (0 - Top.y));
		cTB.b += (cdTB.b * (0 - Top.y));
		cTMB.r += (cdTM.r * ( 0 - Top.y));
		cTMB.g += (cdTM.g * ( 0 - Top.y));
		cTMB.b += (cdTM.b * ( 0 - Top.y));
		tTB.uz += (tdTB.uz * (0 - Top.y));
		tTB.vz += (tdTB.vz * (0 - Top.y));
		tTB.oz += (tdTB.oz * (0 - Top.y));
		tTMB.uz += (tdTM.uz * ( 0 - Top.y));
		tTMB.vz += (tdTM.vz * ( 0 - Top.y));
		tTMB.oz += (tdTM.oz * ( 0 - Top.y));
		
		y = 0;
	}
	if ( Middle. y < 0 ) goto MIDDLETOBOTTOM;
	while( y < Middle.y )
	{
		
		//HLineGouraudPerspectTexturef16( (xTB+32768)>>16, (xTMB + 32768 )>>16, y, cTB, cTMB, tTB, tTMB);
		HLineGouraudPerspectTexturef16( (xTB)>>16, (xTMB)>>16, y, cTB, cTMB, tTB, tTMB);
		
		xTB += xdTB;
		xTMB += xdTM;
		
		cTB.r += cdTB.r;
		cTB.g += cdTB.g;
		cTB.b += cdTB.b;
		cTMB.r += cdTM.r;
		cTMB.g += cdTM.g;
		cTMB.b += cdTM.b;
		tTB.uz += tdTB.uz;
		tTB.vz += tdTB.vz;
		tTB.oz += tdTB.oz;
		tTMB.uz += tdTM.uz;
		tTMB.vz += tdTM.vz;
		tTMB.oz += tdTM.oz;

		y++;
	}

MIDDLETOBOTTOM:	

	if (Bottom.y == Middle.y) return;
	xdMB = ((Bottom.x - Middle.x)<<16) / (Bottom.y - Middle.y);
	cdMB.r = ((BottomColorR - MiddleColorR)<<16) / (Bottom.y - Middle.y);
	cdMB.g = ((BottomColorG - MiddleColorG)<<16) / (Bottom.y - Middle.y);
	cdMB.b = ((BottomColorB - MiddleColorB)<<16) / (Bottom.y - Middle.y);
	tdMB.uz = (BottomUZ - MiddleUZ) / (Bottom.y - Middle.y);
	tdMB.vz = (BottomVZ - MiddleVZ) / (Bottom.y - Middle.y);
	tdMB.oz = (BottomOZ - MiddleOZ) / (Bottom.y - Middle.y);
	if ( Bottom.y > 479 ) Bottom.y = 479;
	
	if ( Middle.y < 0 ) 
	{
		xTB = Top.x <<16;
		cTB.r = TopColorR <<16;
		cTB.g = TopColorG <<16;
		cTB.b = TopColorB <<16;
		tTB.uz = TopUZ;
		tTB.vz = TopVZ;
		tTB.oz = TopOZ;
		if ( Middle.y != Top.y ) 
		{
			xTMB = Top.x <<16;
			cTMB.r = TopColorR <<16;
			cTMB.g = TopColorG <<16;
			cTMB.b = TopColorB <<16;
			tTMB.uz = TopUZ;
			tTMB.vz = TopVZ;
			tTMB.oz = TopOZ;
		}
		else
		{ 
			xTMB = Middle.x <<16;
			cTMB.r = MiddleColorR <<16;
			cTMB.g = MiddleColorG <<16;
			cTMB.b = MiddleColorB <<16;
			tTMB.uz = MiddleUZ;
			tTMB.vz = MiddleVZ;
			tTMB.oz = MiddleOZ;
		}
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * (Middle.y - Top.y));
		xTMB += (xdMB * (0 - Middle.y));
		
		cTB.r += (cdTB.r * (0 - Top.y));
		cTB.g += (cdTB.g * (0 - Top.y));
		cTB.b += (cdTB.b * (0 - Top.y));
		cTMB.r += (cdTM.r * (Middle.y - Top.y));
		cTMB.g += (cdTM.g * (Middle.y - Top.y));
		cTMB.b += (cdTM.b * (Middle.y - Top.y));
		cTMB.r += (cdMB.r * (0 - Middle.y));
		cTMB.g += (cdMB.g * (0 - Middle.y));
		cTMB.b += (cdMB.b * (0 - Middle.y));
		tTB.uz += (tdTB.uz * (0 - Top.y));
		tTB.vz += (tdTB.vz * (0 - Top.y));
		tTB.oz += (tdTB.oz * (0 - Top.y));
		tTMB.uz += (tdTM.uz * (Middle.y - Top.y));
		tTMB.vz += (tdTM.vz * (Middle.y - Top.y));
		tTMB.oz += (tdTM.oz * (Middle.y - Top.y));
		tTMB.uz += (tdMB.uz * (0 - Middle.y));
		tTMB.vz += (tdMB.vz * (0 - Middle.y));
		tTMB.oz += (tdMB.oz * (0 - Middle.y));
		
		y = 0;
	}
	while( y < Bottom.y )
	{
		//HLineGouraudPerspectTexturef16( (xTB+32768)>>16, (xTMB + 32768)>>16, y, cTB, cTMB, tTB, tTMB);
		HLineGouraudPerspectTexturef16( (xTB)>>16, (xTMB)>>16, y, cTB, cTMB, tTB, tTMB);
		
		xTB += xdTB;
		xTMB += xdMB;
		
		cTB.r += cdTB.r;
		cTB.g += cdTB.g;
		cTB.b += cdTB.b;
		cTMB.r += cdMB.r;
		cTMB.g += cdMB.g;
		cTMB.b += cdMB.b;
		tTB.uz += tdTB.uz;
		tTB.vz += tdTB.vz;
		tTB.oz += tdTB.oz;
		tTMB.uz += tdMB.uz;
		tTMB.vz += tdMB.vz;
		tTMB.oz += tdMB.oz;

		y++;
	}
}

void FillTriPerspectTexturef16(TRISTRUCT *pTri, TRIANGLE *pTriangle)
{
	POINT2D Top, Middle, Bottom;
	int xTB, xTMB;
	int xdTB, xdTM, xdMB;
	int i, TopIndex, MiddleIndex, BottomIndex;
	int y;

	float TopZ, TopU, TopV;
	float MiddleZ, MiddleU, MiddleV; 
	float BottomZ, BottomU, BottomV;
	
	float TopOZ, TopUZ, TopVZ;
	float MiddleOZ, MiddleUZ, MiddleVZ; 
	float BottomOZ, BottomUZ, BottomVZ;

	TEXCOORDFLOAT tTB, tTMB;
	TEXCOORDFLOAT tdTB, tdTM, tdMB;
	
	// Sorting Triangle Vertex
	Top.y = (int)pTri->Vertex[0].y;
	TopIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Top.y > (int)pTri->Vertex[i].y )
		{
			Top.y = (int)pTri->Vertex[i].y;
			TopIndex = i;
		}
	}

	Bottom.y = (int)pTri->Vertex[0].y;
	BottomIndex = 0;
	for ( i = 1; i < 3; i++ )
	{
		if ( Bottom.y < (int)pTri->Vertex[i].y )
		{
			Bottom.y = (int)pTri->Vertex[i].y;
			BottomIndex = i;
		}
	}
	
	if (TopIndex == BottomIndex) return;
	MiddleIndex = 3 - (TopIndex + BottomIndex);
	Middle.x = (int)pTri->Vertex[MiddleIndex].x;
	Middle.y = (int)pTri->Vertex[MiddleIndex].y;
	Top.x = (int)pTri->Vertex[TopIndex].x;
	Bottom.x = (int)pTri->Vertex[BottomIndex].x;

	// Texture coordinate
	TopU = (pTriangle->VertexTexture[TopIndex].u * (TextureWidth-1));
	TopV = (pTriangle->VertexTexture[TopIndex].v * (TextureHeight-1));
	TopZ = pTri->Vertex[TopIndex].z;//(ScreenVertexList[pTriangle->VertexIndex[TopIndex]].z);

	MiddleU = (pTriangle->VertexTexture[MiddleIndex].u * (TextureWidth-1));
	MiddleV = (pTriangle->VertexTexture[MiddleIndex].v * (TextureHeight-1));
	MiddleZ = pTri->Vertex[MiddleIndex].z;//(ScreenVertexList[ pTriangle->VertexIndex[MiddleIndex] ].z);

	BottomU = (pTriangle->VertexTexture[BottomIndex].u * (TextureWidth-1));
	BottomV = (pTriangle->VertexTexture[BottomIndex].v * (TextureHeight-1));
	BottomZ = pTri->Vertex[BottomIndex].z;//(ScreenVertexList[pTriangle->VertexIndex[BottomIndex]].z);

	TopOZ = 1 / TopZ;
	MiddleOZ = 1 / MiddleZ;
	BottomOZ = 1 / BottomZ;

	TopUZ = (TopU) / TopZ;
	TopVZ = (TopV) / TopZ;

	MiddleUZ = (MiddleU) / MiddleZ;
	MiddleVZ = (MiddleV) / MiddleZ;

	BottomUZ = (BottomU) / BottomZ;
	BottomVZ = (BottomV) / BottomZ;

	y = Top.y;

	xTB = Top.x<<16;
	xTMB = Top.x<<16;

	tTB.u = TopUZ;
	tTB.v = TopVZ;
	tTB.oz = TopOZ;
	tTMB.u = TopUZ;
	tTMB.v = TopVZ;
	tTMB.oz = TopOZ;

	if (Bottom.y == Top.y) return;
	xdTB = ((Bottom.x - Top.x)<<16) / (Bottom.y - Top.y);
	tdTB.u = (BottomUZ - TopUZ) / (Bottom.y - Top.y);
	tdTB.v = (BottomVZ - TopVZ) / (Bottom.y - Top.y);
	tdTB.oz = (BottomOZ - TopOZ) / (Bottom.y - Top.y);
	
	if (Middle.y == Top.y) 
	{ 
		xTMB = Middle.x<<16;
		tTMB.u = MiddleUZ; 
		tTMB.v = MiddleVZ; 
		tTMB.oz = MiddleOZ; 
		goto MIDDLETOBOTTOM; 
	}
	xdTM = ((Middle.x - Top.x)<<16) / (Middle.y - Top.y);
	tdTM.u = (MiddleUZ - TopUZ) / (Middle.y - Top.y);
	tdTM.v = (MiddleVZ - TopVZ) / (Middle.y - Top.y);
	tdTM.oz = (MiddleOZ - TopOZ) / (Middle.y - Top.y);
	
	if ( Middle.y > 479 ) Middle.y = 479;

	if ( Top.y < 0 ) 
	{
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * ( 0 - Top.y));
		
		tTB.u += (tdTB.u * (0 - Top.y));
		tTB.v += (tdTB.v * (0 - Top.y));
		tTB.oz += (tdTB.oz * (0 - Top.y));
		tTMB.u += (tdTM.u * ( 0 - Top.y));
		tTMB.v += (tdTM.v * ( 0 - Top.y));
		tTMB.oz += (tdTM.oz * ( 0 - Top.y));
		
		y = 0;
	}
	if ( Middle. y < 0 ) goto MIDDLETOBOTTOM;
	while( y < Middle.y )
	{
		
		//HLinePerspectTexturef16( (xTB+32768)>>16, (xTMB + 32768 )>>16, y, tTB, tTMB);
		HLinePerspectTexturef16( (xTB)>>16, (xTMB)>>16, y, tTB, tTMB);
		
		xTB += xdTB;
		xTMB += xdTM;
		
		tTB.u += tdTB.u;
		tTB.v += tdTB.v;
		tTB.oz += tdTB.oz;
		tTMB.u += tdTM.u;
		tTMB.v += tdTM.v;
		tTMB.oz += tdTM.oz;

		y++;
	}

MIDDLETOBOTTOM:	

	if (Bottom.y == Middle.y) return;
	xdMB = ((Bottom.x - Middle.x)<<16) / (Bottom.y - Middle.y);
	tdMB.u = (BottomUZ - MiddleUZ) / (Bottom.y - Middle.y);
	tdMB.v = (BottomVZ - MiddleVZ) / (Bottom.y - Middle.y);
	tdMB.oz = (BottomOZ - MiddleOZ) / (Bottom.y - Middle.y);
	if ( Bottom.y > 479 ) Bottom.y = 479;
	
	if ( Middle.y < 0 ) 
	{
		xTB = Top.x <<16;
		tTB.u = TopUZ;
		tTB.v = TopVZ;
		tTB.oz = TopOZ;
		if ( Middle.y != Top.y ) 
		{
			xTMB = Top.x <<16;
			tTMB.u = TopUZ;
			tTMB.v = TopVZ;
			tTMB.oz = TopOZ;
		}
		else
		{ 
			xTMB = Middle.x <<16;
			tTMB.u = MiddleUZ;
			tTMB.v = MiddleVZ;
			tTMB.oz = MiddleOZ;
		}
		xTB += (xdTB * (0 - Top.y));
		xTMB += (xdTM * (Middle.y - Top.y));
		xTMB += (xdMB * (0 - Middle.y));
		
		tTB.u += (tdTB.u * (0 - Top.y));
		tTB.v += (tdTB.v * (0 - Top.y));
		tTB.oz += (tdTB.oz * (0 - Top.y));
		tTMB.u += (tdTM.u * (Middle.y - Top.y));
		tTMB.v += (tdTM.v * (Middle.y - Top.y));
		tTMB.oz += (tdTM.oz * (Middle.y - Top.y));
		tTMB.u += (tdMB.u * (0 - Middle.y));
		tTMB.v += (tdMB.v * (0 - Middle.y));
		tTMB.oz += (tdMB.oz * (0 - Middle.y));
		
		y = 0;
	}
	while( y < Bottom.y )
	{
		//HLinePerspectTexturef16( (xTB+32768)>>16, (xTMB + 32768)>>16, y, tTB, tTMB);
		HLinePerspectTexturef16( (xTB)>>16, (xTMB)>>16, y, tTB, tTMB);
		
		xTB += xdTB;
		xTMB += xdMB;
		
		tTB.u += tdTB.u;
		tTB.v += tdTB.v;
		tTB.oz += tdTB.oz;
		tTMB.u += tdMB.u;
		tTMB.v += tdMB.v;
		tTMB.oz += tdMB.oz;

		y++;
	}
}

