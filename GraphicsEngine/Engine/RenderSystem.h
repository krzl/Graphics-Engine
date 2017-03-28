#pragma once

#include <vector>

#include "Shaders\ShaderManager.h"

namespace Kz
{
	class Scene;
	class AmbientLight;
	class Light;
	class Viewport;
	class Entity;
	class GBuffer;

	class RenderSystem
	{
		friend class SceneManager;
		friend class SpotLight;
		friend class PointLight;
		friend class DirectionalLight;
		friend class Window;

	private:

		GraphicsDevice&				m_gd;
		RenderPerFrameInfo			m_frameInfo;

		Scene*						m_scene;
		GBuffer*					m_currentGBuffer;

		std::vector<Entity*>		m_renderList;
		std::vector<Light*>			m_lightsList;

		void						SetDeltaTime(float timeElapsed);
		void						Render(Viewport& viewport);
		void						GeomPass();
		void						LightPass();
		void						ShadowPass();
		void						FinalPass(Viewport& viewport);
		void						RenderSkybox(Viewport& viewport);

	public:

		ShaderManager				m_shaderManager;

		RenderSystem(GraphicsDevice* gd);

		void						Init(int width, int height);
		void						SetScene(Scene* scene);
		Matrix4f&					GetPVMatrix();
		Viewport&					CreateViewport(int xStart, int yStart, int width, int height);
		bool						AttachViewport(Viewport& viewport);
		bool						RemoveViewport(Viewport& viewport);
		void						AttachEntity(Entity* entity);
		void						AttachLight(Light* light);
		void						DetachEntity(Entity* entity);
		void						DetachLight(Light* light);

		void						SetupPointStencilPass();
		void						SetupLightPass();
		void						SetupLightPassPoint();
		void						SetupShadowVolumeStencil();
		void						FinalizeLightPass();
		void						FinalizeLightPassPoint();
	};
}