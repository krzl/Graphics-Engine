#pragma once

#include "Scene.h"
#include "EntityManager.h"
#include "RenderSystem.h"
#include "TextureCreator.h"


namespace Kz
{
	class Viewport;

	class SceneManager
	{
	private:

		Scene*						m_scene;
		EntityManager*				m_entityManager;
		RenderSystem*				m_renderSystem;
		TextureCreator*				m_textureCreator;
		ShaderManager*				m_shaderManager;

		GraphicsDevice*				m_gd = NULL;

	public:

		SceneManager();
		~SceneManager();

		void						Init(int width, int height);
		DirectionalLight*			CreateDirectionalLight();
		PointLight*					CreatePointLight();
		SpotLight*					CreateSpotLight();
		Scene&						GetScene();
		EntityManager&				GetEntityManager();
		RenderSystem&				GetRenderSystem();
		GraphicsDevice&				GetGraphicsDevice();
		TextureCreator&				GetTextureCreator();
		void						Update(double timeElapsed);
	};
}