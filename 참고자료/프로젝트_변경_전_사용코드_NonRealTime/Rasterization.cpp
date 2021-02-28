#include "Math.h"
#include "ScreenSurface.h"
#include "Rasterization.h"

Rasterization::Rasterization(std::shared_ptr<ScreenSurface> surface) : m_pSurface(surface)
{

}

Rasterization::~Rasterization()
{

}

void Rasterization::DrawHLine(int x0, int x1, int y, const RGBColor& c)
{
	if (x0 > x1)
	{
		std::swap(x0, x1);
	}

	for (int x = x0; x <= x1; ++x)
	{
		m_pSurface->SetPixel(x, y, GetRGBRed(c), GetRGBGreen(c), GetRGBBlue(c));
	}
}

void Rasterization::DrawHLine(int x0, float z0, int x1, float z1, int y, const RGBColor& c)
{
	float z = 0.0f;
	int length = std::abs(x1 - x0) + 1;

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(z0, z1);
	}

	for (int x = x0, t = 0; x <= x1; ++x, ++t)
	{
		z = Interpolation(z0, z1, t, length);
		m_pSurface->SetZBuffer(x, y, z, GetRGBRed(c), GetRGBGreen(c), GetRGBBlue(c));
	}
}

void Rasterization::DrawHLine(int x0, int x1, int y, const RGBColor& c0, const RGBColor& c1)
{
	RGBColor color0 = c0;
	RGBColor color1 = c1;
	RGBColor color;
	int length = std::abs(x1 - x0) + 1;

	if (x0 > x1)
	{
		std::swap(x0, x1);
		Swap(color0, color1);
	}

	for (int x = x0, t = 0; x <= x1; ++x, ++t)
	{
		color = Interpolation(color0, color1, t, length);
		m_pSurface->SetPixel(x, y, GetRGBRed(color), GetRGBGreen(color), GetRGBBlue(color));
	}
}

void Rasterization::DrawHLine(int x0, float z0, int x1, float z1, int y, const RGBColor& c0, const RGBColor& c1)
{
	RGBColor color0 = c0;
	RGBColor color1 = c1;
	RGBColor color;
	int length = std::abs(x1 - x0) + 1;
	float z = 0.0f;

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(z0, z1);
		Swap(color0, color1);
	}

	for (int x = x0, t = 0; x <= x1; ++x, ++t)
	{
		color = Interpolation(color0, color1, t, length);
		z = Interpolation(z0, z1, t, length);
		m_pSurface->SetZBuffer(x, y, z, GetRGBRed(color), GetRGBGreen(color), GetRGBBlue(color));
	}
}

void Rasterization::SortVertexInfo(Vector2i& top, Vector2i& middle, Vector2i& bottom)
{
	if (top.y < bottom.y)    Swap(top, bottom);
	if (top.y < middle.y)    Swap(top, middle);
	if (middle.y < bottom.y) Swap(middle, bottom);
}

void Rasterization::SortVertexInfo(Vector2i& top, float& topz, Vector2i& middle, float& middlez, Vector2i& bottom, float& bottomz)
{
	if (top.y < bottom.y)
	{
		Swap(top, bottom);
		std::swap(topz, bottomz);
	}

	if (top.y < middle.y)
	{
		Swap(top, middle);
		std::swap(topz, middlez);
	}

	if (middle.y < bottom.y)
	{
		Swap(middle, bottom);
		std::swap(middlez, bottomz);
	}
}

void Rasterization::SortVertexInfo(Vector2i& topv, RGBColor& topc, Vector2i& middlev, RGBColor& middlec, Vector2i& bottomv, RGBColor& bottomc)
{
	if (topv.y < bottomv.y)
	{
		Swap(topv, bottomv);
		Swap(topc, bottomc);
	}

	if (topv.y < middlev.y)
	{
		Swap(topv, middlev);
		Swap(topc, middlec);
	}

	if (middlev.y < bottomv.y)
	{
		Swap(middlev, bottomv);
		Swap(middlec, bottomc);
	}
}

