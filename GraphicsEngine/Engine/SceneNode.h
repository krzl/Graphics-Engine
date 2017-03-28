#pragma once

#include <vector>

#include "Math\Quaternion.h"

namespace Kz
{
	class Entity;
	class Light;
	class Scene;

	class SceneNode
	{
		friend class Scene;
		friend class AnimationSystem;

	private:

		Scene&							m_scene;

		SceneNode*						m_parentNode;

		Vector3f						m_position;
		Quaternion						m_rotation;
		Vector3f						m_scale;

		Matrix4f						m_transformMatrix;
		bool							m_isUpdated = false;

		std::vector<SceneNode*>			m_nodes;

		SceneNode(Scene& scene) :
			m_scene(scene),
			m_parentNode(NULL),
			m_scale(Vector3f(1.0f, 1.0f, 1.0f)) 
		{}

		SceneNode(SceneNode* parent) :
			m_scene(parent->m_scene),
			m_parentNode(parent),
			m_scale(Vector3f(1.0f, 1.0f, 1.0f))
		{}

		SceneNode(SceneNode* parent, Vector3f& position, Quaternion& rotation, Vector3f& scale) :
			m_scene(parent->m_scene),
			m_parentNode(parent),
			m_position(position),
			m_rotation(rotation),
			m_scale(scale)
		{}

		void							SetLookAtMatrix();
		void							SetParentNode(SceneNode* parentNode);
		void							Update();

	public:

		~SceneNode();

		void							AttachEntity(Entity* entity);
		void							AttachLight(Light* light);
		void							DetachEntity(Entity* entity);
		void							DetachLight(Light* light);
		SceneNode*						CreateNode();
		SceneNode*						CreateNode(Vector3f& position, Quaternion& rotation, Vector3f& scale);
		SceneNode*						GetParentNode() const;
		const Vector3f&					GetGlobalPosition() const;
		const Vector3f&					GetPosition() const;
		const Quaternion&				GetRotation() const;
		const Vector3f&					GetScale() const;
		const Matrix4f&					GetTransformMatrix() const;
		void							SetPosition(const Vector3f& positionVector);
		void							SetRotation(const Quaternion& rotationQuaternion);
		void							SetScale(const Vector3f& scaleVector);
		void							Translate(const Vector3f& translateVector);
		void							Yaw(float yawRadians);
		void							Pitch(float pitchRadians);
		void							Roll(float rollRadians);
		void							Rotate(const Quaternion& rotationQuaternion);
		void							Scale(const Vector3f& scaleVector);
	};
}