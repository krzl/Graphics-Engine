#include "stdafx.h"
#include "Cube.h"

namespace Kz
{
	Cube::Cube(Model& model, float x, float y, float z, GraphicsDevice& gd, Material& material) :
		Primitive(model, gd, material)
	{
		m_entityInfo.scaleVector = Vector3f(x, y, z);
	}

	Cube::Cube(Model& model, float x, float y, float z, GraphicsDevice& gd, ShaderProgram& shader) :
		Primitive(model, gd, shader)
	{
		m_entityInfo.scaleVector = Vector3f(x, y, z);
	}
}