void Rasterization::SortVertexInfo(Vector2i& topv, RGBColor& topc, float& topz, Vector2i& middlev, RGBColor& middlec, float& middlez, Vector2i& bottomv, RGBColor& bottomc, float& bottomz)
{
	if (topv.y < bottomv.y)
	{
		Swap(topv, bottomv);
		Swap(topc, bottomc);
		std::swap(topz, bottomz);
	}

	if (topv.y < middlev.y)
	{
		Swap(topv, middlev);
		Swap(topc, middlec);
		std::swap(topz, middlez);
	}

	if (middlev.y < bottomv.y)
	{
		Swap(middlev, bottomv);
		Swap(middlec, bottomc);
		std::swap(middlez, bottomz);
	}
}

void Rasterization::DrawLine(const Vector2i& v0, const Vector2i& v1, const RGBColor& c)
{
	int x0 = v0.x, x1 = v1.x;
	int y0 = v0.y, y1 = v1.y;

	bool isYLonger   = false;
	int  shortLength = y1 - y0;
	int  longLength  = x1 - x0;
	int  increase    = 0;
	int  bias        = 0;

	if (std::abs(shortLength) > std::abs(longLength))
	{
		std::swap(shortLength, longLength);
		isYLonger = true;
	}

	if (longLength != 0)
	{
		increase = (shortLength << 16) / longLength;
	}

	if (isYLonger)
	{
		int y = y0;
		int x = 0x8000 + (x0 << 16);
		bias = longLength > 0 ? 1 : -1;

		while ((y0 <= y && y <= y1) || (y1 <= y && y <= y0))
		{
			m_pSurface->SetPixel(x >> 16, y, GetRGBRed(c), GetRGBGreen(c), GetRGBBlue(c));
			x += bias * increase;
			y += bias;
		}
	}
	else
	{
		int x = x0;
		int y = 0x8000 + (y0 << 16);
		bias = longLength > 0 ? 1 : -1;

		while ((x0 <= x && x <= x1) || (x1 <= x && x <= x0))
		{
			m_pSurface->SetPixel(x, y >> 16, GetRGBRed(c), GetRGBGreen(c), GetRGBBlue(c));
			y += bias * increase;
			x += bias;
		}
	}
}

void Rasterization::DrawLine(const Vector2i& v0, float z0, const Vector2i& v1, float z1, const RGBColor& c)
{
	int x0 = v0.x, x1 = v1.x;
	int y0 = v0.y, y1 = v1.y;

	bool isYLonger   = false;
	int  shortLength = y1 - y0;
	int  longLength  = x1 - x0;
	int  increase    = 0;
	int  bias        = 0;

	float z = 0.0f;
	int length = std::abs(longLength) + 1;
	int t = 0;

	if (std::abs(shortLength) > std::abs(longLength))
	{
		std::swap(shortLength, longLength);
		isYLonger = true;
	}

	if (longLength != 0)
	{
		increase = (shortLength << 16) / longLength;
	}

	if (isYLonger)
	{
		int y = y0;
		int x = 0x8000 + (x0 << 16);
		bias = longLength > 0 ? 1 : -1;

		while ((y0 <= y && y <= y1) || (y1 <= y && y <= y0))
		{
			z = Interpolation(z0, z1, t, length);

			m_pSurface->SetZBuffer(x >> 16, y, z, GetRGBRed(c), GetRGBGreen(c), GetRGBBlue(c));
			x += bias * increase;
			y += bias;
			t += 1;
		}
	}
	else
	{
		int x = x0;
		int y = 0x8000 + (y0 << 16);
		bias = longLength > 0 ? 1 : -1;

		while ((x0 <= x && x <= x1) || (x1 <= x && x <= x0))
		{
			z = Interpolation(z0, z1, t, length);

			m_pSurface->SetZBuffer(x, y >> 16, z,GetRGBRed(c), GetRGBGreen(c), GetRGBBlue(c));
			y += bias * increase;
			x += bias;
			t += 1;
		}
	}
}

