#include "Image.h"
#include "ScreenSurface.h"
#include "Model.h"
#include "Math.h"
#include "Geometry.h"
#include "FlatShader.h"
#include "Transform.h"
#include "Rasterization.h"
#include "Renderer.h"

#include <iostream>

Renderer::Renderer(int width, int height, int channels, unsigned char bgRed, unsigned char bgGreen, unsigned char bgBlue)
{
	m_pSurface        = std::make_shared<ScreenSurface>(width, height, channels, bgRed, bgGreen, bgBlue);
	m_pRasterization = std::make_shared<Rasterization>(m_pSurface);
	m_pModel         = std::make_shared<Model>("resource/sphere.obj");
	m_pShader        = std::make_shared<FlatShader>();
}

Renderer::~Renderer()
{

}

void Renderer::RenderScene(const char* savePath)
{
	Vector3f eye = Vector3f(5.0f, 5.0f, 5.0f);
	Vector3f target = Vector3f(0.0f, 0.0f, 0.0f);

	Matrix4x4 model = Scale(0.4f, 0.4f, 0.4f);
	Matrix4x4 view  = LookAt(eye, target, Vector3f(0.0f, 1.0f, 0.0f));
	Matrix4x4 projection;
	projection.Identity();
	Matrix4x4 viewport = Viewport(0, 0, m_pSurface->GetWidth(), m_pSurface->GetHeight());

	m_pShader->SetModel(model);
	m_pShader->SetView(view);
	m_pShader->SetProjection(projection);
	m_pShader->SetViewport(viewport);

	m_pShader->SetLightColor(RGBColor(1.0f, 1.0f, 1.0f));

	for (int i = 0; i < m_pModel->CountOfFace(); ++i)
	{
		std::vector<Vertex> face = m_pModel->GetFace(i);
		std::vector<Vector3f> world;
		std::vector<Vector3f> screen;
		std::vector<Vector3f> normal;

		for (int j = 0; j < 3; ++j)
		{
			Vertex vertex = face[j];
			Vector3f v = vertex.GetPosition();
			Vector3f norm = vertex.GetNormal();

			screen.push_back(m_pShader->VertexShader(v));
			world.push_back(v);
			normal.push_back(norm);
		}

		Vector3f n = Cross(world[1] - world[0], world[2] - world[0]);
		Vector3f look = target - eye;
		n.Normalize();
		look.Normalize();

		m_pShader->SetLightDir(look);

		float theta = Dot(n, look);

		if(theta < 0.0f)
		{
			float theta0 = std::abs(Dot(normal[0], look));
			//theta0 = Clamp(theta0, 0.0f, 1.0f);

			float theta1 = std::abs(Dot(normal[1], look));
			//theta1 = Clamp(theta1, 0.0f, 1.0f);

			float theta2 = std::abs(Dot(normal[2], look));
			//theta2 = Clamp(theta2, 0.0f, 1.0f);

			m_pRasterization->FillTriangle(
				Vector2i(screen[0].x, screen[0].y), screen[0].z, RGBColor(theta0 * 1.0f, theta0 * 1.0f, theta0 * 1.0f),
				Vector2i(screen[1].x, screen[1].y), screen[1].z, RGBColor(theta1 * 1.0f, theta1 * 1.0f, theta1 * 1.0f),
				Vector2i(screen[2].x, screen[2].y), screen[2].z, RGBColor(theta2 * 1.0f, theta2 * 1.0f, theta2 * 1.0f)
			);
		}
	}

	m_pSurface->Save(savePath);
}