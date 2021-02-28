/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

BITMAPFILEHEADER BMFileHeader;
BITMAPINFOHEADER BMInfoHeader;

BYTE* LoadBMP24(char * fname, int bits)
{
	FILE *fp;
	BYTE * Image24;
	BYTE * Image16;

	fp = fopen(fname, "rb");
	if ( fp == NULL ) return NULL;

	fread(&BMFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	if (BMFileHeader.bfType != 0x4D42 )
	{
		fclose(fp);
		return NULL;
	}

	fread(&BMInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	if (BMInfoHeader.biBitCount != 24 || BMInfoHeader.biCompression != BI_RGB)
	{
		fclose(fp);
		return NULL;
	}

	int Width = BMInfoHeader.biWidth;
	int Height = BMInfoHeader.biHeight;

	DWORD ScanBytes = (Width * 3 + 3) & (~3);
	DWORD ImageSize =  ScanBytes * abs(Height);

	Image24 = (BYTE*)malloc(ImageSize);
	fread(Image24, ImageSize, 1, fp);

	fclose(fp);
	
	Image16 = (BYTE*)malloc(Width * Height * 2);

	for (int y = 0; y < abs(Height); y++)
	{
		for (int x = 0; x < Width; x++)
		{
			BYTE b = Image24[x * 3 + y * ScanBytes];
			BYTE g = Image24[x * 3 + y * ScanBytes + 1];
			BYTE r = Image24[x * 3 + y * ScanBytes + 2];

			WORD color16 = r>>3;
			color16 <<= 11;
			WORD color = g>>2;
			color16 |= color<<5;
			color = b>>3;
			color16 |= color;

			*((WORD*)(Image16 + x * 2 + y * Width * 2)) = color16;

		}
	}

	if (bits == 16)
	{
		free(Image24);
		return Image16;
	}
	else if (bits == 24)
	{
		free(Image16);
		return Image24;
	}
	else return NULL;

}