void Rasterization::DrawLine(const Vector2i& v0, const RGBColor& c0, const Vector2i& v1, const RGBColor& c1)
{
	int x0 = v0.x, x1 = v1.x;
	int y0 = v0.y, y1 = v1.y;

	bool isYLonger   = false;
	int  shortLength = y1 - y0;
	int  longLength  = x1 - x0;
	int  increase    = 0;
	int  bias        = 0;

	if (std::abs(shortLength) > std::abs(longLength))
	{
		std::swap(shortLength, longLength);
		isYLonger = true;
	}

	if (longLength != 0)
	{
		increase = (shortLength << 16) / longLength;
	}

	int t = 0;
	int length = std::abs(longLength) + 1;
	RGBColor color(0.0f, 0.0f, 0.0f);
	RGBColor color0 = c0;
	RGBColor color1 = c1;

	if (isYLonger)
	{
		int y = y0;
		int x = 0x8000 + (x0 << 16);
		bias = longLength > 0 ? 1 : -1;

		while ((y0 <= y && y <= y1) || (y1 <= y && y <= y0))
		{
			color = Interpolation(color0, color1, t, length);
			m_pSurface->SetPixel(x >> 16, y, GetRGBRed(color), GetRGBGreen(color), GetRGBBlue(color));

			x += bias * increase;
			y += bias;
			t += 1;
		}
	}
	else
	{
		int x = x0;
		int y = 0x8000 + (y0 << 16);
		bias = longLength > 0 ? 1 : -1;

		while ((x0 <= x && x <= x1) || (x1 <= x && x <= x0))
		{
			color = Interpolation(color0, color1, t, length);
			m_pSurface->SetPixel(x, y >> 16, GetRGBRed(color), GetRGBGreen(color), GetRGBBlue(color));

			x += bias;
			y += bias * increase;
			t += 1;
		}
	}
}

void Rasterization::DrawLine(const Vector2i& v0, float z0, const RGBColor& c0, const Vector2i& v1, float z1, const RGBColor& c1)
{
	int x0 = v0.x, x1 = v1.x;
	int y0 = v0.y, y1 = v1.y;

	bool isYLonger   = false;
	int  shortLength = y1 - y0;
	int  longLength  = x1 - x0;
	int  increase    = 0;
	int  bias        = 0;

	float z = 0.0f;

	if (std::abs(shortLength) > std::abs(longLength))
	{
		std::swap(shortLength, longLength);
		isYLonger = true;
	}

	if (longLength != 0)
	{
		increase = (shortLength << 16) / longLength;
	}

	int t = 0;
	int length = std::abs(longLength) + 1;
	RGBColor color(0.0f, 0.0f, 0.0f);
	RGBColor color0 = c0;
	RGBColor color1 = c1;

	if (isYLonger)
	{
		int y = y0;
		int x = 0x8000 + (x0 << 16);
		bias = longLength > 0 ? 1 : -1;

		while ((y0 <= y && y <= y1) || (y1 <= y && y <= y0))
		{
			z = Interpolation(z0, z1, t, length);
			color = Interpolation(color0, color1, t, length);

			m_pSurface->SetZBuffer(x >> 16, y, z, GetRGBRed(color), GetRGBGreen(color), GetRGBBlue(color));

			x += bias * increase;
			y += bias;
			t += 1;
		}
	}
	else
	{
		int x = x0;
		int y = 0x8000 + (y0 << 16);
		bias = longLength > 0 ? 1 : -1;

		while ((x0 <= x && x <= x1) || (x1 <= x && x <= x0))
		{
			z = Interpolation(z0, z1, t, length);
			color = Interpolation(color0, color1, t, length);

			m_pSurface->SetZBuffer(x, y >> 16, z, GetRGBRed(color), GetRGBGreen(color), GetRGBBlue(color));

			x += bias;
			y += bias * increase;
			t += 1;
		}
	}
}

