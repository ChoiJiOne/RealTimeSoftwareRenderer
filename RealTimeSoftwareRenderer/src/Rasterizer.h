#pragma once

#include <memory>
#include "Vector.h"

class Rasterizer
{
public:
	Rasterizer(std::shared_ptr<class Framebuffer> pFramebuffer);
	virtual ~Rasterizer();

	void DrawLine(const Vec2i vert[2], const RGB24& color);
	void DrawLine(const Vec2i vert[2], const RGB24 color[2]);
	void DrawTriangle(const Vec2i vert[3], const RGB24& color);
	void FillTriangle(const Vec2i vert[3], const RGB24& color);
	void DrawTriangle(const Vec2i vert[3], const RGB24 color[3]);
	void FillTriangle(const Vec2i vert[3], const RGB24 color[3]);

private:
	void DrawLine(const Vec2i& vert0, const Vec2i& vert1, const RGB24& color);
	void DrawLine(const Vec2i& vert0, const RGB24& color0, const Vec2i& vert1, const RGB24& color1);
	
private:
	std::shared_ptr<class Framebuffer> m_pFramebuffer;
};