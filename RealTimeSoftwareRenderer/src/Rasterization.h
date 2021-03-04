#pragma once

#include <memory>
#include "Vector.h"
#include "Color.h"

class DIBSection;

class Rasterization
{
public:
	Rasterization(std::shared_ptr<DIBSection> image);
	virtual ~Rasterization();

	void DrawLine(const Vector2i& v0, const Vector2i& v1, const RGBColor& c);
	void DrawLine(const Vector2i& v0, float z0, const Vector2i& v1, float z1, const RGBColor& c);
	void DrawLine(const Vector2i& v0, const RGBColor& c0, const Vector2i& v1, const RGBColor& c1);
	void DrawLine(const Vector2i& v0, float z0, const RGBColor& c0, const Vector2i& v1, float z1, const RGBColor& c1);

	void DrawTriangle(const Vector2i& v0, const Vector2i& v1, const Vector2i& v2, const RGBColor& c);
	void DrawTriangle(const Vector2i& v0, float z0, const Vector2i& v1, float z1, const Vector2i& v2, float z2, const RGBColor& c);
	void DrawTriangle(const Vector2i& v0, const RGBColor& c0, const Vector2i& v1, const RGBColor& c1, const Vector2i& v2, const RGBColor& c2);
	void DrawTriangle(const Vector2i& v0, float z0, const RGBColor& c0, const Vector2i& v1, float z1, const RGBColor& c1, const Vector2i& v2, float z2, const RGBColor& c2);

	void FillTriangle(const Vector2i& v0, const Vector2i& v1, const Vector2i& v2, const RGBColor& c);
	void FillTriangle(const Vector2i& v0, float z0, const Vector2i& v1, float z1, const Vector2i& v2, float z2, const RGBColor& c);
	void FillTriangle(const Vector2i& v0, const RGBColor& c0, const Vector2i& v1, const RGBColor& c1, const Vector2i& v2, const RGBColor& c2);
	void FillTriangle(const Vector2i& v0, float z0, const RGBColor& c0, const Vector2i& v1, float z1, const RGBColor& c1, const Vector2i& v2, float z2, const RGBColor& c2);

	void DrawCircle(const Vector2i& center, int radius, const RGBColor& c);
	void FillCircle(const Vector2i& center, int radius, const RGBColor& c);

private:
	void DrawHLine(int x0, int x1, int y, const RGBColor& c);
	void DrawHLine(int x0, float z0, int x1, float z1, int y, const RGBColor& c);
	void DrawHLine(int x0, int x1, int y, const RGBColor& c0, const RGBColor& c1);
	void DrawHLine(int x0, float z0, int x1, float z1, int y, const RGBColor& c0, const RGBColor& c1);
	void SortVertexInfo(Vector2i& top, Vector2i& middle, Vector2i& bottom);
	void SortVertexInfo(Vector2i& top, float& topz, Vector2i& middle, float& middlez, Vector2i& bottom, float& bottomz);
	void SortVertexInfo(Vector2i& topv, RGBColor& topc, Vector2i& middlev, RGBColor& middlec, Vector2i& bottomv, RGBColor& bottomc);
	void SortVertexInfo(Vector2i& topv, RGBColor& topc, float& topz, Vector2i& middlev, RGBColor& middlec, float& middlez, Vector2i& bottomv, RGBColor& bottomc, float& bottomz);

private:
	std::shared_ptr<DIBSection> m_pDIBSection;
};
