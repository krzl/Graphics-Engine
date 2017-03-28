#pragma once

#include "Entity.h"

namespace Kz
{
	class Texture2D;
	class Material;

	class Primitive : public Entity
	{
		friend class EntityManager;

	protected:

		Primitive(Model& model, GraphicsDevice& gd, Material& material) :
			Entity(model, gd, material)
		{}

		Primitive(Model& model, GraphicsDevice& gd, ShaderProgram& shader) :
			Entity(model, gd, shader)
		{}
	};
}