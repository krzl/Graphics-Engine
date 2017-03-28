#include "stdafx.h"
#include "AmbientLight.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "SceneNode.h"
#include "EntityManager.h"
#include "Buffer.h"
#include "Material.h"

namespace Kz
{
	AmbientLight::AmbientLight(Material& material)
	{
		m_lightCube = (Entity*)m_entityManager->CreateCube(1.0f, 1.0f, 1.0f, material);
		m_lightInfo.ambient = 0.0f;
		m_lightInfo.type = AMBIENT_LIGHT;
	}

	const Vector4f& AmbientLight::GetAmbient() const
	{
		return m_lightInfo.ambient;
	}

	void AmbientLight::SetAmbient(const Vector4f& ambientColor)
	{
		m_lightInfo.ambient = ambientColor;
	}

	void AmbientLight::LightPass(RenderSystem& renderSystem)
	{
		renderSystem.m_shaderManager.SetBuffer(BUFFER_PER_LIGHT, 
			(void*)&m_lightInfo, sizeof(RenderPerLightInfo));
		renderSystem.m_shaderManager.GetPredefMaterial(LIGHT_PASS_PROGRAM)->Bind(renderSystem.m_shaderManager);
		renderSystem.SetupLightPass();
		m_lightCube->Render();
		renderSystem.FinalizeLightPass();
	}
}