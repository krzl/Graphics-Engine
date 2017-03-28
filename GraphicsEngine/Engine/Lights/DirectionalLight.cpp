#include "stdafx.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "SceneNode.h"
#include "EntityManager.h"
#include "Buffer.h"
#include "Material.h"

namespace Kz
{
	DirectionalLight::DirectionalLight(Material& material)
	{
		m_lightCube = (Entity*)m_entityManager->CreateCube(1.0f, 1.0f, 1.0f, material);
		m_lightInfo.type = DIRECTIONAL_LIGHT;
	}

	const Vector3f& DirectionalLight::GetDirection() const
	{
		return m_lightInfo.direction;
	}

	void DirectionalLight::SetDirection(const Vector3f& direction)
	{
		m_lightInfo.direction = direction;
	}

	void DirectionalLight::LightPass(RenderSystem& renderSystem)
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