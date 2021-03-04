#pragma once

#include <memory>

#include "WindowApplication.h"
#include "Geometry.h"

class Timer;
class DIBSection;
class Rasterization;
class FlatShader;
class Model;

class Engine : public WindowApplication
{
public:
	Engine();
	virtual ~Engine();

public:
	bool InitializeEngine(LPCWSTR title, int iWndWidth, int iWndHeight, BYTE bgRed,  BYTE bgGreen,  BYTE bgBlue, bool isRemoveTitleBar = false, bool isFullScreen = false);

	void ResizeEngineWindow(void);

	int  Run(void);
	virtual int  MainLoop(void);
	virtual void ProcessInput(void);
	virtual void UpdateLoop(void);
	virtual void GenerateOutput(void);

public:
	void DisplayFPS(int x, int y);
	void SetDisplayTextColor(BYTE red, BYTE green, BYTE blue);

protected:
	BYTE m_BufferBgRed;
	BYTE m_BufferBgGreen;
	BYTE m_BufferBgBlue;
	bool m_bIsDoneLoop;
	std::shared_ptr<Timer>         m_pTimer;
	std::shared_ptr<Model>         m_pModel;
	std::shared_ptr<FlatShader>    m_pShader;
	std::shared_ptr<DIBSection>    m_pDIBSection;
	std::shared_ptr<Rasterization> m_pRasterization;

private:
	Vector3f m_Eye;
	Vector3f m_Target;

	Matrix4x4 m_Model;
	Matrix4x4 m_View;
	Matrix4x4 m_Projection;
	Matrix4x4 m_Viewport;
};