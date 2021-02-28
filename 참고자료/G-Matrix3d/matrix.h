/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#ifndef MATRIX_H
#define MATRIX_H

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

extern MATRIX CTM, WTM, VTM, PTM;
void SetActiveTM(int mode);
int GetActiveTM(void);
void MatrixMult(MATRIX *M1, MATRIX *M2, MATRIX *M3);
void SetIdentity(MATRIX *mat);
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


#endif