/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#ifndef TYPES_H
#define TYPES_H

#include <windows.h>

#ifndef PI
#define PI 3.1415926535897932384626f
#endif

typedef int FIXED16;

typedef struct tagPOINT2D {
	int x, y;
} POINT2D;

typedef struct tagPOINT3D {
	float x, y, z;
} POINT3D;

typedef struct tagTRISTRUCT {
	POINT3D Vertex[3];
} TRISTRUCT;

typedef struct tagCOLORf24 {
	float r, g, b;
} COLORf24;

typedef struct tagCOLORi24 {
	int r, g, b;
} COLORi24;

typedef struct tagTEXCOORD {
	int u, v, oz;
} TEXCOORD;

typedef struct tagTEXCOORDFLOAT {
	union {
		float u;
		float uz; 
	};
	union {
		float v;
		float vz;
	};
	float oz;
} TEXCOORDFLOAT;

typedef struct tagGPCOORD {
	float x, y, z, w;
} GPCOORD;

typedef struct tagGPVECTOR {
	float x, y, z, w;
} GPVECTOR;

typedef struct tagVERTEX {
	float x, y, z, w;
	float r, g, b, a;
	float nx, ny, nz;
	float u, v;
	float fog_factor;
	BOOL tflag;
} VERTEX;

typedef struct tagTRIANGLE {
	int VertexIndex[3];
	COLORf24 VertexColor[3];
	COLORf24 FaceColor;
	POINT3D VertexNormal[3];
	POINT3D FaceNormal;
	TEXCOORDFLOAT VertexTexture[3];
	BOOL IsVisible;
} TRIANGLE;

typedef struct tagMESH {
	char Name[20];
	int NumVertex;
	int NumTriangle;
	VERTEX * VertexList;
	int	* VIndexList;
	TRIANGLE * TriangleList;
} MESH;

typedef struct tagMODEL {
	char Name[20];
	int NumMesh;
	MESH * MeshList;
} MODEL;

typedef struct tagENTITY {
	char Name[20];
	MODEL * pModel;
	POINT3D Position;
	POINT3D Angle;
} ENTITY;

typedef struct tagLIGHT {
	int Type;
	POINT3D Location;
	POINT3D Direction;
	COLORf24 Intensity;
} LIGHT;

#endif