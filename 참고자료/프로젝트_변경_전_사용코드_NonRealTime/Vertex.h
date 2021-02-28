#pragma once

#include "Vector.h"

class Vertex
{
public:
	Vertex(const Vector3f& position)                                                  : m_Position(position) {}
	Vertex(const Vector3f& position, const Vector3f& normal)                          : m_Position(position), m_Normal(normal) {}
	Vertex(const Vector3f& position, const Vector3f& normal, const Vector3f& texture) : m_Position(position), m_Normal(normal), m_Texture(texture) {}
	~Vertex() {}

	void SetPosition(const Vector3f& position) { m_Position = position; }
	void SetNormal(const Vector3f& normal)     { m_Normal = normal;     }
	void SetTexture(const Vector3f& texture)   { m_Texture = texture;   }

	Vector3f GetPosition(void) const { return m_Position; }
	Vector3f GetNormal(void)   const { return m_Normal;   }
	Vector3f GetTexture(void)  const { return m_Texture;  }

private:
	Vector3f m_Position;
	Vector3f m_Normal;
	Vector3f m_Texture;
};
