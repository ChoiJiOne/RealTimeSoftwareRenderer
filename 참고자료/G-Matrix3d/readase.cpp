/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

#include "types.h"
#include "software.h"
#include "file.h"

static char line[256];
static char string[80];
static int linecount = 0;
static int GEOMOBJECTCOUNT = 0;
static TEXCOORDFLOAT * TVertexList;
static int MAXTVERTEX, MAXTVFACES;
static int VertexCounter, FaceCounter, TFaceCounter;
static int LastIndex;

BOOL bIsASE;
BOOL NormalFlag;

BOOL LoadASE(HWND hwnd);
BOOL ReadASE(char *fname);

static int DecodeASE(FILE *fp);
static int DecodeSCENE(FILE *fp);
static int DecodeMATERIAL_LIST(FILE *fp);
static int DecodeGEOMOBJECT(FILE *fp);
static int DecodeMESH(FILE *fp);
static int DecodeMESH_VERTEX_LIST(FILE *fp);
static int DecodeMESH_FACE_LIST(FILE *fp);
static int DecodeMESH_TVERTLIST(FILE *fp);
static int DecodeMESH_TFACELIST(FILE *fp);
static int DecodeMESH_CVERTEX(FILE *fp);
static int DecodeMESH_NORMALS(FILE *fp);

BOOL LoadASE(HWND hwnd)
{
	char fname[80];
	ASEFileOpenDlg(hwnd, fname);
	return (ReadASE(fname));
}

BOOL ReadASE(char *fname)
{
	FILE *fp;

	fp = fopen(fname, "rt");
	if ( fp == NULL )
	{
		return FALSE;
	}
	
	linecount = 0;
	GEOMOBJECTCOUNT = 0;
	VertexCounter = 0;
	FaceCounter = 0;
	TFaceCounter = 0;
	LastIndex = 0;
	MAXVERTEX = 0;
	MAXTRIANGLE = 0;
	MAXTVERTEX = 0, 
	MAXTVFACES = 0;
	NormalFlag = FALSE;

	if (VertexList)	
	{
		free(VertexList); 
		VertexList = NULL;
	}
	
	if (ViewVertexList)
	{
		free(ViewVertexList);
		ViewVertexList = NULL;
	}
	
	if (ScreenVertexList)	
	{
		free(ScreenVertexList);
		ScreenVertexList = NULL;
	}
	
	if (TriangleList)
	{
		free(TriangleList);
		TriangleList = NULL;
	}
	
	if (MeshList) 
	{
		free(MeshList);
		MeshList = NULL;
	}

	DecodeASE(fp);
	
	if (NormalFlag == FALSE)
	{
		MakeFaceNormalFromWorldVertex();
		MakeVertexNormalFromFaceNormal();
	}
	
	fclose(fp);

	return TRUE;
}

int DecodeASE(FILE *fp)
{
	linecount++;
	fgets(line, 256, fp);
	sscanf(line, "%s", string);

	// CHECK ASE FILE
	if (_stricmp(string, "*3DSMAX_ASCIIEXPORT") != 0) return -1;

	while ( !feof(fp) )
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		
		if (_stricmp(string, "*COMMENT") == 0) continue;
		
		///////////////////////
		// Scene Descriptions
		if (_stricmp(string, "*SCENE") == 0)
			DecodeSCENE(fp);
		
		///////////////////
		// material list
		//if (_stricmp(string, "*MATERIAL_LIST") == 0)
		//	DecodeMATERIAL_LIST(fp);
		
		////////////////////
		// geometry object
		if (_stricmp(string, "*GEOMOBJECT") == 0)
			DecodeGEOMOBJECT(fp);
	}
	
	return linecount;	
}

int DecodeSCENE(FILE *fp)
{
	linecount += 8;
	
	fgets(line, 256, fp);//*SCENE_FILENAME	
	fgets(line, 256, fp);//*SCENE_FIRSTFRAME	
	fgets(line, 256, fp);//*SCENE_LASTFRAME	
	fgets(line, 256, fp);//*SCENE_FRAMESPEED	
	fgets(line, 256, fp);//*SCENE_TICKSPERFRAME	
	fgets(line, 256, fp);//*SCENE_BACKGROUND_STATIC	
	fgets(line, 256, fp);//*SCENE_AMBIENT_STATIC	
	fgets(line, 256, fp);//}
	
	return 0;
}
		
int DecodeMATERIAL_LIST(FILE *fp)
{
	do {
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
	} while (_stricmp(string, "}") != 0);

	return 0;
}
		
