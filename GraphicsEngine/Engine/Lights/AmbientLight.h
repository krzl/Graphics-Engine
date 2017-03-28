#pragma once

#include "Light.h"

namespace Kz
{
	class Entity;

	class AmbientLight : public Light
	{
		friend class RenderSystem;
		friend class Scene;

	private:

		Entity*				m_lightCube;

		AmbientLight(Material& material);

		virtual void		LightPass(RenderSystem& renderSystem);

	public:

		void				Init();
		const Vector4f&		GetAmbient() const;
		void				SetAmbient(const Vector4f& ambientColor);
	};
}