#pragma once

#include <vector>

#include "Vertex.h"
#include "Vector.h"

class Model
{
public:
	Model(const char* filename);
	virtual ~Model();

	int CountOfPosition(void) const { return static_cast<int>(m_Positions.size()); }
	int CountOfNormal(void)   const { return static_cast<int>(m_Normals.size()); }
	int CountOfTexture(void)  const { return static_cast<int>(m_Textures.size()); }
	int CountOfFace(void)     const { return static_cast<int>(m_Face.size()); }

	Vec3f GetPosition(int index);
	Vec3f GetNormal(int index);
	Vec3f GetTexture(int index);
	std::vector<Vertex> GetFace(int index);

private:
	std::vector<Vec3f> m_Positions;
	std::vector<Vec3f> m_Normals;
	std::vector<Vec3f> m_Textures;
	std::vector<std::vector<Vertex>> m_Face;
};