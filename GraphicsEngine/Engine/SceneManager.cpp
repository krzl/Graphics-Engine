#include "stdafx.h"
#include "SceneManager.h"
#include "Viewport.h"
#include "Lights\AmbientLight.h"
#include "Lights\DirectionalLight.h"
#include "Lights\PointLight.h"
#include "Lights\SpotLight.h"
#include "GraphicsDevice.h"
#include "Utility\Logger.h"

namespace Kz
{
	SceneManager::SceneManager()
	{
		Logger::CreateLogFile("\\Logs\\log.txt", LOG_TYPE_ALL);
		Logger::VSOutputEditLogTypes(LOG_TYPE_ALL);

		m_gd = GraphicsDevice::CreateGraphicsDevice(DIRECTX);
		m_textureCreator	= new TextureCreator(*m_gd);
		m_renderSystem		= new RenderSystem(m_gd);
		m_shaderManager		= &m_renderSystem->m_shaderManager;

		m_scene = new Scene(*m_renderSystem, *m_shaderManager);
		m_renderSystem->SetScene(m_scene);

		m_entityManager = new EntityManager(*m_scene, *m_gd, *m_textureCreator, m_renderSystem->m_shaderManager);
	}

	void SceneManager::Init(int width, int height)
	{
		Light::SetEntityManager(*m_entityManager);
		m_renderSystem->Init(width, height);
	}

	DirectionalLight* SceneManager::CreateDirectionalLight()
	{
		return new DirectionalLight(*m_shaderManager->GetPredefMaterial(LIGHT_PASS_PROGRAM));
	}

	PointLight* SceneManager::CreatePointLight()
	{
		return new PointLight(*m_shaderManager->GetPredefMaterial(LIGHT_PASS_PROGRAM));
	}

	SpotLight* SceneManager::CreateSpotLight()
	{
		return new SpotLight(*m_shaderManager->GetPredefMaterial(LIGHT_PASS_PROGRAM));
	}

	Scene& SceneManager::GetScene()
	{
		return *m_scene;
	}

	EntityManager& SceneManager::GetEntityManager()
	{
		return *m_entityManager;
	}

	RenderSystem& SceneManager::GetRenderSystem()
	{
		return *m_renderSystem;
	}

	TextureCreator& SceneManager::GetTextureCreator()
	{
		return *m_textureCreator;
	}

	void SceneManager::Update(double timeElapsed)
	{
		m_gd->SetupNextFrame();
		m_renderSystem->SetDeltaTime((float)timeElapsed);
		m_entityManager->Update((float)timeElapsed);
		m_scene->UpdateNodes();
	}

	GraphicsDevice& SceneManager::GetGraphicsDevice()
	{
		return *m_gd;
	}

	SceneManager::~SceneManager()
	{
		delete m_textureCreator;
		delete m_entityManager;
		delete m_renderSystem;
		delete m_gd;
	}
}