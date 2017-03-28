#include "stdafx.h"
#include "ShaderDX.h"
#include "Shaders\ShaderManager.h"
#include "..\Textures\Texture2DDX.h"
#include "..\BufferDX.h"
#include "Shaders\UserDefBuffer.h"
#include "Material.h"
#include "Texture.h"

namespace Kz
{
	ShaderDX::ShaderDX(GraphicsDevice& gd, ID3D11Device* device, ID3D11DeviceContext* context,
		ShaderType type, const std::string& shaderFilePath)
	{
		m_context	= context;
		m_device	= device;
		m_type		= type;

		ID3DBlob* blob;

		std::wstring stemp = std::wstring(shaderFilePath.begin(), shaderFilePath.end());
		LPCWSTR sw = stemp.c_str();

		D3DReadFileToBlob(sw, &blob);

		switch (type)
		{
		case VERT_SHADER:
			DXCALL_(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), 
				NULL, (ID3D11VertexShader**)&m_shader));
			break;
		case GEOM_SHADER:
			DXCALL_(device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), 
				NULL, (ID3D11GeometryShader**)&m_shader));
			break;
		case FRAG_SHADER:
			DXCALL_(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(),
				NULL, (ID3D11PixelShader**)&m_shader));
			break;
		}

		ID3D11ShaderReflection* reflection = NULL;
		DXCALL_(D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), 
			IID_ID3D11ShaderReflection, (void**)&reflection));

		D3D11_SHADER_DESC desc;
		reflection->GetDesc(&desc);

		if (type == VERT_SHADER)
			CreateInputLayout(reflection, desc, blob);

		for (UINT i = 0; i < desc.ConstantBuffers; i++)
		{
			D3D11_SHADER_BUFFER_DESC constBufferDesc;
			ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
			constantBuffer->GetDesc(&constBufferDesc);

			if (IsPredefinedBuffer(constBufferDesc.Name))
			{
				m_predefBuffers[constBufferDesc.Name] = 0;

			}
			else
			{
				UserBufferInfo* bufferInfo	= new UserBufferInfo();
				bufferInfo->bufferSize		= constBufferDesc.Size; 
				bufferInfo->bufferName		= constBufferDesc.Name;

				m_userBufferInfos.push_back(bufferInfo);

				for (UINT j = 0; j < constBufferDesc.Variables; j++)
				{
					ID3D11ShaderReflectionVariable* variable = constantBuffer->GetVariableByIndex(j);
					D3D11_SHADER_VARIABLE_DESC varDesc;
					variable->GetDesc(&varDesc);

					bufferInfo->variablesOffsets[varDesc.Name] = varDesc.StartOffset;
				}
			}
		}

		for (UINT i = 0; i < desc.BoundResources; i++)
		{
			D3D11_SHADER_INPUT_BIND_DESC inputDesc;
			reflection->GetResourceBindingDesc(i, &inputDesc);
			switch (inputDesc.Type)
			{
			case D3D_SIT_CBUFFER:

				if (IsPredefinedBuffer(inputDesc.Name))
				{
					m_predefBuffers[inputDesc.Name] = inputDesc.BindPoint;
				}
				else
				{
					for (UserBufferInfo* bufferInfo : m_userBufferInfos)
					{
						if (bufferInfo->bufferName == inputDesc.Name)
						{
							bufferInfo->bufferIndex = inputDesc.BindPoint;
						}
					}
				}

				break;

			case D3D_SIT_TEXTURE:
				m_textures[inputDesc.Name] = inputDesc.BindPoint;
				break;
			}
		}
	}

	void ShaderDX::Enable(ShaderManager& srManager, ShaderVariablesData& svData)
	{
		void(TextureDX::*bindTextureOperation)(int) = NULL;
		void(BufferDX ::*bindCBufferOperation)(int) = NULL;

		switch (m_type)
		{
		case VERT_SHADER:
			m_context->VSSetShader((ID3D11VertexShader*)m_shader, NULL, 0);
			bindTextureOperation = &TextureDX::BindTextureVertexShader;
			bindCBufferOperation = &BufferDX ::BindShaderResourceVertexShader;
			m_context->IASetInputLayout(m_inputLayout);
			break;
		case GEOM_SHADER:
			m_context->GSSetShader((ID3D11GeometryShader*)m_shader, NULL, 0);
			bindTextureOperation = &TextureDX::BindTextureGeometryShader;
			bindCBufferOperation = &BufferDX ::BindShaderResourceGeometryShader;
			break;
		case FRAG_SHADER:
			m_context->PSSetShader((ID3D11PixelShader*)m_shader, NULL, 0);
			bindTextureOperation = &TextureDX::BindTexturePixelShader;
			bindCBufferOperation = &BufferDX ::BindShaderResourcePixelShader;
			break;
		default:
			__debugbreak(); //TODO: change to assert
		}

		for (std::pair<const std::string, int>& pair : m_predefBuffers)
		{
			BufferName bufferName = (BufferName) pair.second;
			BufferDX& buffer = reinterpret_cast<BufferDX&>(srManager.GetBuffer(bufferName));
			(buffer.*bindCBufferOperation)(bufferName);
		}

		for (UserDefBuffer* userBuffer : svData.buffers)
		{
			userBuffer->UpdateBuffer();
			BufferDX& buffer = reinterpret_cast<BufferDX&>(userBuffer->GetBuffer());
			(buffer.*bindCBufferOperation)(userBuffer->GetBufferIndex());
		}

		for (std::pair<const std::string, int>& pair : m_textures)
		{
			Texture* texBasePointer = svData.textures[pair.first];
			TextureDX* texture = dynamic_cast<TextureDX*>(texBasePointer);
			if (texture)
				(texture->*bindTextureOperation)(pair.second);
		}
	}

	void ShaderDX::CreateInputLayout(ID3D11ShaderReflection* reflection, 
		D3D11_SHADER_DESC desc, ID3D10Blob* blob)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (unsigned int i = 0; i < desc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			reflection->GetInputParameterDesc(i, &paramDesc);

			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			inputLayoutDesc.push_back(elementDesc);
		}

		DXCALL_V(m_device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &m_inputLayout));
	}

	void ShaderDX::BindBuffer(Buffer& buffer, int bindPoint)
	{

	}

	void ShaderDX::BindTexture(Texture& texture, int bindPoint)
	{

	}

	ShaderDX::~ShaderDX(){
		DX_RELEASE(m_shader);
	}
}