int DecodeGEOMOBJECT(FILE *fp)
{
	GEOMOBJECTCOUNT++;
	linecount += 17;
	fgets(line, 256, fp);//*NODE_NAME
	fgets(line, 256, fp);//*NODE_TM {
	fgets(line, 256, fp);//*NODE_NAME
	fgets(line, 256, fp);//*INHERIT_POS
	fgets(line, 256, fp);//*INHERIT_ROT
	fgets(line, 256, fp);//*INHERIT_SCL
	fgets(line, 256, fp);//*TM_ROW0
	fgets(line, 256, fp);//*TM_ROW1
	fgets(line, 256, fp);//*TM_ROW2
	fgets(line, 256, fp);//*TM_ROW3
	fgets(line, 256, fp);//*TM_POS
	fgets(line, 256, fp);//*TM_ROTAXIS
	fgets(line, 256, fp);//*TM_ROTANGLE 
	fgets(line, 256, fp);//*TM_SCALE
	fgets(line, 256, fp);//*TM_SCALEAXIS
	fgets(line, 256, fp);//*TM_SCALEAXISANG
	fgets(line, 256, fp);//}

	linecount++;
	fgets(line, 256, fp);
	sscanf(line, "%s", string);
	if (_stricmp(string, "*MESH") == 0)
		DecodeMESH(fp);
	return 0;
}

