#pragma once

#include "Shaders/ShaderManager.h"

#include <queue>

namespace Kz
{
	struct Animation_t;
	struct Bone_t;
	struct Submesh_t;

	class SceneNode;
	class Scene;
	class Model;
	class EntityManager;

	class AnimationSystem
	{
		friend class Entity;
		friend class EntityManager;

	private:

		Model&						m_model;
		EntityManager&				m_entityManager;

		float						m_animationTime = 0;
		SceneNode**					m_sceneNodes = NULL;
		Animation_t*				m_currentAnimation = NULL;

		RenderPerSkinningInfo		m_skinningInfo;

		std::queue<Animation_t*>	m_animationQueue;

		void						AnimateBone(Bone_t& bone, Matrix4f currentMatrix);
		virtual void				SetNode(SceneNode* sceneNode);
		void						SetupSceneNodes(Bone_t& bone, SceneNode* parentNode, Scene& scene);
		void						Update(float timeElapsed);

		AnimationSystem(Model& model, Scene& scene, EntityManager& entityManager);

	public:

		~AnimationSystem();

		SceneNode*					GetSceneNodeFromBone(const std::string& boneName);
		void						LoadAnimation(const std::string& animationName);
		void						EnqueueAnimation(const std::string& animationName);
		void						RunAnimation(const std::string& animationName);
	};
}