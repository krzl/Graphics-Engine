#pragma once

#include "Light.h"

namespace Kz
{
	class Entity;

	class DirectionalLight : public Light
	{
		friend class SceneManager;

	private:

		Entity*             m_lightCube;

		DirectionalLight(Material& material);

		virtual void        LightPass(RenderSystem& renderSystem);

	public:

		const Vector3f&     GetDirection() const;
		void                SetDirection(const Vector3f& direction);
	};
}