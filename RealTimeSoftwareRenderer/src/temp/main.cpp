#include <iostream>
#include "Win32.h"
#include "Framebuffer.h"
#include "Model.h"
#include "Rasterizer.h"

int main(void)
{
	const int WIDTH = 800;
	const int HEIGHT = 800;
	BYTE color[3] = { 0, 0, 0 };

	Win32App app;
	bool isOk = app.InitializeWin32App(L"Win32 App", WIDTH, HEIGHT);
	std::shared_ptr<Framebuffer> buffer = std::make_shared<Framebuffer>(app.GetHWND());
	Rasterizer raster(buffer);
	Model model("resource/african_head.obj");

	if (isOk)
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (true)
		{
			buffer->Clear(color);

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			for (int i = 0; i < model.CountOfFace(); ++i)
			{
				std::vector<Vertex> face = model.GetFace(i);
				std::vector<Vec3f> screen;

				for (int j = 0; j < 3; ++j)
				{
					Vertex vertex0 = face[j % 3];
					Vertex vertex1 = face[(j + 1) % 3];

					Vec3f v0 = vertex0.GetPosition();
					Vec3f v1 = vertex1.GetPosition();

					Vec2i p[2];

					p[0] = Vec2i(
						static_cast<int>((v0.x + 1.0f) * WIDTH / 2.0f),
						static_cast<int>((v0.y + 1.0f) * HEIGHT / 2.0f)
					);

					p[1] = Vec2i(
						static_cast<int>((v1.x + 1.0f) * WIDTH / 2.0f),
						static_cast<int>((v1.y + 1.0f) * HEIGHT / 2.0f)
					);

					raster.DrawLine(p, RGB24(1.0f, 1.0f, 1.0f));
				}
			}

			buffer->Render();

			if (GetAsyncKeyState(VK_ESCAPE) && 0x8000 || GetAsyncKeyState(VK_ESCAPE) && 0x8001)
			{
				break;
			}
		}
	}

	return 0;
}