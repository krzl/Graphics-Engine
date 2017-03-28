#pragma once

#include "Light.h"

namespace Kz
{
	class SpotLight : public Light
	{
		friend class SceneManager;

	private:

		Entity*			m_lightCube;

		SpotLight(Material& material);

		virtual void	LightPass(RenderSystem& renderSystem);

	public:

		const Vector3f&	GetAttenuation() const;
		const Vector3f&	GetDirection() const;
		float			GetSpotCutoff() const;
		int				GetSpotExponent() const;
		void			SetAttenuation(Vector3f& attenuation);
		void			SetDirection(const Vector3f& direction);
		void			SetSpotCutoff(float);
		void			SetSpotExponent(int);
	};
}