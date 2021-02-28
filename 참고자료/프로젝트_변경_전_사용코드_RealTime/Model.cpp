#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>
#include "Vector.h"
#include "Model.h"

Model::Model(const char* filename)
{
	std::ifstream fileStream;
	std::string readLine;
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
			Vector3f v;

			iss >> type >> v.x >> v.y >> v.z;
			m_Position.push_back(v);
		}
		else if (!readLine.compare(0, 3, "vn "))
		{
			Vector3f n;

			iss >> type >> type >> n.x >> n.y >> n.z;
			m_Normal.push_back(n);
		}
		else if (!readLine.compare(0, 3, "vt "))
		{
			Vector3f t;

			iss >> type >> type >> t.x >> t.y >> t.z;
			m_Texture.push_back(t);
		}
		else if (!readLine.compare(0, 2, "f "))
		{
			if (m_Texture.size() == 0)
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

					vertices.push_back(Vertex(m_Position[iv], m_Normal[inorm]));
				}

				m_Face.push_back(vertices);
			}
			else if (m_Normal.size() == 0)
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

					vertices.push_back(Vertex(m_Position[iv], Vector3f(0.0f, 0.0f, 0.0f), m_Texture[iuv]));
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

					vertices.push_back(Vertex(m_Position[iv], m_Normal[inorm], m_Texture[iuv]));
				}

				m_Face.push_back(vertices);
			}
		}
	}
}

Model::~Model()
{

}

int Model::CountOfPosition(void) const
{
	return static_cast<int>(m_Position.size());
}

int Model::CountOfNormal(void)   const
{
	return static_cast<int>(m_Normal.size());
}

int Model::CountOfTexture(void)  const
{
	return static_cast<int>(m_Texture.size());
}

int Model::CountOfFace(void) const
{
	return static_cast<int>(m_Face.size());
}

Vector3f Model::GetPosition(int idx)
{
	assert(0 <= idx && idx < m_Position.size());
	return m_Position[idx];
}

Vector3f Model::GetNormal(int idx)
{
	assert(0 <= idx && idx < m_Normal.size());
	return m_Normal[idx];
}

Vector3f Model::GetTexture(int idx)
{
	assert(0 <= idx && idx < m_Texture.size());
	return m_Texture[idx];
}

std::vector<Vertex> Model::GetFace(int idx)
{
	assert(0 <= idx && idx < m_Face.size());
	return m_Face[idx];
}