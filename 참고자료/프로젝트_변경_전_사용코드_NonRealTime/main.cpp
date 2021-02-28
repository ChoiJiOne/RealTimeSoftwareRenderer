#include "Image.h"
#include "Renderer.h"


int main(void)
{
	const int width = 800;
	const int height = 800;

	Renderer render(width, height, 3, 0, 0, 0);

	render.RenderScene("image/GouraudShaderScene4.png");

	return 0;
}

/*
#include "Model.h"
#include <iostream>

int main(void)
{
	Model model("resource/african_head.obj");

	for (int i = 0; i < model.CountOfFace(); ++i)
	{
		std::vector<Vertex> face = model.GetFace(i);
		std::vector<Vector3f> world;
		std::vector<Vector3f> normal;
		std::vector<Vector3f> texture;

		for (int j = 0; j < 3; ++j)
		{
			world.push_back(face[j].GetPosition());
			normal.push_back(face[j].GetNormal());
			texture.push_back(face[j].GetTexture());
		}

		std::cout << "v : ";
		for (int i = 0; i < 3; ++i)
		{
			std::cout << "(" << world[i].x << ", " << world[i].y << ", " << world[i].z << ")";
		}
		std::cout << '\n';

		std::cout << "n : ";
		for (int i = 0; i < 3; ++i)
		{
			std::cout << "(" << normal[i].x << ", " << normal[i].y << ", " << normal[i].z << ")";
		}
		std::cout << '\n';

		std::cout << "t : ";
		for (int i = 0; i < 3; ++i)
		{
			std::cout << "(" << texture[i].x << ", " << texture[i].y << ", " << texture[i].z << ")";
		}
		std::cout << '\n';
	}

	return 0;
}
*/
