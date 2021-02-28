#include <cfloat>
#include "ScreenSurface.h"

ScreenSurface::ScreenSurface(int width, int height, int channels, unsigned char red, unsigned char green, unsigned char blue)
	: Image(width, height, channels, true, red, green, blue), m_pZBuffer(nullptr)
{
	m_pZBuffer = new float[width * height];
	ClearZBuffer();
}

ScreenSurface::~ScreenSurface()
{
	if (m_pZBuffer != nullptr)
	{
		delete[] m_pZBuffer;
		m_pZBuffer = nullptr;
	}
}

int ScreenSurface::GetZOffset(int x, int y)
{
	return x + y * m_iWidth;
}

void ScreenSurface::ClearZBuffer(void)
{
	for (int i = 0; i < m_iWidth * m_iHeight; ++i)
	{
		m_pZBuffer[i] = -FLT_MAX;
	}
}

void ScreenSurface::SetZBuffer(int x, int y, float z, unsigned char red, unsigned char green, unsigned char blue)
{
	if (IsIncludePixelInBuffer(x, y))
	{
		int offset = GetZOffset(x, y);

		if (m_pZBuffer[offset] < z)
		{
			m_pZBuffer[offset] = z;
			SetPixel(x, y, red, green, blue);
		}
	}
}

void ScreenSurface::SetZBuffer(float u, float v, float z, unsigned char red, unsigned char green, unsigned char blue)
{
	int x = static_cast<int>(u * static_cast<float>(m_iWidth));
	int y = static_cast<int>(v * static_cast<float>(m_iHeight));

	SetZBuffer(x, y, z, red, green, blue);
}
