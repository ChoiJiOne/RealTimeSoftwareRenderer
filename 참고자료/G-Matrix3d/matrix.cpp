/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

#include "types.h"

#define CURRENTTM	0
#define WORLDTM		1
#define VIEWTM		2
#define PROJECTTM	3

typedef struct tagMATRIX{
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
} MATRIX;

MATRIX CTM, WTM, VTM, PTM;
MATRIX *pTM = &CTM;
int TMmode = 0;

void SetActiveTM(int mode);
int GetActiveTM(void);
void MatrixMult(MATRIX *M1, MATRIX *M2, MATRIX *M3);
static void SetIdentity(MATRIX *mat);
void LoadIdentity(void);
void Translate(float dx, float dy, float dz);
void RotateX(float theta);
void RotateY(float theta);
void RotateZ(float theta);
void Scale(float sx, float sy, float sz);
void Perspective(float d);
void PerspectiveFOV(float width, float height, float d, float nearz, float farz);
void oglPerspectiveFOV(float width, float height, float d, float nearz, float farz);
void Transform(VERTEX *p1, VERTEX *p2, MATRIX *mat);
void TransformCTM(VERTEX *p1, VERTEX *p2);
void TransformWTM(VERTEX *p1, VERTEX *p2);
void TransformVTM(VERTEX *p1, VERTEX *p2);
void TransformPTM(VERTEX *p1, VERTEX *p2);

void SetActiveTM(int mode)
{
	switch(mode)
	{
		case CURRENTTM:
			pTM = &CTM;
			break;
		
		case WORLDTM:
			pTM = &WTM;
			break;
		
		case VIEWTM:
			pTM = &VTM;
			break;
		
		case PROJECTTM:
			pTM = &PTM;
			break;
	}

	TMmode = mode;
}

int GetActiveTM(void)
{	
	return TMmode;
}

void MatrixMult(MATRIX *M1, MATRIX *M2, MATRIX *M3)
{
	MATRIX mat;
	
	mat.m11 = M2->m11 * M3->m11 + M2->m12 * M3->m21 + M2->m13 * M3->m31 + M2->m14 * M3->m41;
	mat.m12 = M2->m11 * M3->m12 + M2->m12 * M3->m22 + M2->m13 * M3->m32 + M2->m14 * M3->m42;
	mat.m13 = M2->m11 * M3->m13 + M2->m12 * M3->m23 + M2->m13 * M3->m33 + M2->m14 * M3->m43;
	mat.m14 = M2->m11 * M3->m14 + M2->m12 * M3->m24 + M2->m13 * M3->m34 + M2->m14 * M3->m44;
	
	mat.m21 = M2->m21 * M3->m11 + M2->m22 * M3->m21 + M2->m23 * M3->m31 + M2->m24 * M3->m41;
	mat.m22 = M2->m21 * M3->m12 + M2->m22 * M3->m22 + M2->m23 * M3->m32 + M2->m24 * M3->m42;
	mat.m23 = M2->m21 * M3->m13 + M2->m22 * M3->m23 + M2->m23 * M3->m33 + M2->m24 * M3->m43;
	mat.m24 = M2->m21 * M3->m14 + M2->m22 * M3->m24 + M2->m23 * M3->m34 + M2->m24 * M3->m44;

	mat.m31 = M2->m31 * M3->m11 + M2->m32 * M3->m21 + M2->m33 * M3->m31 + M2->m34 * M3->m41;
	mat.m32 = M2->m31 * M3->m12 + M2->m32 * M3->m22 + M2->m33 * M3->m32 + M2->m34 * M3->m42;
	mat.m33 = M2->m31 * M3->m13 + M2->m32 * M3->m23 + M2->m33 * M3->m33 + M2->m34 * M3->m43;
	mat.m34 = M2->m31 * M3->m14 + M2->m32 * M3->m24 + M2->m33 * M3->m34 + M2->m34 * M3->m44;

	mat.m41 = M2->m41 * M3->m11 + M2->m42 * M3->m21 + M2->m43 * M3->m31 + M2->m44 * M3->m41;
	mat.m42 = M2->m41 * M3->m12 + M2->m42 * M3->m22 + M2->m43 * M3->m32 + M2->m44 * M3->m42;
	mat.m43 = M2->m41 * M3->m13 + M2->m42 * M3->m23 + M2->m43 * M3->m33 + M2->m44 * M3->m43;
	mat.m44 = M2->m41 * M3->m14 + M2->m42 * M3->m24 + M2->m43 * M3->m34 + M2->m44 * M3->m44;

	memcpy(M1, &mat, sizeof(MATRIX));
}

void SetIdentity(MATRIX *mat)
{
	memset(mat, 0, sizeof(MATRIX));

	mat->m11 = 1;
	mat->m22 = 1;
	mat->m33 = 1;
	mat->m44 = 1;
}

void LoadIdentity(void)
{
	memset(pTM, 0, sizeof(MATRIX));

	pTM->m11 = 1;
	pTM->m22 = 1;
	pTM->m33 = 1;
	pTM->m44 = 1;
}

void Translate(float dx, float dy, float dz)
{
	MATRIX T;

	SetIdentity(&T);

	T.m41 = dx;
	T.m42 = dy;
	T.m43 = dz;

	MatrixMult(pTM, pTM, &T);
}

void RotateX(float theta)
{
	MATRIX R;
	
	float radian = PI / 180.0f * theta;
	float sine = (float)sin(radian);
	float cosine = (float)cos(radian);

	SetIdentity(&R);

	R.m22 = cosine;
	R.m23 = sine;
	R.m32 = -sine;
	R.m33 = cosine;

	MatrixMult(pTM, pTM, &R);
}

