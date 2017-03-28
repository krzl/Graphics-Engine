#pragma once

#include "Math\Matrix.h"
#include "Shaders\ShaderManager.h"
#include "Material.h"

namespace Kz
{
	class Model;
	class RenderSystem;
	class Material;
	class AnimationSystem;
	class Scene;

	class Entity
	{
		friend class AmbientLight;
		friend class DirectionalLight;
		friend class PointLight;
		friend class SpotLight;
		friend class SceneNode;
		friend class Skybox;
		friend class RenderSystem;
		friend class EntityManager;
		friend class Primitive;

	private:

		bool						m_castingShadow = true;

		static int					m_entityIDCounter;

		Material					m_material;
		AnimationSystem*			m_animationSystem = NULL;

		Entity(Model& model, GraphicsDevice& gd, Material& material);
		Entity(Model& model, GraphicsDevice& gd, ShaderProgram& shaderProgram);
		Entity(Model& model, GraphicsDevice& gd, Material& material, 
			EntityManager& entityManager, Scene& scene);
		Entity(Model& model, GraphicsDevice& gd, ShaderProgram& shaderProgram, 
			EntityManager& entityManager, Scene& scene);

		void						Render() const;

	protected:

		SceneNode*					m_sceneNode;

		Model&						m_model;

		RenderPerEntityInfo			m_entityInfo;

		virtual void				PreRender(RenderSystem& renderSystem);
		virtual void				PreShadow(RenderSystem& renderSystem);
		bool						IsCastingShadow() const;
		void						SetNode(SceneNode* sceneNode);
		const Matrix4f&				GetTransformMatrix() const;

	public:

		~Entity();

		void						SetCastingShadow(bool);
		AnimationSystem&			GetAnimationSystem();
		void						SetShaderVariable(const std::string& variable,void* data, int dataSize);
	};
}