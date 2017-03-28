#pragma once

#include "Primitive.h"

namespace Kz
{
	class Texture2D;

	class Plane : public Primitive
	{
		friend class EntityManager;

	private:

		Plane(Model& planeModel, float x, float z, GraphicsDevice& gd, Material& material);
		Plane(Model& planeModel, float x, float z, GraphicsDevice& gd, ShaderProgram& shader);
	};
}