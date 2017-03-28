#include "stdafx.h"
#include "PointLight.h"
#include "Entity.h"
#include "RenderSystem.h"
#include "SceneNode.h"
#include "EntityManager.h"
#include "Buffer.h"
#include "Material.h"

namespace Kz
{
	PointLight::PointLight(Material& material)
	{
		m_lightCube			= (Entity*)m_entityManager->CreateCube(1.0f, 1.0f, 1.0f, material);
		m_lightInfo.type	= POINT_LIGHT;
	}

	const Vector3f& PointLight::GetAttenuation() const
	{
		return m_lightInfo.attenuation;
	}

	void PointLight::SetAttenuation(Vector3f& attenuation)
	{
		m_lightInfo.attenuation = attenuation;
		SetScaleMatrix();
	}

	void PointLight::SetDiffuse(Vector4f& diffuseColor)
	{
		Light::SetDiffuse(diffuseColor);
		SetScaleMatrix();
	}

	void PointLight::SetSpecular(Vector4f& specularColor)
	{
		Light::SetSpecular(specularColor);
		SetScaleMatrix();
	}

	void PointLight::SetScaleMatrix()
	{
		float maxChannel = fmax(fmax(GetDiffuse().x + GetSpecular().x, 
			GetDiffuse().y + GetSpecular().y), GetDiffuse().z + GetSpecular().z);

		float lightDistance = (-GetAttenuation().y + 
			sqrtf(GetAttenuation().y * GetAttenuation().y -4.0f * GetAttenuation().z * 
			(GetAttenuation().x - 256.0f * maxChannel))) / (2.0f * GetAttenuation().z);
		
		m_scaleMatrix = Matrix4f::ScaleMatrix(Vector3f(lightDistance, lightDistance, lightDistance));
	}

	void PointLight::LightPass(RenderSystem& renderSystem)
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