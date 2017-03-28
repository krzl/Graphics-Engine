#include "stdafx.h"
#include "SpotLight.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "SceneNode.h"
#include "EntityManager.h"
#include "RenderSystem.h"
#include "Buffer.h"
#include "Material.h"

namespace Kz
{
	SpotLight::SpotLight(Material& material)
	{
		m_lightCube			= (Entity*)m_entityManager->CreateCube(1.0f, 1.0f, 1.0f, material);
		m_lightInfo.type	= SPOT_LIGHT;
	}

	const Vector3f& SpotLight::GetAttenuation() const
	{
		return m_lightInfo.attenuation;
	}

	const Vector3f& SpotLight::GetDirection() const
	{
		return m_lightInfo.direction;
	}

	float SpotLight::GetSpotCutoff() const
	{
		return m_lightInfo.cutoff;
	}

	int SpotLight::GetSpotExponent() const
	{
		return m_lightInfo.spotExponent;
	}

	void SpotLight::SetAttenuation(Vector3f& attenuation)
	{
		m_lightInfo.attenuation = attenuation;
	}

	void SpotLight::SetDirection(const Vector3f& direction)
	{
		m_lightInfo.direction = direction;
	}

	void SpotLight::SetSpotCutoff(float spotCutoff)
	{
		m_lightInfo.cutoff = spotCutoff <= 90.0f ? spotCutoff : 90.0f;
	}

	void SpotLight::SetSpotExponent(int spotExponent)
	{
		m_lightInfo.spotExponent = spotExponent;
	}

	void SpotLight::LightPass(RenderSystem& renderSystem)
	{
		SetLightPosition(renderSystem);
		renderSystem.m_shaderManager.SetBuffer(BUFFER_PER_LIGHT, 
			(void*)&m_lightInfo, sizeof(RenderPerLightInfo));
		renderSystem.SetupShadowVolumeStencil();
		renderSystem.ShadowPass();
		renderSystem.m_shaderManager.GetPredefMaterial(LIGHT_PASS_PROGRAM)->Bind(renderSystem.m_shaderManager);
		renderSystem.SetupLightPass();
		m_lightCube->Render();
		renderSystem.FinalizeLightPass();
	}
}