void Rasterization::DrawTriangle(const Vector2i& v0, const Vector2i& v1, const Vector2i& v2, const RGBColor& c)
{
	DrawLine(v0, v1, c);
	DrawLine(v1, v2, c);
	DrawLine(v2, v0, c);
}

void Rasterization::DrawTriangle(const Vector2i& v0, float z0, const Vector2i& v1, float z1, const Vector2i& v2, float z2, const RGBColor& c)
{
	DrawLine(v0, z0, v1, z1, c);
	DrawLine(v1, z1, v2, z2, c);
	DrawLine(v2, z2, v0, z0, c);
}

void Rasterization::DrawTriangle(const Vector2i& v0, const RGBColor& c0, const Vector2i& v1, const RGBColor& c1, const Vector2i& v2, const RGBColor& c2)
{
	DrawLine(v0, c0, v1, c1);
	DrawLine(v1, c1, v2, c2);
	DrawLine(v2, c2, v0, c0);
}

void Rasterization::DrawTriangle(const Vector2i& v0, float z0, const RGBColor& c0, const Vector2i& v1, float z1, const RGBColor& c1, const Vector2i& v2, float z2, const RGBColor& c2)
{
	DrawLine(v0, z0, c0, v1, z1, c1);
	DrawLine(v1, z1, c1, v2, z2, c2);
	DrawLine(v2, z2, c2, v0, z0, c0);
}

void Rasterization::FillTriangle(const Vector2i& v0, const Vector2i& v1, const Vector2i& v2, const RGBColor& c)
{
	Vector2i top    = v0;
	Vector2i middle = v1;
	Vector2i bottom = v2;

	int slope0 = 0;
	int slope1 = 0;
	int x0 = 0, x1 = 0;

	SortVertexInfo(top, middle, bottom);
	
	if (top.y != middle.y && middle.y == bottom.y)
	{
		slope0 = static_cast<int>(((top.x - middle.x) << 16) / (top.y - middle.y + 1));
		slope1 = static_cast<int>(((top.x - bottom.x) << 16) / (top.y - bottom.y + 1));

		x0 = middle.x << 16;
		x1 = bottom.x << 16;

		for (int y = bottom.y; y <= top.y; ++y)
		{
			DrawHLine(x0 >> 16, x1 >> 16, y, c);
			x0 += slope0;
			x1 += slope1;
		}
	}
	else if (top.y == middle.y && middle.y != bottom.y)
	{
		slope0 = static_cast<int>(((middle.x - bottom.x) << 16) / (middle.y - bottom.y + 1));
		slope1 = static_cast<int>(((   top.x - bottom.x) << 16) / (   top.y - bottom.y + 1));

		x0 = bottom.x << 16;
		x1 = x0;

		for (int y = bottom.y; y <= top.y; ++y)
		{
			DrawHLine(x0 >> 16, x1 >> 16, y, c);
			x0 += slope0;
			x1 += slope1;
		}
	}
	else if (top.y != middle.y && middle.y != bottom.y && bottom.y != top.y)
	{
		slope0 = static_cast<int>(((middle.x - bottom.x) << 16) / (middle.y - bottom.y));
		slope1 = static_cast<int>(((   top.x - bottom.x) << 16) / (   top.y - bottom.y + 1));

		x0 = bottom.x << 16;
		x1 = x0;

		for (int y = bottom.y; y < middle.y; ++y)
		{
			DrawHLine(x0 >> 16, x1 >> 16, y, c);
			x0 += slope0;
			x1 += slope1;
		}

		slope0 = static_cast<int>(((top.x - middle.x) << 16) / (top.y - middle.y + 1));

		for (int y = middle.y; y <= top.y; ++y)
		{
			DrawHLine(x0 >> 16, x1 >> 16, y, c);
			x0 += slope0;
			x1 += slope1;
		}
	}
}

