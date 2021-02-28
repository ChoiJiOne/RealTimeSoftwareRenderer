#pragma once

#include <memory>
class ScreenSurface;
class Rasterization;
class Model;
class FlatShader;

class Renderer
{
public:
	Renderer(int width, int height, int channels, unsigned char bgRed, unsigned char bgGreen, unsigned char bgBlue);
	virtual ~Renderer();

	void RenderScene(const char* savePath);

private:
	std::shared_ptr<Model>         m_pModel;
	std::shared_ptr<FlatShader>    m_pShader;
	std::shared_ptr<ScreenSurface> m_pSurface;
	std::shared_ptr<Rasterization> m_pRasterization;
};