/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#ifndef OPENGL_H
#define OPENGL_H

#include <GL/gl.h>
#include <GL/glu.h>

HWND CreateOPENGLWindow(HINSTANCE hInstance, int nCmdShow);

void CloseOPENGLWindow(void);
void RenderSceneOPENGL(void);
void UpdateFrameOPENGL(void);

#endif