void Rasterization::FillTriangle(const Vector2i& v0, float z0, const Vector2i& v1, float z1, const Vector2i& v2, float z2, const RGBColor& c)
{
	Vector2i top     = v0;
	float    topz    = z0;
	Vector2i middle  = v1;
	float    middlez = z1;
	Vector2i bottom  = v2;
	float    bottomz = z2;

	int slope0 = 0;
	int slope1 = 0;
	int x0 = 0, x1 = 0;

	SortVertexInfo(top, topz, middle, middlez, bottom, bottomz);

	if (top.y != middle.y && middle.y == bottom.y)
	{
		int length = top.y - bottom.y + 1;
		float z0 = 0.0f, z1 = 0.0f;

		slope0 = static_cast<int>(((top.x - middle.x) << 16) / (top.y - middle.y + 1));
		slope1 = static_cast<int>(((top.x - bottom.x) << 16) / (top.y - bottom.y + 1));

		x0 = middle.x << 16;
		x1 = bottom.x << 16;

		for (int y = bottom.y, t = 0; y <= top.y; ++y, ++t)
		{
			z0 = Interpolation(middlez, topz, t, length);
			z1 = Interpolation(bottomz, topz, t, length);

			DrawHLine(x0 >> 16, z0, x1 >> 16, z1, y, c);
			x0 += slope0;
			x1 += slope1;
		}
	}
	else if (top.y == middle.y && middle.y != bottom.y)
	{
		int length = top.y - bottom.y + 1;
		float z0 = 0.0f, z1 = 0.0f;

		slope0 = static_cast<int>(((middle.x - bottom.x) << 16) / (middle.y - bottom.y + 1));
		slope1 = static_cast<int>(((   top.x - bottom.x) << 16) / (   top.y - bottom.y + 1));

		x0 = bottom.x << 16;
		x1 = x0;

		for (int y = bottom.y, t = 0; y <= top.y; ++y, ++t)
		{
			z0 = Interpolation(bottomz, middlez, t, length);
			z1 = Interpolation(bottomz,    topz, t, length);

			DrawHLine(x0 >> 16, z0, x1 >> 16, z1, y, c);
			x0 += slope0;
			x1 += slope1;
		}
	}
	else if (top.y != middle.y && middle.y != bottom.y && bottom.y != top.y)
	{
		int length0 = middle.y - bottom.y;
		int length1 = top.y - bottom.y + 1;
		float z0 = 0.0f, z1 = 0.0f;
		int t0 = 0, t1 = 0;

		slope0 = static_cast<int>(((middle.x - bottom.x) << 16) / (middle.y - bottom.y));
		slope1 = static_cast<int>(((   top.x - bottom.x) << 16) / (   top.y - bottom.y + 1));

		x0 = bottom.x << 16;
		x1 = x0;

		for (int y = bottom.y; y < middle.y; ++y, ++t0, ++t1)
		{
			z0 = Interpolation(bottomz, middlez, t0, length0);
			z1 = Interpolation(bottomz,    topz, t1, length1);

			DrawHLine(x0 >> 16, z0, x1 >> 16, z1, y, c);
			x0 += slope0;
			x1 += slope1;
		}

		length0 = top.y - middle.y + 1;
		z0 = 0.0f;
		t0 = 0;
		slope0 = static_cast<int>(((top.x - middle.x) << 16) / (top.y - middle.y + 1));

		for (int y = middle.y; y <= top.y; ++y, ++t0, ++t1)
		{
			z0 = Interpolation(middlez, topz, t0, length0);
			z1 = Interpolation(bottomz, topz, t1, length1);

			DrawHLine(x0 >> 16, z0, x1 >> 16, z1, y, c);
			x0 += slope0;
			x1 += slope1;
		}
	}
}

