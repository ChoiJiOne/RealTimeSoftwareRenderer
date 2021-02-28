#pragma once

#include "Image.h"

class ScreenSurface : public Image
{
public:
	ScreenSurface(int width, int height, int channels, unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0);
	virtual ~ScreenSurface();

	void SetZBuffer(int x, int y, float z, unsigned char red, unsigned char green, unsigned char blue);
	void SetZBuffer(float u, float v, float z, unsigned char red, unsigned char green, unsigned char blue);

	int  GetZOffset(int x, int y);
	void ClearZBuffer(void);

private:
	float* m_pZBuffer;
};