int DecodeMESH(FILE *fp)
{
	
	linecount++;
	fgets(line, 256, fp);//*TIMEVALUE
		
	int VCOUNT;
	linecount++;
	fgets(line, 256, fp);//*MESH_NUMVERTEX
	sscanf(line, "%s%d", string, &VCOUNT);
	MAXVERTEX += VCOUNT;

	///////////
	if (VertexList == NULL)
	{
		VertexList = (VERTEX*)malloc(MAXVERTEX * sizeof(VERTEX));
	}
	else
	{
		VertexList = (VERTEX*)realloc(VertexList, MAXVERTEX * sizeof(VERTEX));
	}

	if (ViewVertexList == NULL)
	{
		ViewVertexList = (VERTEX*)malloc(MAXVERTEX * sizeof(VERTEX));
	}
	else
	{
		ViewVertexList = (VERTEX*)realloc(ViewVertexList, MAXVERTEX * sizeof(VERTEX));
	}
	
	if (ScreenVertexList == NULL)
	{
		ScreenVertexList = (VERTEX*)malloc(MAXVERTEX * sizeof(VERTEX));
	}
	else
	{
		ScreenVertexList = (VERTEX*)realloc(ScreenVertexList, MAXVERTEX * sizeof(VERTEX));
	}

	int TCOUNT;
	linecount++;
	fgets(line, 256, fp);//*MESH_NUMFACES
	sscanf(line, "%s%d", string, &TCOUNT);
	MAXTRIANGLE += TCOUNT;
	///////////
	if (TriangleList == NULL)
	{
		TriangleList = (TRIANGLE*)malloc(MAXTRIANGLE * sizeof(TRIANGLE));
		memset(TriangleList, 0, MAXTRIANGLE * sizeof(TRIANGLE));
	}
	else 
	{
		TriangleList = (TRIANGLE*)realloc(TriangleList, MAXTRIANGLE * sizeof(TRIANGLE));
		//memset(TriangleList + (MAXTRIANGLE - TCOUNT) * sizeof(TRIANGLE),
		//	0, TCOUNT * sizeof(TRIANGLE));
	}

			
	linecount++;
	fgets(line, 256, fp);//*MESH_VERTEX_LIST
	sscanf(line, "%s", string);
	if ( _stricmp(string, "*MESH_VERTEX_LIST") == 0 )
		DecodeMESH_VERTEX_LIST(fp);

	linecount++;
	fgets(line, 256, fp);//*MESH_FACE_LIST
	sscanf(line, "%s", string);
	if ( _stricmp(string, "*MESH_FACE_LIST") == 0 )
		DecodeMESH_FACE_LIST(fp);
	//--------------------------------------------------
	//--------------------------------------------------
	while (1)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		if ( _stricmp(string, "*MESH_NUMTVERTEX") == 0 )//*MESH_NUMTVERTEX
		{
			DecodeMESH_TVERTLIST(fp);
			linecount++;
			fgets(line, 256, fp);//*MESH_NUMTVFACES
			DecodeMESH_TFACELIST(fp);
			continue;
		}	
		
		if ( _stricmp(string, "*MESH_NUMCVERTEX") == 0 )//*MESH_NUMCVERTEX
		{
			DecodeMESH_CVERTEX(fp);
			continue;
		}
		
		if ( _stricmp(string, "*MESH_NORMALS") == 0 )//*MESH_NORMALS
		{
			DecodeMESH_NORMALS(fp);
			continue;
		}

		if ( _stricmp(string, "}") == 0 ) return 0;
	}
	
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int DecodeMESH_VERTEX_LIST(FILE *fp)
{
	LastIndex = VertexCounter;
	while( VertexCounter < MAXVERTEX )
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_VERTEX
		sscanf(line, "%s%d%f%f%f", string, string, 
			&VertexList[VertexCounter].x, &VertexList[VertexCounter].z, &VertexList[VertexCounter].y);
		VertexList[VertexCounter].w = 1;
		VertexCounter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int DecodeMESH_FACE_LIST(FILE *fp)
{
	while( FaceCounter < MAXTRIANGLE )
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_FACE
		sscanf(line, "%s%s%s%d%s%d%s%d", string, string, 
			string, &TriangleList[FaceCounter].VertexIndex[0], 
			string, &TriangleList[FaceCounter].VertexIndex[2], 
			string, &TriangleList[FaceCounter].VertexIndex[1]);
		TriangleList[FaceCounter].VertexIndex[0] += LastIndex;
		TriangleList[FaceCounter].VertexIndex[2] += LastIndex;
		TriangleList[FaceCounter].VertexIndex[1] += LastIndex;
		FaceCounter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int DecodeMESH_TVERTLIST(FILE *fp)
{
	sscanf(line, "%s%d", string, &MAXTVERTEX);

	TVertexList = (TEXCOORDFLOAT*)malloc(MAXTVERTEX * sizeof(TEXCOORDFLOAT));

	linecount++;
	fgets(line, 256, fp);//*MESH_TVERTLIST
	
	int counter = 0;
	while( counter < MAXTVERTEX )
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_TVERT
		sscanf(line, "%s%d%f%f", string, string, 
			&TVertexList[counter].u, &TVertexList[counter].v);
		counter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int DecodeMESH_TFACELIST(FILE *fp)
{
	int TIndex[3];

	int TFACECOUNT;
	sscanf(line, "%s%d", string, &TFACECOUNT);
	MAXTVFACES += TFACECOUNT;

	linecount++;
	fgets(line, 256, fp);//*MESH_TFACELIST

	while( TFaceCounter < MAXTVFACES )
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_TFACE
		sscanf(line, "%s%d%d%d%d", string, string, 
			&TIndex[0], 
			&TIndex[1], 
			&TIndex[2]);

		TriangleList[TFaceCounter].VertexTexture[0].u = TVertexList[ TIndex[0] ].u; 
		TriangleList[TFaceCounter].VertexTexture[0].v = TVertexList[ TIndex[0] ].v; 
		TriangleList[TFaceCounter].VertexTexture[2].u = TVertexList[ TIndex[1] ].u; 
		TriangleList[TFaceCounter].VertexTexture[2].v = TVertexList[ TIndex[1] ].v; 
		TriangleList[TFaceCounter].VertexTexture[1].u = TVertexList[ TIndex[2] ].u; 
		TriangleList[TFaceCounter].VertexTexture[1].v = TVertexList[ TIndex[2] ].v; 

		TFaceCounter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	free(TVertexList);
	return 0;
}

int	DecodeMESH_CVERTEX(FILE *fp)
{
	return 0;
}

int DecodeMESH_NORMALS(FILE *fp)
{
	int counter = 0;	
	
	NormalFlag = TRUE;

	while ( counter < MAXTRIANGLE )
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_FACENORMAL
		sscanf(line, "%s%d%f%f%f", string, string,
			&TriangleList[counter].FaceNormal.x,
			&TriangleList[counter].FaceNormal.z,
			&TriangleList[counter].FaceNormal.y);

		linecount++;
		fgets(line, 256, fp);//*MESH_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, string,
			&TriangleList[counter].VertexNormal[0].x,
			&TriangleList[counter].VertexNormal[0].z,
			&TriangleList[counter].VertexNormal[0].y);

		linecount++;
		fgets(line, 256, fp);//*MESH_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, string,
			&TriangleList[counter].VertexNormal[2].x,
			&TriangleList[counter].VertexNormal[2].z,
			&TriangleList[counter].VertexNormal[2].y);
		
		linecount++;
		fgets(line, 256, fp);//*MESH_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, string,
			&TriangleList[counter].VertexNormal[1].x,
			&TriangleList[counter].VertexNormal[1].z,
			&TriangleList[counter].VertexNormal[1].y);

		counter++;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}
		
