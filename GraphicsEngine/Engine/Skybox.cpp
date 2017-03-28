#include "stdafx.h"
#include "Skybox.h"
#include "EntityManager.h"
#include "RenderSystem.h"
#include "TextureCreator.h"
#include "Buffer.h"
#include "Viewport.h"
#include "Material.h"

namespace Kz
{
	Skybox::Skybox(EntityManager& modelManager, TextureCreator& textureCreator,
		const std::string& skyboxFilepath, ShaderProgram& shader)
	{
		m_cube		= modelManager.CreateCube(1.0f, shader);
		m_cubemap	= textureCreator.CreateTextureCubemap(skyboxFilepath);

		m_entityInfo.scaleVector = Vector3f(1.0f, 1.0f, 1.0f);
	}

	void Skybox::SetViewport(Viewport& viewport)
	{
		m_entityInfo.modelMatrix = 
			Matrix4f::TranslationMatrix(viewport.m_camera.GetPosition()) * 
			Matrix4f::ScaleMatrix(50.0f);
	}

	void Skybox::PreRender(RenderSystem& renderSystem)
	{
		m_entityInfo.pvmMatrix = renderSystem.GetPVMatrix() * m_entityInfo.modelMatrix;
		renderSystem.m_shaderManager.SetBuffer(BUFFER_PER_ENTITY, 
			(void*)&m_entityInfo, sizeof(RenderPerEntityInfo));
		Material& material = *renderSystem.m_shaderManager.GetPredefMaterial(SKYBOX_PROGRAM);
		material.SetTexture("skyboxTexture", static_cast<Texture*>(m_cubemap));
		material.Bind(renderSystem.m_shaderManager);
	}

	void Skybox::Render()
	{
		m_cube->Render();
	}

	Skybox::~Skybox()
	{
		delete m_cube;
	}
}