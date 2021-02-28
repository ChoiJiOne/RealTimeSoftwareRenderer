/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

#include "types.h"
#include "software.h"

BOOL FOG_ENABLE = TRUE;

int FogColorR;
int FogColorG;
int FogColorB;

void SetFogColor(int r, int g, int b);
void FogLinear(VERTEX *vlist, float near, float far);

void SetFogColor(int r, int g, int b)
{
	FogColorR = r>>3;
	FogColorG = g>>2;
	FogColorB = b>>3;
}

void FogLinear(VERTEX *vlist, float nearz, float farz)
{
	int i;
	float fog;

	for ( i = 0; i < MAXVERTEX; i++)
	{
		//if (vlist[i].tflag == FALSE ) continue;

		fog = (vlist[i].z - nearz) / (farz - nearz);
		
		if (fog > 1.0f) fog = 1.0f; 
		if (fog < 0.0f) fog = 0.0f;
		
		if (!FOG_ENABLE) fog = 0.0f; 
		
		vlist[i].fog_factor = fog;
	}
}
