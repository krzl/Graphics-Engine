#include "stdafx.h"
#include "ShaderManager.h"
#include "ShaderProgram.h"
#include "GraphicsDevice.h"
#include "DirectX\Textures\Texture2DDX.h"
#include "Material.h"

namespace Kz
{
	void ShaderManager::Init(GraphicsDevice& gd)
	{
		m_gd = &gd;

		ShaderProgram::shaderManager = this;

		m_buffers[BUFFER_PER_FRAME]		= m_gd->CreateBuffer(BUFFER_TYPE_SHADER_RESOURCE, sizeof(RenderPerFrameInfo),  0, BUFFER_UPDATE_DYNAMIC);
		m_buffers[BUFFER_PER_ENTITY]	= m_gd->CreateBuffer(BUFFER_TYPE_SHADER_RESOURCE, sizeof(RenderPerEntityInfo), 0, BUFFER_UPDATE_DYNAMIC);
		m_buffers[BUFFER_PER_LIGHT]		= m_gd->CreateBuffer(BUFFER_TYPE_SHADER_RESOURCE, sizeof(RenderPerLightInfo),  0, BUFFER_UPDATE_DYNAMIC);
		m_buffers[BUFFER_PER_SKINNING]	= m_gd->CreateBuffer(BUFFER_TYPE_SHADER_RESOURCE, sizeof(RenderPerSkinningInfo), 0, BUFFER_UPDATE_DYNAMIC);

		m_predefShaders.resize(PREDEF_SHADER_COUNT);
		m_predefShaderPrograms.resize(PREDEF_SHADER_PROGRAM_COUNT);
		m_materials.resize(PREDEF_SHADER_PROGRAM_COUNT);

		m_predefShaders[ANIM_SHADOW_VOLUME_VERT]
			= CreateShader(VERT_SHADER, "../Engine/ShaderSources/DX/AnimShadowVolumeVert.cso");
		m_predefShaders[BASIC_VERT]
			= CreateShader(VERT_SHADER, "../Engine/ShaderSources/DX/BasicVert.cso");
		m_predefShaders[LIGHT_PASS_FRAG]
			= CreateShader(FRAG_SHADER, "../Engine/ShaderSources/DX/LightPassFrag.cso");
		m_predefShaders[NULL_VERT]
			= CreateShader(VERT_SHADER, "../Engine/ShaderSources/DX/NullVert.cso");
		m_predefShaders[NULL_FRAG]
			= CreateShader(FRAG_SHADER, "../Engine/ShaderSources/DX/NullFrag.cso");
		m_predefShaders[SHADOW_VOLUME_VERT]
			= CreateShader(VERT_SHADER, "../Engine/ShaderSources/DX/ShadowVolumeVert.cso");
		m_predefShaders[SHADOW_VOLUME_GEOM]
			= CreateShader(GEOM_SHADER, "../Engine/ShaderSources/DX/ShadowVolumeGeom.cso");
		m_predefShaders[SKYBOX_VERT]
			= CreateShader(VERT_SHADER, "../Engine/ShaderSources/DX/SkyboxVert.cso");
		m_predefShaders[SKYBOX_FRAG]
			= CreateShader(FRAG_SHADER, "../Engine/ShaderSources/DX/SkyboxFrag.cso");

		
		m_predefShaderPrograms[ANIM_SHADOW_VOLUME_PROGRAM] = CreateShaderProgram
			({ m_predefShaders[ANIM_SHADOW_VOLUME_VERT], m_predefShaders[SHADOW_VOLUME_GEOM],
			m_predefShaders[NULL_FRAG] });

		m_predefShaderPrograms[BASIC_PROGRAM]			= CreateShaderProgram
			({ m_predefShaders[BASIC_VERT], m_predefShaders[NULL_FRAG] });

		m_predefShaderPrograms[LIGHT_PASS_PROGRAM]		= CreateShaderProgram
			({ m_predefShaders[NULL_VERT], m_predefShaders[LIGHT_PASS_FRAG] });


		m_predefShaderPrograms[SHADOW_VOLUME_PROGRAM]	= CreateShaderProgram
			({ m_predefShaders[SHADOW_VOLUME_VERT], m_predefShaders[SHADOW_VOLUME_GEOM],
				m_predefShaders[NULL_FRAG] });

		m_predefShaderPrograms[SKYBOX_PROGRAM]			= CreateShaderProgram
			({ m_predefShaders[SKYBOX_VERT], m_predefShaders[SKYBOX_FRAG] });


		for (int i = 0; i < PREDEF_SHADER_PROGRAM_COUNT; i++)
		{
			m_materials[i] = new Material(gd, *m_predefShaderPrograms[i]);
		}
	}

	Buffer& ShaderManager::GetBuffer(const std::string& name)
	{
		return *m_buffers[GetBufferType(name)];
	}

	BufferName ShaderManager::GetBufferType(const std::string& name)
	{
		if (name == "PerFrame")
		{
			return BUFFER_PER_FRAME;
		}
		else if (name == "PerEntity")
		{
			return BUFFER_PER_ENTITY;
		}
		else if (name == "PerLight")
		{
			return BUFFER_PER_LIGHT;
		}
		else if (name == "PerSkinning")
		{
			return BUFFER_PER_SKINNING;
		}
		else
		{
			__debugbreak(); //TODO: assertion
			return BUFFER_PER_FRAME;
		}
	}

	Buffer& ShaderManager::GetBuffer(BufferName buffer)
	{
		return *m_buffers[buffer];
	}


	void ShaderManager::SetBuffer(BufferName buffer, void* data, int dataSize)
	{
		m_buffers[buffer]->BindBuffer();
		m_buffers[buffer]->SetData(data, dataSize);
	}

	Shader* ShaderManager::CreateShader(ShaderType type, std::string filename)
	{
		return m_gd->CreateShader(type, filename);
	}

	ShaderProgram* ShaderManager::CreateShaderProgram(std::initializer_list<Shader*> shaders)
	{
		return m_gd->CreateShaderProgram(shaders, *this);
	}

	Material* ShaderManager::GetPredefMaterial(PredefShaderProgram shaderProgram)
	{
		return m_materials[shaderProgram];
	}

	ShaderManager::~ShaderManager()
	{
		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			delete m_buffers[i];
		}
	}
}