void Rasterization::FillTriangle(const Vector2i& v0, const RGBColor& c0, const Vector2i& v1, const RGBColor& c1, const Vector2i& v2, const RGBColor& c2)
{
	Vector2i topv    = v0;
	RGBColor topc    = c0;
	Vector2i middlev = v1;
	RGBColor middlec = c1;
	Vector2i bottomv = v2;
	RGBColor bottomc = c2;

	int slope0 = 0;
	int slope1 = 0;
	int x0 = 0, x1 = 0;

	int length;
	RGBColor color0, color1;

	SortVertexInfo(topv, topc, middlev, middlec, bottomv, bottomc);

	if (topv.y != middlev.y && middlev.y == bottomv.y)
	{
		slope0 = static_cast<int>(((topv.x - middlev.x) << 16) / (topv.y - middlev.y + 1));
		slope1 = static_cast<int>(((topv.x - bottomv.x) << 16) / (topv.y - bottomv.y + 1));
		
		length = topv.y - bottomv.y + 1;

		x0 = middlev.x << 16;
		x1 = bottomv.x << 16;

		for (int y = bottomv.y, t = 0; y <= topv.y; ++y, ++t)
		{
			color0 = Interpolation(middlec, topc, t, length);
			color1 = Interpolation(bottomc, topc, t, length);

			DrawHLine(x0 >> 16, x1 >> 16, y, color0, color1);
			x0 += slope0;
			x1 += slope1;
		}
	}
	else if (topv.y == middlev.y && middlev.y != bottomv.y)
	{
		slope0 = static_cast<int>(((middlev.x - bottomv.x) << 16) / (middlev.y - bottomv.y + 1));
		slope1 = static_cast<int>(((topv.x - bottomv.x) << 16) / (topv.y - bottomv.y + 1));

		length = topv.y - bottomv.y + 1;

		x0 = bottomv.x << 16;
		x1 = x0;

		for (int y = bottomv.y, t = 0; y <= topv.y; ++y, ++t)
		{
			color0 = Interpolation(bottomc, middlec, t, length);
			color1 = Interpolation(bottomc,    topc, t, length);

			DrawHLine(x0 >> 16, x1 >> 16, y, color0, color1);
			x0 += slope0;
			x1 += slope1;
		}
	}
	else if (topv.y != middlev.y && middlev.y != bottomv.y && bottomv.y != topv.y)
	{
		slope0 = static_cast<int>(((middlev.x - bottomv.x) << 16) / (middlev.y - bottomv.y));
		slope1 = static_cast<int>(((   topv.x - bottomv.x) << 16) / (topv.y - bottomv.y + 1));

		int t0 = 0, t1 = 0;
		int length0 = middlev.y - bottomv.y;
		int length1 = topv.y - bottomv.y + 1;

		x0 = bottomv.x << 16;
		x1 = x0;

		for (int y = bottomv.y; y < middlev.y; ++y, ++t0, ++t1)
		{
			color0 = Interpolation(bottomc, middlec, t0, length0);
			color1 = Interpolation(bottomc,    topc, t1, length1);

			DrawHLine(x0 >> 16, x1 >> 16, y, color0, color1);
			x0 += slope0;
			x1 += slope1;
		}

		t0 = 0;
		length0 = topv.y - middlev.y + 1;
		slope0 = static_cast<int>(((topv.x - middlev.x) << 16) / (topv.y - middlev.y + 1));

		for (int y = middlev.y; y <= topv.y; ++y, ++t0, ++t1)
		{
			color0 = Interpolation(middlec, topc, t0, length0);
			color1 = Interpolation(bottomc, topc, t1, length1);

			DrawHLine(x0 >> 16, x1 >> 16, y, color0, color1);
			x0 += slope0;
			x1 += slope1;
		}
	}
}

