#include "Framebuffer.h"
#include "Rasterizer.h"


Rasterizer::Rasterizer(std::shared_ptr<Framebuffer> pFramebuffer) : m_pFramebuffer(pFramebuffer)
{

}

Rasterizer::~Rasterizer()
{

}

void Rasterizer::DrawLine(const Vec2i& vert0, const Vec2i& vert1, const RGB24& color)
{
	bool isYLonger = false;

	int x0 = vert0.x;
	int y0 = vert0.y;
	int x1 = vert1.x;
	int y1 = vert1.y;

	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		isYLonger = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx     = x1 - x0;
	int dy     = y1 - y0;
	int derror = 2 * std::abs(dy);
	int error  = 0;
	int y      = y0;
	int bias   = y1 > y0 ? 1 : -1;

	if (isYLonger)
	{
		for (int x = x0; x <= x1; ++x)
		{
			m_pFramebuffer->SetPixel(y, x, color);
			error += derror;

			if (error > dx)
			{
				y += bias;
				error -= (dx * 2);
			}
		}
	}
	else
	{
		for (int x = x0; x <= x1; ++x)
		{
			m_pFramebuffer->SetPixel(x, y, color);
			error += derror;

			if (error > dx)
			{
				y += bias;
				error -= (dx * 2);
			}
		}
	}
}

void Rasterizer::DrawLine(const Vec2i& vert0, const RGB24& color0, const Vec2i& vert1, const RGB24& color1)
{
	bool isYLonger = false;

	int x0 = vert0.x;
	int y0 = vert0.y;
	int x1 = vert1.x;
	int y1 = vert1.y;
	RGB24 c0 = color0;
	RGB24 c1 = color1;

	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		isYLonger = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(c0, c1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror = 2 * std::abs(dy);
	int error = 0;
	int y = y0;
	int bias = y1 > y0 ? 1 : -1;
	int length = std::abs(dx) + 1;
	int t = 0;

	if (isYLonger)
	{
		for (int x = x0, t = 0; x <= x1; ++x, ++t)
		{
			float scale = static_cast<float>(t) / static_cast<float>(length);
			RGB24 color = c0 * (1.0f - scale) + c1 * scale;

			m_pFramebuffer->SetPixel(y, x, color);
			error += derror;

			if (error > dx)
			{
				y += bias;
				error -= (dx * 2);
			}
		}
	}
	else
	{
		for (int x = x0, t = 0; x <= x1; ++x, ++t)
		{
			float scale = static_cast<float>(t) / static_cast<float>(length);
			RGB24 color = c0 * (1.0f - scale) + c1 * scale;

			m_pFramebuffer->SetPixel(x, y, color);
			error += derror;

			if (error > dx)
			{
				y += bias;
				error -= (dx * 2);
			}
		}
	}
}

void Rasterizer::DrawLine(const Vec2i vert[2], const RGB24& color)
{
	DrawLine(vert[0], vert[1], color);
}

void Rasterizer::DrawLine(const Vec2i vert[2], const RGB24 color[2])
{
	DrawLine(vert[0], color[0], vert[1], color[1]);
}

void Rasterizer::DrawTriangle(const Vec2i vert[3], const RGB24& color)
{
	DrawLine(vert[0], vert[1], color);
	DrawLine(vert[1], vert[2], color);
	DrawLine(vert[2], vert[0], color);
}

void Rasterizer::FillTriangle(const Vec2i vert[3], const RGB24& color)
{



}

void Rasterizer::DrawTriangle(const Vec2i vert[3], const RGB24 color[3])
{
	DrawLine(vert[0], color[0], vert[1], color[1]);
	DrawLine(vert[1], color[1], vert[2], color[2]);
	DrawLine(vert[2], color[2], vert[0], color[0]);
}

void Rasterizer::FillTriangle(const Vec2i vert[3], const RGB24 color[3])
{
	


}