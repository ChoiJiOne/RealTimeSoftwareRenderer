#pragma once

#include <vector>
#include "Vertex.h"
#include "Vector.h"

class Model
{
public:
	Model(const char* filename);
	~Model();

	int CountOfPosition(void) const;
	int CountOfNormal(void)   const;
	int CountOfTexture(void)  const;
	int CountOfFace(void)     const;

	Vector3f GetPosition(int idx);
	Vector3f GetNormal(int idx);
	Vector3f GetTexture(int idx);
	std::vector<Vertex> GetFace(int idx);

private:
	std::vector<Vector3f>            m_Position;
	std::vector<Vector3f>            m_Normal;
	std::vector<Vector3f>            m_Texture;
	std::vector<std::vector<Vertex>> m_Face;
};