void RotateY(float theta)
{
	MATRIX R;

	float radian = PI / 180.0f * theta;
	float sine = (float)sin(radian);
	float cosine = (float)cos(radian);

	SetIdentity(&R);

	R.m11 = cosine;
	R.m13 = -sine;
	R.m31 = sine;
	R.m33 = cosine;

	MatrixMult(pTM, pTM, &R);
}

void RotateZ(float theta)
{
	MATRIX R;

	float radian = PI / 180.0f * theta;
	float sine = (float)sin(radian);
	float cosine = (float)cos(radian);

	SetIdentity(&R);

	R.m11 = cosine;
	R.m12 = sine;
	R.m21 = -sine;
	R.m22 = cosine;

	MatrixMult(pTM, pTM, &R);
}

void Scale(float sx, float sy, float sz)
{
	MATRIX S;

	SetIdentity(&S);

	S.m11 = sx;
	S.m22 = sy;
	S.m33 = sz;

	MatrixMult(pTM, pTM, &S);
}

// Simple Perspective projection
void Perspective(float d)
{
	MATRIX P;

	SetIdentity(&P);

	P.m34 = 1 / d;
	P.m44 = 0;

	MatrixMult(pTM, pTM, &P);
}

void PerspectiveFOV(float width, float height, float d, float nearz, float farz)
{
	MATRIX P;

    float    h, w, Q;
	
	w = 2 * d / width;
    h = 2 * d / height;
    Q = farz / (farz - nearz); 

    memset(&P, 0, sizeof(MATRIX));
	
	P.m11 = w;    
	P.m22 = h;
    P.m33 = Q;
    P.m43 = -Q * nearz;
    P.m34 = 1;

	MatrixMult(pTM, pTM, &P);
}

void oglPerspectiveFOV(float width, float height, float d, float nearz, float farz)
{
	MATRIX P;

    float    h, w, Q;
	
	w = 2 * d / width;
    h = 2 * d / height;
    Q = (farz + nearz) / (farz - nearz); 

    memset(&P, 0, sizeof(MATRIX));
	
	P.m11 = w;    
	P.m22 = h;
    P.m33 = Q;
    P.m43 = -2 * farz * nearz / (farz - nearz);
    P.m34 = 1;

	MatrixMult(pTM, pTM, &P);
}

void Transform(VERTEX *p1, VERTEX *p2, MATRIX *mat)
{
	p1->x = p2->x * mat->m11 + p2->y * mat->m21 + p2->z * mat->m31 + p2->w * mat->m41;
	p1->y = p2->x * mat->m12 + p2->y * mat->m22 + p2->z * mat->m32 + p2->w * mat->m42;
	p1->z = p2->x * mat->m13 + p2->y * mat->m23 + p2->z * mat->m33 + p2->w * mat->m43;
	p1->w = p2->x * mat->m14 + p2->y * mat->m24 + p2->z * mat->m34 + p2->w * mat->m44;
}

void TransformCTM(VERTEX *p1, VERTEX *p2)
{
	if (p2->tflag == FALSE) return;

	p1->x = p2->x * CTM.m11 + p2->y * CTM.m21 + p2->z * CTM.m31 + CTM.m41;
	p1->y = p2->x * CTM.m12 + p2->y * CTM.m22 + p2->z * CTM.m32 + CTM.m42;
	p1->z = p2->x * CTM.m13 + p2->y * CTM.m23 + p2->z * CTM.m33 + CTM.m43;
	p1->w = p2->x * CTM.m14 + p2->y * CTM.m24 + p2->z * CTM.m34 + CTM.m44;
}

void TransformWTM(VERTEX *p1, VERTEX *p2)
{
	p1->x = p2->x * WTM.m11 + p2->y * WTM.m21 + p2->z * WTM.m31 + p2->w * WTM.m41;
	p1->y = p2->x * WTM.m12 + p2->y * WTM.m22 + p2->z * WTM.m32 + p2->w * WTM.m42;
	p1->z = p2->x * WTM.m13 + p2->y * WTM.m23 + p2->z * WTM.m33 + p2->w * WTM.m43;
	p1->w = p2->x * WTM.m14 + p2->y * WTM.m24 + p2->z * WTM.m34 + p2->w * WTM.m44;
}

void TransformVTM(VERTEX *p1, VERTEX *p2)
{
	p1->x = p2->x * VTM.m11 + p2->y * VTM.m21 + p2->z * VTM.m31 + p2->w * VTM.m41;
	p1->y = p2->x * VTM.m12 + p2->y * VTM.m22 + p2->z * VTM.m32 + p2->w * VTM.m42;
	p1->z = p2->x * VTM.m13 + p2->y * VTM.m23 + p2->z * VTM.m33 + p2->w * VTM.m43;
	p1->w = p2->x * VTM.m14 + p2->y * VTM.m24 + p2->z * VTM.m34 + p2->w * VTM.m44;
}

void TransformPTM(VERTEX *p1, VERTEX *p2)
{
	p1->x = p2->x * PTM.m11 + p2->y * PTM.m21 + p2->z * PTM.m31 + p2->w * PTM.m41;
	p1->y = p2->x * PTM.m12 + p2->y * PTM.m22 + p2->z * PTM.m32 + p2->w * PTM.m42;
	p1->z = p2->x * PTM.m13 + p2->y * PTM.m23 + p2->z * PTM.m33 + p2->w * PTM.m43;
	p1->w = p2->x * PTM.m14 + p2->y * PTM.m24 + p2->z * PTM.m34 + p2->w * PTM.m44;
}
