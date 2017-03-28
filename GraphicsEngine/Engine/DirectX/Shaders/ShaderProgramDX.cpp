#include "stdafx.h"
#include "ShaderProgramDX.h"
#include "ShaderDX.h"
#include "Material.h"

namespace Kz
{
	ShaderProgramDX::ShaderProgramDX(ID3D11Device* device, ID3D11DeviceContext* context,
		std::vector<Shader*>& shaders, ShaderManager& srManager) :
		ShaderProgram(shaders)
	{
		m_device = device;
		m_context = context;
	}

	void ShaderProgramDX::Enable(ShaderManager& srManager, Material& material)
	{
		m_context->GSSetShader(NULL, NULL, 0);

		for (size_t i = 0; i < m_shaders.size(); i++)
		{
			ShaderVariablesData svData = material.GetShaderVariablesData(m_shaders[i]);
			static_cast<ShaderDX*>(m_shaders[i])->Enable(srManager, svData);
		}
	}
}