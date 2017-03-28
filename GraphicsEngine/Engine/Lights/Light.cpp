#include "stdafx.h"
#include "Light.h"
#include "SceneNode.h"
#include "RenderSystem.h"

namespace Kz
{
	EntityManager* Light::m_entityManager = 0;

	void Light::SetEntityManager(EntityManager& modelManager)
	{
		Light::m_entityManager = &modelManager;
	}

	EntityManager* Light::GetEntityManager()
	{
		return m_entityManager;
	}

	void Light::SetNode(SceneNode* sceneNode)
	{
		m_sceneNode = sceneNode;
	}

	const Vector4f& Light::GetDiffuse() const
	{
		return m_lightInfo.diffuse;
	}

	const Vector4f& Light::GetSpecular() const
	{
		return m_lightInfo.specular;
	}

	void Light::SetDiffuse(Vector4f& diffuseColor)
	{
		m_lightInfo.diffuse = diffuseColor;
	}

	void Light::SetSpecular(Vector4f& specularColor)
	{
		m_lightInfo.specular = specularColor;
	}

	void Light::SetLightPosition(RenderSystem& renderSystem)
	{
		const Matrix4f& transformMatrix = m_sceneNode->GetTransformMatrix();
		Vector3f position(transformMatrix[12], transformMatrix[13], transformMatrix[14]);
		m_lightInfo.position = position;
	}

	Light::~Light()
	{
		m_sceneNode->DetachLight(this);
	}
}