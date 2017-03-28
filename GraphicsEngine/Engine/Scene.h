#pragma once

#include "SceneNode.h"
#include "Lights\DirectionalLight.h"

namespace Kz
{
	class Skybox;
	class AmbientLight;
	class RenderSystem;
	class ShaderManager;

	class Scene
	{
		friend class SceneManager;
		friend class RenderSystem;
		friend class SceneNode;

	private:

		SceneNode			m_rootNode;
		AmbientLight*		m_ambientLight;
		Skybox*				m_skybox;
		RenderSystem*		m_renderSystem;
		ShaderManager*		m_shaderManager;

		Scene(RenderSystem& renderSystem, ShaderManager& srManager);

	public:

		void				AttachDirectionalLight(DirectionalLight* dirLight);
		void				DetachDirectionalLight(DirectionalLight* dirLight);
		void				AttachSkybox(Skybox* skybox);
		void				DetachSkybox();
		SceneNode&			GetRootNode();
		Vector4f			GetAmbientLight() const;
		Vector4f&			GetAmbientLight();
		void				SetAmbientLight(const Vector4f& ambientLight);
		void				UpdateNodes();
	};
}