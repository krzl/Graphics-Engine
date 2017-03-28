#pragma once

#include <d3dcompiler.h>

#include "Shaders\Shader.h"
#include "..\DXDirectives.h"


namespace Kz
{
	class ShaderManager;
	class GraphicsDevice;
	struct ShaderVariablesData;

	class ShaderDX : Shader
	{
		friend class GraphicsDeviceDX;
		friend class ShaderProgramDX;

	private:

		ShaderType				m_type;
		ID3D11Device*			m_device;
		ID3D11DeviceChild*		m_shader;
		ID3D11DeviceContext*	m_context;
		ID3D11InputLayout*		m_inputLayout;

		ShaderDX(GraphicsDevice& gd, ID3D11Device* device, ID3D11DeviceContext* context,
			ShaderType type, const std::string& shaderName);

		~ShaderDX();

		virtual void			BindBuffer(Buffer& buffer, int bindPoint);
		virtual void			BindTexture(Texture& texture, int bindPoint);

		void					Enable(ShaderManager& srManager, ShaderVariablesData& svData);
		void					CreateInputLayout(ID3D11ShaderReflection* reflection,
									D3D11_SHADER_DESC desc, ID3DBlob* blob);
	};
}