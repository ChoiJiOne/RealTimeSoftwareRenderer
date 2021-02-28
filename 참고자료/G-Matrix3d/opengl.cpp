/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"
#include <GL\gl.h>
#include <GL\glu.h>

#include "resource.h"
#include "software.h"
#include "matrix.h"
#include "readase.h"
#include "bmp.h"
#include "file.h"
#include "fog.h"

static HWND hWnd;
static HDC hDC;
static HGLRC hRC;				/* opengl context */

void RenderSceneOPENGL(void);
void UpdateFrameOPENGL(void);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateOPENGLWindow(HINSTANCE hInstance, int nCmdShow);
void CloseOPENGLWindow(void);

HWND CreateOPENGLWindow(HINSTANCE hInstance, int nCmdShow)
{
    int         pf;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;

    /* only register the window class once - use hInstance as a flag. */
	wc.style         = 0;
	wc.lpfnWndProc   = (WNDPROC)WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = "OpenGL";

	if (!RegisterClass(&wc)) {
	    MessageBox(NULL, "RegisterClass() failed:  "
		       "Cannot register window class.", "Error", MB_OK);
	    return NULL;
	}

    RECT rect = {0, 0, 639, 479};

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
	int width = rect.right - rect.left + 1;
	int height = rect.bottom - rect.top + 1;
	
    hWnd = CreateWindow("OpenGL", "OpenGL", WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - height) / 2,
			width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
		   "Error", MB_OK);
	return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 16;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
	MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
	return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	MessageBox(NULL, "SetPixelFormat() failed:  "
		   "Cannot set format specified.", "Error", MB_OK);
	return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hWnd, nCmdShow);
	
	// load texture file
	pTexture = LoadBMP24("check.bmp", 24);
	if (pTexture == NULL) 
	{
		MessageBox(hWnd, "Cannot find Texture check.bmp", "Error", MB_OK);
		CloseOPENGLWindow();
		return 0;
	}
	
	SetupWorld();

	/////////////////////////////////////
	// Setup Texture
	/////////////////////////////////////
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 256, 256,
		      GL_BGR_EXT, GL_UNSIGNED_BYTE, pTexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	
    float fogcolor[4];
	fogcolor[0] = 0.0f;
	fogcolor[1] = 0.0f;
	fogcolor[2] = 1.0f;
	fogcolor[3] = 0.0f;
	glFogfv(GL_FOG_COLOR, fogcolor);
	
	//glFogi(GL_FOG_MODE, GL_LINEAR);
	//glFogf(GL_FOG_START, 140);
	//glFogf(GL_FOG_END, 150);
	
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.002f);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glEnable(GL_FOG);
	
	//glFrontFace(GL_CW);
    //glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE);
	
	//glDepthFunc(GL_GREATER);
	//glClearDepth(0);
	glEnable(GL_DEPTH_TEST);

	//glShadeModel(GL_FLAT);
	
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

	//glEnable(GL_SCISSOR_TEST);
	//glScissor(50, 50, 640, 480);
	
	GetClientRect(hWnd, &rect);
	glViewport(0, 0, rect.right - rect.left + 1, rect.bottom - rect.top + 1);
	glDepthRange(0, 1);
	
	bIsActive = TRUE;
    
	return hWnd;
}    

void CloseOPENGLWindow(void)
{
    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hWnd, hDC);
    wglDeleteContext(hRC);

	CloseWorld();
}

void UpdateFrameOPENGL(void)
{
	if (bIsASE == FALSE) return;

	RenderSceneOPENGL();
	
	SwapBuffers(hDC);
	
	int frames = GetFrames();
	SetFrames(++frames);
}

void RenderSceneOPENGL(void)
{
	if (FOG_ENABLE)
		glEnable(GL_FOG);
	else
		glDisable(GL_FOG);

	
	BackFaceCullingAtModel();
	VertexShadingAtModel();
	
	SetActiveTM(CURRENTTM);
	LoadIdentity();
	
	RotateX(ModelAngle.x);
	RotateY(ModelAngle.y);
	RotateZ(ModelAngle.z);
	Translate(ModelCoord.x, ModelCoord.y, ModelCoord.z);
	
	Translate(-ViewCoord.x, -ViewCoord.y, -ViewCoord.z);
	RotateZ(-ViewAngle.z);
	RotateY(-ViewAngle.y);
	RotateX(-ViewAngle.x);
	//////////////////////////////////////////////////
	
	Scale(1, 1, -1);
	
	//////////////////////////////////////////////////
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 640.0f/480.0f, 50, 1000);

	//oglPerspectiveFOV(640, 480, 320, 100, 1000);

	for (int i = 0; i < MAXVERTEX; i++)
	{
		if (VertexList[i].tflag == FALSE) continue;
		TransformCTM(&ViewVertexList[i], &VertexList[i]);
	}
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (i = 0; i < MAXTRIANGLE; i++)
	{
		
		if (TriangleList[i].IsVisible == FALSE) continue;

		int i0 = TriangleList[i].VertexIndex[0];
		int i1 = TriangleList[i].VertexIndex[1];
		int i2 = TriangleList[i].VertexIndex[2];
		
		float i255 = 1.0f / 255.0f;
		float R0 = (float)TriangleList[i].VertexColor[0].r * i255;
		float G0 = (float)TriangleList[i].VertexColor[0].g * i255;
		float B0 = (float)TriangleList[i].VertexColor[0].b * i255;
		float R1 = (float)TriangleList[i].VertexColor[1].r * i255;
		float G1 = (float)TriangleList[i].VertexColor[1].g * i255;
		float B1 = (float)TriangleList[i].VertexColor[1].b * i255;
		float R2 = (float)TriangleList[i].VertexColor[2].r * i255;
		float G2 = (float)TriangleList[i].VertexColor[2].g * i255;
		float B2 = (float)TriangleList[i].VertexColor[2].b * i255;

		glBegin(GL_TRIANGLES);
			glColor3f(R0, G0, B0);
			glTexCoord2fv((float*)&TriangleList[i].VertexTexture[0]);
			glVertex4fv((float*)&ViewVertexList[i0]);

			glColor3f(R1, G1, B1);
			glTexCoord2fv((float*)&TriangleList[i].VertexTexture[1]);
			glVertex4fv((float*)&ViewVertexList[i1]);
			
			glColor3f(R2, G2, B2);
			glTexCoord2fv((float*)&TriangleList[i].VertexTexture[2]);
			glVertex4fv((float*)&ViewVertexList[i2]);
		glEnd();
	}
	glFlush();
}
