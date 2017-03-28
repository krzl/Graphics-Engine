#include "stdafx.h"
#include "AnimationSystem.h"
#include "Model.h"
#include "Scene.h"
#include "EntityManager.h"

namespace Kz
{
	AnimationSystem::AnimationSystem(Model& model, Scene& scene, EntityManager& entityManager) :
		m_entityManager(entityManager),
		m_model(model)
	{
		m_sceneNodes = new SceneNode*[model.m_bones.size()];
		for (size_t i = 0; i < model.m_rootBones.size(); i++)
		{
			SetupSceneNodes(*model.m_rootBones[i], NULL, scene);
		}
	}

	void AnimationSystem::AnimateBone(Bone_t& bone, Matrix4f currentMatrix)
	{
		Matrix4f finalTransformMatrix;
		Matrix4f transformMatrix;

		if (m_currentAnimation)
		{
			Matrix4f keyFrameTranslation;
			Matrix4f keyFrameRotation;

			BoneKeyFrames_t* boneTrack = m_currentAnimation->m_boneTracks[bone.m_id];
			if (boneTrack)
			{
				size_t j = 1;
				while (boneTrack->m_keyFrames.size() > j && boneTrack->m_keyFrames[j]->m_time < m_animationTime)
				{
					j++;
				}

				if (boneTrack->m_keyFrames.size() == j)
				{
					keyFrameTranslation = boneTrack->m_keyFrames[j - 1]->m_translationMatrix;
					keyFrameRotation = boneTrack->m_keyFrames[j - 1]->m_rotationQuaternion.ToMatrix();
				}
				else if (boneTrack->m_keyFrames.size() != 0)
				{
					float lerpValue = (m_animationTime - boneTrack->m_keyFrames[j - 1]->m_time) / (boneTrack->m_keyFrames[j]->m_time - boneTrack->m_keyFrames[j - 1]->m_time);
					keyFrameTranslation = boneTrack->m_keyFrames[j - 1]->m_translationMatrix.Lerp(boneTrack->m_keyFrames[j]->m_translationMatrix, lerpValue);
					Quaternion keyFrameRot1 = boneTrack->m_keyFrames[j - 1]->m_rotationQuaternion;
					Quaternion keyFrameRot2 = boneTrack->m_keyFrames[j]->m_rotationQuaternion;
					Quaternion keyFrameRot3 = keyFrameRot1.Slerp(keyFrameRot2, lerpValue);
					keyFrameRotation = keyFrameRot3.ToMatrix();
				}

				transformMatrix = boneTrack->m_translationMatrix;
				transformMatrix = transformMatrix * keyFrameTranslation;
				transformMatrix = transformMatrix * boneTrack->m_rotationMatrix;
				transformMatrix = transformMatrix * keyFrameRotation;
			}

			finalTransformMatrix = currentMatrix * transformMatrix * bone.m_transformMatrix;
		}
		else
		{
			transformMatrix = bone.m_staticMatrix;
		}

		currentMatrix = currentMatrix * transformMatrix;

		m_sceneNodes[bone.m_id]->SetPosition(Vector3f(transformMatrix[12], transformMatrix[13], transformMatrix[14]));
		m_sceneNodes[bone.m_id]->SetRotation(transformMatrix);

		for (size_t i = 0; i < bone.m_childBones.size(); i++)
		{
			AnimateBone(*bone.m_childBones[i], currentMatrix);
		}

		m_skinningInfo.bonesData[bone.m_id] = finalTransformMatrix;
	}

	void AnimationSystem::SetNode(SceneNode* sceneNode)
	{
		for (Bone_t* rootBone : m_model.m_rootBones)
		{
			m_sceneNodes[rootBone->m_id]->SetParentNode(sceneNode);
			sceneNode->m_nodes.push_back(m_sceneNodes[rootBone->m_id]);
		}
	}

	void AnimationSystem::SetupSceneNodes(Bone_t& bone, SceneNode* parentNode, Scene& scene)
	{
		m_sceneNodes[bone.m_id] = new SceneNode(scene);

		if (parentNode)
		{
			m_sceneNodes[bone.m_id]->SetParentNode(parentNode);
			parentNode->m_nodes.push_back(m_sceneNodes[bone.m_id]);
		}
		for (Bone_t* childBone : bone.m_childBones)
		{
			SetupSceneNodes(*childBone, m_sceneNodes[bone.m_id], scene);
		}
	}

	void AnimationSystem::Update(float timeElapsed)
	{
		if (!m_currentAnimation && m_animationQueue.size() > 0)
		{
			m_currentAnimation = m_animationQueue.front();
			m_animationQueue.pop();
		}

		m_animationTime += timeElapsed;

		if (m_currentAnimation)
		{
			while (m_currentAnimation && m_animationTime > m_currentAnimation->m_length)
			{
				m_animationTime = m_animationTime - m_currentAnimation->m_length;
				m_currentAnimation = NULL;

				if (m_animationQueue.size() > 0)
				{
					m_currentAnimation = m_animationQueue.front();
					m_animationQueue.pop();
				}
				else
				{
					EnqueueAnimation("Idle");
					m_currentAnimation = m_animationQueue.front();
					m_animationQueue.pop();
				}
			}
		}

		for (size_t i = 0; i < m_model.m_rootBones.size(); i++){
			AnimateBone(*m_model.m_rootBones[i], Matrix4f());
		}
	}

	SceneNode* AnimationSystem::GetSceneNodeFromBone(const std::string& boneName)
	{
		std::map<std::string, Bone_t>::iterator it = m_model.m_bones.find(boneName);
		if (it == m_model.m_bones.end())
		{
			return NULL;
		}
		return m_sceneNodes[it->second.m_id];
	}

	void AnimationSystem::LoadAnimation(const std::string& animationName)
	{
		m_model.LoadAnimation(animationName);
	}

	void AnimationSystem::EnqueueAnimation(const std::string& animationName)
	{
		std::map<std::string, Animation_t*>::iterator it = m_model.m_animations.find(animationName);
		if (it != m_model.m_animations.end())
		{
			m_animationQueue.push(it->second);
		}
		else
		{
			if (m_model.LoadAnimation(animationName))
			{
				EnqueueAnimation(animationName);
			}
		}
	}

	void AnimationSystem::RunAnimation(const std::string& animationName)
	{
		std::map<std::string, Animation_t*>::iterator it = m_model.m_animations.find(animationName);
		if (it != m_model.m_animations.end())
		{
			m_animationQueue = std::queue<Animation_t*>();
			m_animationQueue.push(it->second);
			m_currentAnimation = NULL;
		}
		else
		{
			if (m_model.LoadAnimation(animationName))
			{
				RunAnimation(animationName);
			}
		}
	}

	AnimationSystem::~AnimationSystem()
	{
		for (size_t i = 0; i < m_model.m_bones.size(); i++)
		{
			delete m_sceneNodes[i];
		}

		delete[] m_sceneNodes;
		m_entityManager.RemoveAnimatedEntity(this);
	}
}