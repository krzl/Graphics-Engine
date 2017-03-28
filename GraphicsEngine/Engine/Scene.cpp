#include "stdafx.h"
#include "Scene.h"
#include "Lights/Light.h"
#include "Lights/AmbientLight.h"
#include "Skybox.h"
#include "Shaders\ShaderManager.h"

namespace Kz
{
	Scene::Scene(RenderSystem& renderSystem, ShaderManager& srManager) : 
		m_rootNode(*this), 
		m_renderSystem(&renderSystem)
	{
		m_skybox = NULL;
		m_ambientLight = NULL;
		m_shaderManager = &srManager;
	}

	void Scene::AttachDirectionalLight(DirectionalLight* light)
	{
		GetRootNode().AttachLight(light);
	}

	void Scene::AttachSkybox(Skybox* newSkybox)
	{
		m_skybox = newSkybox;
	}

	void Scene::DetachSkybox()
	{
		m_skybox = NULL;
	}

	SceneNode& Scene::GetRootNode()
	{
		return m_rootNode;
	}

	void Scene::SetAmbientLight(const Vector4f& newAmbientLight)
	{
		if (m_ambientLight == NULL)
			m_ambientLight = new AmbientLight(*m_shaderManager->GetPredefMaterial(LIGHT_PASS_PROGRAM));
		m_ambientLight->SetAmbient(newAmbientLight);
	}

	void Scene::UpdateNodes()
	{
		m_rootNode.Update();
	}
}