#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

#include "Model.h"

Model::Model(const char* filename)
{
	std::ifstream fileStream;
	std::string   readLine;
	char type, trash;

	fileStream.open(filename, std::ifstream::in);

	if (fileStream.fail()) throw std::runtime_error("Failed to Open obj File");

	std::cout << "Read .obj File : " << filename << '\n';

	while (!fileStream.eof())
	{
		std::getline(fileStream, readLine);
		std::istringstream iss(readLine.c_str());

		if (!readLine.compare(0, 2, "v "))
		{
			Vec3f v;

			iss >> type >> v.x >> v.y >> v.z;
			m_Positions.push_back(v);
		}
		else if (!readLine.compare(0, 3, "vn "))
		{
			Vec3f n;

			iss >> type >> type >> n.x >> n.y >> n.z;
			m_Normals.push_back(n);
		}
		else if (!readLine.compare(0, 3, "vt "))
		{
			Vec3f t;

			iss >> type >> type >> t.x >> t.y >> t.z;
			m_Textures.push_back(t);
		}
		else if (!readLine.compare(0, 2, "f "))
		{
			if (m_Textures.size() == 0)
			{
				std::vector<Vertex> vertices;
				int iv;
				int inorm;

				iss >> type;

				for (int i = 0; i < 3; ++i)
				{
					iss >> iv >> trash >> trash >> inorm;

					iv = iv > 0 ? iv : -iv;
					inorm = inorm > 0 ? inorm : -inorm;

					--iv;
					--inorm;

					vertices.push_back(Vertex(m_Positions[iv], m_Normals[inorm], Vec3f(0.0f, 0.0f, 0.0f)));
				}

				m_Face.push_back(vertices);
			}
			else if (m_Normals.size() == 0)
			{
				std::vector<Vertex> vertices;
				int iv;
				int iuv;

				iss >> type;

				for (int i = 0; i < 3; ++i)
				{
					iss >> iv >> trash >> iuv;

					iv = iv > 0 ? iv : -iv;
					iuv = iuv > 0 ? iuv : -iuv;

					--iv;
					--iuv;

					vertices.push_back(Vertex(m_Positions[iv], Vec3f(0.0f, 0.0f, 0.0f), m_Textures[iuv]));
				}

				m_Face.push_back(vertices);
			}
			else
			{
				std::vector<Vertex> vertices;
				int iv;
				int iuv;
				int inorm;

				iss >> type;

				for (int i = 0; i < 3; ++i)
				{
					iss >> iv >> trash >> iuv >> trash >> inorm;

					iv = iv > 0 ? iv : -iv;
					iuv = iuv > 0 ? iuv : -iuv;
					inorm = inorm > 0 ? inorm : -inorm;

					--iv;
					--iuv;
					--inorm;

					vertices.push_back(Vertex(m_Positions[iv], m_Normals[inorm], m_Textures[iuv]));
				}

				m_Face.push_back(vertices);
			}
		}
	}
}

Model::~Model()
{

}

Vec3f Model::GetPosition(int index)
{
	assert(0 <= index && index < m_Positions.size());
	return m_Positions[index];
}

Vec3f Model::GetNormal(int index)
{
	assert(0 <= index && index < m_Normals.size());
	return m_Normals[index];
}

Vec3f Model::GetTexture(int index)
{
	assert(0 <= index && index < m_Textures.size());
	return m_Textures[index];
}

std::vector<Vertex> Model::GetFace(int index)
{
	assert(0 <= index && index < m_Face.size());
	return m_Face[index];
}