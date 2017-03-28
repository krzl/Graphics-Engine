#pragma once

#include "Light.h"

namespace Kz
{
	class PointLight : public Light
	{
		friend class SceneManager;

	private:

		Entity*				m_lightCube;
		Matrix4f			m_scaleMatrix;

		PointLight(Material& material);

		virtual void		LightPass(RenderSystem& renderSystem);
		void				SetScaleMatrix();

	public:

		const Vector3f&		GetAttenuation() const;
		void				SetAttenuation(Vector3f& attenuation);
		virtual void		SetDiffuse(Vector4f& diffuseColor);
		virtual void		SetSpecular(Vector4f& specularColor);
	};
}