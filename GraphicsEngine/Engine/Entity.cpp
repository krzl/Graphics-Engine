#include "stdafx.h"
#include "Entity.h"
#include "Model.h"
#include "SceneNode.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Buffer.h"
#include "AnimationSystem.h"

namespace Kz
{
	int Entity::m_entityIDCounter = 0;

	Entity::Entity(Model& model, GraphicsDevice& gd, ShaderProgram& shaderProgram) :
		m_model(model),
		m_material(gd, shaderProgram, model)
	{
		m_entityInfo.scaleVector = Vector3f(1.0f, 1.0f, 1.0f);
		m_entityInfo.entityID = m_entityIDCounter++;
	}

	Entity::Entity(Model& model, GraphicsDevice& gd, Material& material) :
		m_model(model),
		m_material(material)
	{
		m_entityInfo.scaleVector = Vector3f(1.0f, 1.0f, 1.0f);
		m_entityInfo.entityID = m_entityIDCounter++;
	}

	Entity::Entity(Model& model, GraphicsDevice& gd, ShaderProgram& shaderProgram, 
		EntityManager& entityManager, Scene& scene) :
		m_model(model),
		m_material(gd, shaderProgram, model)
	{
		m_entityInfo.scaleVector = Vector3f(1.0f, 1.0f, 1.0f);
		m_entityInfo.entityID = m_entityIDCounter++;

		if (m_model.IsRigged())
			m_animationSystem = new AnimationSystem(model, scene, entityManager);
	}

	Entity::Entity(Model& model, GraphicsDevice& gd, Material& material, 
		EntityManager& entityManager, Scene& scene) :
		m_model(model),
		m_material(material)
	{
		m_entityInfo.scaleVector = Vector3f(1.0f, 1.0f, 1.0f);
		m_entityInfo.entityID = m_entityIDCounter++;

		if (m_model.IsRigged())
			m_animationSystem = new AnimationSystem(model, scene, entityManager);
	}


	const Matrix4f& Entity::GetTransformMatrix() const
	{
		return m_sceneNode->GetTransformMatrix();
	}

	void Entity::SetShaderVariable(const std::string& variable, void* data, int dataSize)
	{
		m_material.SetShaderVariable(variable, data, dataSize);
	}

	void Entity::PreRender(RenderSystem& renderSystem)
	{
		m_entityInfo.modelMatrix = GetTransformMatrix();
		m_entityInfo.pvmMatrix = renderSystem.GetPVMatrix() * m_entityInfo.modelMatrix;
		renderSystem.m_shaderManager.SetBuffer(BUFFER_PER_ENTITY, 
			(void*)&m_entityInfo, sizeof(RenderPerEntityInfo));

		if (m_animationSystem)
			renderSystem.m_shaderManager.SetBuffer(BUFFER_PER_SKINNING,
			(void*)&m_animationSystem->m_skinningInfo, sizeof(RenderPerSkinningInfo));

		m_material.Bind(renderSystem.m_shaderManager);
	}

	void Entity::PreShadow(RenderSystem& renderSystem)
	{
		m_entityInfo.modelMatrix = GetTransformMatrix();
		m_entityInfo.pvmMatrix = renderSystem.GetPVMatrix() * m_entityInfo.modelMatrix;
		renderSystem.m_shaderManager.SetBuffer(BUFFER_PER_ENTITY, 
			(void*)&m_entityInfo, sizeof(RenderPerEntityInfo));

		if (m_animationSystem)
		{
			renderSystem.m_shaderManager.SetBuffer(BUFFER_PER_SKINNING,
				(void*)&m_animationSystem->m_skinningInfo, sizeof(RenderPerSkinningInfo));
			renderSystem.m_shaderManager.GetPredefMaterial(ANIM_SHADOW_VOLUME_PROGRAM)->
				Bind(renderSystem.m_shaderManager);
		}
		else
		{
			renderSystem.m_shaderManager.GetPredefMaterial(SHADOW_VOLUME_PROGRAM)->
				Bind(renderSystem.m_shaderManager);
		}
	}

	bool Entity::IsCastingShadow() const
	{
		return m_castingShadow;
	}

	void Entity::SetCastingShadow(bool newCastingShadow)
	{
		m_castingShadow = newCastingShadow;
	}

	AnimationSystem& Entity::GetAnimationSystem()
	{
		return *m_animationSystem;
	}

	void Entity::SetNode(SceneNode* sceneNode)
	{
		m_sceneNode = sceneNode;
		if (m_animationSystem)
			m_animationSystem->SetNode(sceneNode);
	}

	void Entity::Render() const
	{
		m_model.m_mesh->Render();
	}

	Entity::~Entity()
	{
		m_sceneNode->DetachEntity(this);
	}
}