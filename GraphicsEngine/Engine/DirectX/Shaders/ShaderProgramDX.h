#pragma once

#include <vector>
#include <map>

#include "Shaders\ShaderProgram.h"
#include "../DXDirectives.h"

namespace Kz
{
	class Shader;
	class ShaderDX;
	class ShaderManager;

	class ShaderProgramDX : public ShaderProgram
	{
		friend class GraphicsDeviceDX;

	private:

		ID3D11Device*           m_device;
		ID3D11DeviceContext*    m_context;
		
		ShaderProgramDX(ID3D11Device* device, ID3D11DeviceContext* context, 
			std::vector<Shader*>& shaders, ShaderManager& srManager);

	public:

		virtual void Enable(ShaderManager& srManager, Material& material);
	};
}