#pragma once

#include "Color.h"
#include "Geometry.h"

class FlatShader
{
public:
	FlatShader() {}
	~FlatShader() {}

	void SetModel(const Matrix4x4& m)      { m_Model = m; }
	void SetView(const Matrix4x4& m)       { m_View = m; }
	void SetProjection(const Matrix4x4& m) { m_Projection = m; }
	void SetViewport(const Matrix4x4& m)   { m_Viewport = m; }

	void SetLightDir(const Vector3f& lightDir)     { m_LightDir = lightDir; }
	void SetLightColor(const RGBColor& lightColor) { m_LightColor = lightColor; }

	Vector3f VertexShader(const Vector3f& v);
	Vector3f FragmentShader(const Vector3f& n, const RGBColor& objectColor);

private:
	Matrix4x4 m_Model;
	Matrix4x4 m_View;
	Matrix4x4 m_Projection;
	Matrix4x4 m_Viewport;

	RGBColor m_LightColor;
	Vector3f m_LightDir;
};
