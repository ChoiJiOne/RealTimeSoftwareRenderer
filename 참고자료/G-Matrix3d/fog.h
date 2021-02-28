/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#ifndef FOG_H
#define FOG_H

extern BOOL FOG_ENABLE;

extern int FogColorR;
extern int FogColorG;
extern int FogColorB;

void SetFogColor(int r, int g, int b);
void FogLinear(VERTEX *vlist, float near, float far);

#endif
