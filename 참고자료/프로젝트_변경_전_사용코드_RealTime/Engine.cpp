#include "Timer.h"
#include "DIBSection.h"
#include "Vertex.h"
#include "Model.h"
#include "Math.h"
#include "FlatShader.h"
#include "Geometry.h"
#include "Transform.h"
#include "Rasterization.h"
#include "Engine.h"

Engine::Engine()
	: m_BufferBgRed(0), m_BufferBgGreen(0), m_BufferBgBlue(0)
	, m_bIsDoneLoop(false)
	, m_pDIBSection(nullptr)
{

}

Engine::~Engine()
{
}

bool Engine::InitializeEngine(LPCWSTR title, int iWndWidth, int iWndHeight, BYTE red, BYTE green, BYTE blue, bool isRemoveTitleBar, bool isFullScreen)
{
	m_BufferBgRed   = red;
	m_BufferBgGreen = green;
	m_BufferBgBlue  = blue;

	if (InitializeWindowApplication(title, iWndWidth, iWndHeight, isRemoveTitleBar, isFullScreen))
	{
		m_pTimer         = std::make_shared<Timer>();
		m_pDIBSection    = std::make_shared<DIBSection>(m_hWnd);
		m_pRasterization = std::make_shared<Rasterization>(m_pDIBSection);
		m_pModel         = std::make_shared<Model>("resource/african_head.obj");
		m_pShader        = std::make_shared<FlatShader>();

		m_Eye    = Vector3f(0.0f, 0.0f, 5.0f);
		m_Target = Vector3f(0.0f, 0.0f, 0.0f);

		m_Model = Scale(0.4f, 0.4f, 0.4f);
		m_View  = LookAt(m_Eye, m_Target, Vector3f(0.0f, 1.0f, 0.0f));
		m_Projection.Identity();
		m_Viewport = Viewport(0, 0, m_iWndWidth, m_iWndHeight);
	}

	return m_bIsInitialize;
}

void Engine::ResizeEngineWindow(void)
{
	if (ResizeWindowApplication())
	{
		m_pDIBSection->DestroyDIBSection();
		m_pDIBSection->GenerateDIBSection();
	}
}

int Engine::Run(void)
{
	if (m_bIsInitialize)
	{
		ShowWindowApplication(true);
		return MainLoop();
	}
	else
	{
		return -1;
	}
}

int Engine::MainLoop(void)
{
	MSG msg;
	memset(&msg, 0, sizeof(MSG));

	m_pTimer->Start();

	while (!m_bIsDoneLoop)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				m_bIsDoneLoop = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ProcessInput();
		UpdateLoop();
		GenerateOutput();

		m_pTimer->Tick();
	}

	return static_cast<int>(msg.wParam);
}

void Engine::ProcessInput(void)
{
	ResizeEngineWindow();
	m_pDIBSection->Clear(m_BufferBgRed, m_BufferBgGreen, m_BufferBgBlue);
	m_pDIBSection->ClearZBuffer();

	if (GetAsyncKeyState(VK_ESCAPE) && 0x8000 || GetAsyncKeyState(VK_ESCAPE) && 0x8001)
	{
		m_bIsDoneLoop = true;
	}

	if (GetAsyncKeyState(VK_LEFT) && 0x8000 || GetAsyncKeyState(VK_LEFT) && 0x8001)
	{
		m_Eye.x -= 0.1f;
		m_View = LookAt(m_Eye, m_Target, Vector3f(0.0f, 1.0f, 0.0f));
	}

	if (GetAsyncKeyState(VK_RIGHT) && 0x8000 || GetAsyncKeyState(VK_RIGHT) && 0x8001)
	{
		m_Eye.x += 0.1f;
		m_View = LookAt(m_Eye, m_Target, Vector3f(0.0f, 1.0f, 0.0f));
	}

	if (GetAsyncKeyState(VK_UP) && 0x8000 || GetAsyncKeyState(VK_UP) && 0x8001)
	{
		m_Eye.y += 0.1f;
		m_View = LookAt(m_Eye, m_Target, Vector3f(0.0f, 1.0f, 0.0f));
	}

	if (GetAsyncKeyState(VK_DOWN) && 0x8000 || GetAsyncKeyState(VK_DOWN) && 0x8001)
	{
		m_Eye.y -= 0.1f;
		m_View = LookAt(m_Eye, m_Target, Vector3f(0.0f, 1.0f, 0.0f));
	}
}

void Engine::UpdateLoop(void)
{
	m_pShader->SetModel(m_Model);
	m_pShader->SetView(m_View);
	m_pShader->SetProjection(m_Projection);
	m_pShader->SetViewport(m_Viewport);

	/* flat shader 
	for (int i = 0; i < m_pModel->CountOfFace(); ++i)
	{
		std::vector<Vertex> face = m_pModel->GetFace(i);
		std::vector<Vector3f> world;
		std::vector<Vector3f> screen;

		for (int j = 0; j < 3; ++j)
		{
			Vertex vertex = face[j];
			Vector3f v = vertex.GetPosition();

			screen.push_back(m_pShader->VertexShader(v));
			world.push_back(v);
		}

		Vector3f n = Cross(world[1] - world[0], world[2] - world[0]);
		Vector3f look = m_Target - m_Eye;

		n.Normalize();
		look.Normalize();

		float theta = Dot(n, look);

		if (theta <= 0.0f)
		{
			theta = std::abs(theta);
			theta = Clamp(theta, 0.0f, 1.0f);

			m_pRasterization->FillTriangle(
				Vector2i(screen[0].x, screen[0].y), screen[0].z,
				Vector2i(screen[1].x, screen[1].y), screen[1].z,
				Vector2i(screen[2].x, screen[2].y), screen[2].z,
				RGBColor(theta * 1.0f, theta * 1.0f, theta * 1.0f)
			);
		}
	}
	*/

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
		Vector3f look = m_Target - m_Eye;

		n.Normalize();
		look.Normalize();

		float theta = Dot(n, look);

		if (theta < 0.0f)
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
}

void Engine::GenerateOutput(void)
{
	SetDisplayTextColor(255, 255, 255);
	DisplayFPS(0, 0);
	m_pDIBSection->Render();
}

void Engine::DisplayFPS(int x, int y)
{
	wchar_t fps[50];

	swprintf(fps, L"FPS : %.3f", 1.0f / m_pTimer->DeltaTime());
	TextOut(m_pDIBSection->GetMemoryDC(), x, y, fps, static_cast<int>(wcslen(fps)));
}

void Engine::SetDisplayTextColor(BYTE red, BYTE green, BYTE blue)
{
	SetTextColor(m_pDIBSection->GetMemoryDC(), RGB(red, green, blue));
	SetBkMode(m_pDIBSection->GetMemoryDC(), TRANSPARENT);
}