void Rasterization::FillTriangle(const Vector2i& v0, float z0, const RGBColor& c0, const Vector2i& v1, float z1, const RGBColor& c1, const Vector2i& v2, float z2, const RGBColor& c2)
{

	Vector2i topv = v0;
	RGBColor topc = c0;
	float    topz = z0;

	Vector2i middlev = v1;
	RGBColor middlec = c1;
	float    middlez = z1;

	Vector2i bottomv = v2;
	RGBColor bottomc = c2;
	float    bottomz = z2;

	int slope0 = 0;
	int slope1 = 0;
	int x0 = 0, x1 = 0;

	int length;
	RGBColor color0, color1;

	SortVertexInfo(topv, topc, topz, middlev, middlec, middlez, bottomv, bottomc, bottomz);

	if (topv.y != middlev.y && middlev.y == bottomv.y)
	{
		length = topv.y - bottomv.y + 1;
		float z0 = 0.0f, z1 = 0.0f;

		slope0 = static_cast<int>(((topv.x - middlev.x) << 16) / (topv.y - middlev.y + 1));
		slope1 = static_cast<int>(((topv.x - bottomv.x) << 16) / (topv.y - bottomv.y + 1));

		x0 = middlev.x << 16;
		x1 = bottomv.x << 16;

		for (int y = bottomv.y, t = 0; y <= topv.y; ++y, ++t)
		{
			color0 = Interpolation(middlec, topc, t, length);
			color1 = Interpolation(bottomc, topc, t, length);

			z0 = Interpolation(middlez, topz, t, length);
			z1 = Interpolation(bottomz, topz, t, length);

			DrawHLine(x0 >> 16, z0, x1 >> 16, z1, y, color0, color1);
			x0 += slope0;
			x1 += slope1;
		}
	}
	else if (topv.y == middlev.y && middlev.y != bottomv.y)
	{
		length = topv.y - bottomv.y + 1;
		float z0 = 0.0f, z1 = 0.0f;

		slope0 = static_cast<int>(((middlev.x - bottomv.x) << 16) / (middlev.y - bottomv.y + 1));
		slope1 = static_cast<int>(((topv.x - bottomv.x) << 16) / (topv.y - bottomv.y + 1));

		x0 = bottomv.x << 16;
		x1 = x0;

		for (int y = bottomv.y, t = 0; y <= topv.y; ++y, ++t)
		{
			color0 = Interpolation(bottomc, middlec, t, length);
			color1 = Interpolation(bottomc, topc, t, length);

			z0 = Interpolation(bottomz, middlez, t, length);
			z1 = Interpolation(bottomz, topz, t, length);

			DrawHLine(x0 >> 16, z0, x1 >> 16, z1, y, color0, color1);
			x0 += slope0;
			x1 += slope1;
		}
	}
	else if (topv.y != middlev.y && middlev.y != bottomv.y && bottomv.y != topv.y)
	{
		int length0 = middlev.y - bottomv.y;
		int length1 = topv.y - bottomv.y + 1;
		int t0 = 0, t1 = 0;

		float z0 = 0.0f, z1 = 0.0f;

		slope0 = static_cast<int>(((middlev.x - bottomv.x) << 16) / (middlev.y - bottomv.y));
		slope1 = static_cast<int>(((topv.x - bottomv.x) << 16) / (topv.y - bottomv.y + 1));

		x0 = bottomv.x << 16;
		x1 = x0;

		for (int y = bottomv.y; y < middlev.y; ++y, ++t0, ++t1)
		{
			color0 = Interpolation(bottomc, middlec, t0, length0);
			color1 = Interpolation(bottomc, topc, t1, length1);

			z0 = Interpolation(bottomz, middlez, t0, length0);
			z1 = Interpolation(bottomz, topz, t1, length1);

			DrawHLine(x0 >> 16, z0, x1 >> 16, z1, y, color0, color1);
			x0 += slope0;
			x1 += slope1;
		}

		z0 = 0.0f;
		t0 = 0;
		length0 = topv.y - middlev.y + 1;
		slope0 = static_cast<int>(((topv.x - middlev.x) << 16) / (topv.y - middlev.y + 1));

		for (int y = middlev.y; y <= topv.y; ++y, ++t0, ++t1)
		{
			color0 = Interpolation(middlec, topc, t0, length0);
			color1 = Interpolation(bottomc, topc, t1, length1);

			z0 = Interpolation(middlez, topz, t0, length0);
			z1 = Interpolation(bottomz, topz, t1, length1);

			DrawHLine(x0 >> 16, z0, x1 >> 16, z1, y, color0, color1);
			x0 += slope0;
			x1 += slope1;
		}
	}
}