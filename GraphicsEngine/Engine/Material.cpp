#include "stdafx.h"
#include "Material.h"
#include "Shaders\ShaderProgram.h"
#include "Shaders\Shader.h"
#include "Shaders\UserDefBuffer.h"
#include "Shaders\ShaderManager.h"
#include "Model.h"

namespace Kz
{
	Material::Material(Material& material) :
		m_program(material.GetShaderProgram())
	{
		m_shadersVariables = material.m_shadersVariables;
	}

	Material::Material(GraphicsDevice& gd, ShaderProgram& program) :
		m_program(program)
	{
		for (size_t i = 0; i < program.m_shaders.size(); i++)
		{
			ShaderVariablesData* svData = new ShaderVariablesData();
			svData->shader = program.m_shaders[i];

			m_shadersVariables.push_back(svData);

			for (UserBufferInfo* bufferInfo : svData->shader->m_userBufferInfos)
			{
				UserDefBuffer* buffer = new UserDefBuffer(gd, *bufferInfo);
				m_shadersVariables[i]->buffers.push_back(buffer);
			}
		}
	}

	Material::Material(GraphicsDevice& gd, ShaderProgram& program, Model& model) :
		Material(gd, program)
	{
		std::map<std::string, MaterialVariable>::iterator it = model.m_materialVariables.begin();
		while (it != model.m_materialVariables.end())
		{
			if (it->second.type == VARTYPE_TEXTURE)
			{
				SetTexture(it->first, static_cast<Texture*>(it->second.data));
			}
			else
			{
				SetShaderVariable(it->first, it->second.data, it->second.dataSize);
			}

			it++;
		}
	}

	ShaderProgram& Material::GetShaderProgram()
	{
		return m_program;
	}

	void Material::Bind(ShaderManager& srManager)
	{
		m_program.Enable(srManager, *this);
	}

	ShaderVariablesData& Material::GetShaderVariablesData(Shader* shader)
	{
		for (size_t i = 0; i < m_shadersVariables.size(); i++)
		{
			if (m_shadersVariables[i]->shader == shader)
			{
				return *m_shadersVariables[i];
			}
		}

		__debugbreak(); //TODO: assertion

		return *m_shadersVariables[0];
	}

	void Material::SetTexture(const std::string& textureName, Texture* texture)
	{
		for (size_t i = 0; i < m_program.m_shaders.size(); i++)
		{
			std::map<std::string, int>& currentMap = m_program.m_shaders[i]->m_textures;

			std::map<std::string, int>::iterator it = currentMap.find(textureName);

			if (it != currentMap.end())
			{
				m_shadersVariables[i]->textures[textureName] = texture;
			}
		}
	}

	void Material::SetShaderVariable(const std::string& variable, void* data, int dataSize)
	{
		for (size_t i = 0; i < m_program.m_shaders.size(); i++)
		{
			for (UserDefBuffer* userBuffer : m_shadersVariables[i]->buffers)
			{
				userBuffer->SetShaderVariable(variable, data, dataSize);
			}
		}
	}

	Material::~Material()
	{
		std::vector<ShaderVariablesData*>::iterator it = m_shadersVariables.begin();
		while (it != m_shadersVariables.end())
		{
			delete *it;
			it++;
		}
	}
}