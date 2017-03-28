#include "stdafx.h"
#include "SceneNode.h"
#include "Entity.h"
#include "Scene.h"
#include "RenderSystem.h"

namespace Kz
{
	void SceneNode::AttachEntity(Entity* entity)
	{
		entity->SetNode(this);
		m_scene.m_renderSystem->AttachEntity(entity);
	}

	void SceneNode::AttachLight(Light* light)
	{
		light->SetNode(this);
		m_scene.m_renderSystem->AttachLight(light);
	}

	void SceneNode::DetachEntity(Entity* entity)
	{
		m_scene.m_renderSystem->DetachEntity(entity);
	}

	void SceneNode::DetachLight(Light* light)
	{
		m_scene.m_renderSystem->DetachLight(light);
	}

	SceneNode* SceneNode::CreateNode()
	{
		SceneNode* newNode = new SceneNode(this);
		m_nodes.push_back(newNode);
		return newNode;
	}

	SceneNode* SceneNode::CreateNode(Vector3f& pos, Quaternion& rotation, Vector3f& scale)
	{
		SceneNode* newNode = new SceneNode(this, pos, rotation, scale);
		m_nodes.push_back(newNode);
		return newNode;
	}

	void SceneNode::Update()
	{
		if (!m_isUpdated)
		{
			SetLookAtMatrix();
			for (SceneNode* node : m_nodes)
			{
				node->m_isUpdated = false;
			}
			m_isUpdated = true;
		}
		for (SceneNode* node : m_nodes)
		{
			node->Update();
		}
	}

	const Matrix4f& SceneNode::GetTransformMatrix() const
	{
		return m_transformMatrix;
	}

	SceneNode* SceneNode::GetParentNode() const
	{
		return m_parentNode;
	}

	void SceneNode::SetParentNode(SceneNode* parentNode)
	{
		this->m_parentNode = parentNode;
	}

	void SceneNode::SetPosition(const Vector3f& newPosition)
	{
		m_position = newPosition;
		m_isUpdated = false;
	}

	const Vector3f& SceneNode::GetGlobalPosition() const
	{
		return m_position;
	}

	const Vector3f& SceneNode::GetPosition() const
	{
		return m_position;
	}

	void SceneNode::SetRotation(const Quaternion& newRotation)
	{
		m_rotation = newRotation;
		m_isUpdated = false;
	}

	const Quaternion& SceneNode::GetRotation() const
	{
		return m_rotation;
	}

	void SceneNode::SetScale(const Vector3f& newScale)
	{
		m_scale = newScale;
		m_isUpdated = false;
	}

	const  Vector3f& SceneNode::GetScale() const
	{
		return m_scale;
	}

	void SceneNode::Translate(const Vector3f& translation)
	{
		m_position += translation;
		m_isUpdated = false;
	}

	void SceneNode::Yaw(float angleRadian)
	{
		m_rotation.Yaw(angleRadian);
		m_isUpdated = false;
	}

	void SceneNode::Pitch(float angleRadian)
	{
		m_rotation.Pitch(angleRadian);
		m_isUpdated = false;
	}

	void SceneNode::Roll(float angleRadian)
	{
		m_rotation.Roll(angleRadian);
		m_isUpdated = false;
	}

	void SceneNode::Rotate(const Quaternion& rotation)
	{
		m_rotation *= rotation;
		m_isUpdated = false;
	}

	void SceneNode::Scale(const Vector3f& scale)
	{
		m_scale *= scale;
		m_isUpdated = false;
	}

	void SceneNode::SetLookAtMatrix()
	{
		Matrix4f t = Matrix4f::TranslationMatrix(m_position);
		Matrix4f r = m_rotation.ToMatrix();
		Matrix4f s = Matrix4f::ScaleMatrix(m_scale);
		m_transformMatrix = t * r * s;
		if (m_parentNode != NULL)
		{
			m_transformMatrix = m_parentNode->m_transformMatrix * m_transformMatrix;
		}
	}

	SceneNode::~SceneNode()
	{
		for (size_t i = 0; i < m_nodes.size(); i++)
		{
			delete m_nodes[i];
		}
	}
}