#include "stdafx.h"
#include "Plane.h"

namespace Kz
{
	Plane::Plane(Model& model, float x, float z, GraphicsDevice& gd, Material& material) :
		Primitive(model, gd, material)
	{
		m_entityInfo.scaleVector = Vector3f(x, 1.0f, z);
	}

	Plane::Plane(Model& model, float x, float z, GraphicsDevice& gd, ShaderProgram& shader) :
		Primitive(model, gd, shader)
	{
		m_entityInfo.scaleVector = Vector3f(x, 1.0f, z);
	}
}