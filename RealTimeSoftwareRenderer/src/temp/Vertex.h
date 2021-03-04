#pragma once

#include "Vector.h"

class Vertex
{
public:
	Vertex(const Vec3f& position, const Vec3f& normal, const Vec3f& texture) : m_Position(position), m_Normal(normal), m_Texture(texture) {}

	void SetPosition(const Vec3f& position) { m_Position = position; }
	void SetNormal(const Vec3f& normal)     { m_Normal = normal; }
	void SetTexture(const Vec3f& texture)   { m_Texture = texture; }

	Vec3f GetPosition(void) const { return m_Position; }
	Vec3f GetNormal(void)   const { return m_Normal; }
	Vec3f GetTexture(void)  const { return m_Texture; }

private:
	Vec3f m_Position;
	Vec3f m_Normal;
	Vec3f m_Texture;
};