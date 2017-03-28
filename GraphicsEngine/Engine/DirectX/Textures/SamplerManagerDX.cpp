#include "stdafx.h"
#include "SamplerManagerDX.h"
#include "Texture.h"

namespace Kz
{
	std::vector<SamplerInfo> SamplerManager::m_samplersData = std::vector<SamplerInfo>();

	ID3D11SamplerState* SamplerManager::CreateSampler(ID3D11Device* device, 
		ID3D11DeviceContext* context, int filtering, int wrap)
	{
		D3D11_SAMPLER_DESC desc;

		switch (filtering)
		{
		case FILTERING_MIN_NEAREST | FILTERING_MAG_NEAREST:
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case FILTERING_MIN_NEAREST | FILTERING_MAG_LINEAR:
			desc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case FILTERING_MIN_LINEAR | FILTERING_MAG_NEAREST:
			desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case FILTERING_MIN_LINEAR | FILTERING_MAG_LINEAR:
			desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case FILTERING_MIN_NEAREST_MIPMAP_NEAREST | FILTERING_MAG_NEAREST:
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case FILTERING_MIN_NEAREST_MIPMAP_LINEAR | FILTERING_MAG_NEAREST:
			desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			break;
		case FILTERING_MIN_NEAREST_MIPMAP_NEAREST | FILTERING_MAG_LINEAR:
			desc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			break;
		case FILTERING_MIN_NEAREST_MIPMAP_LINEAR | FILTERING_MAG_LINEAR:
			desc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			break;
		case FILTERING_MIN_LINEAR_MIPMAP_NEAREST | FILTERING_MAG_NEAREST:
			desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			break;
		case FILTERING_MIN_LINEAR_MIPMAP_LINEAR | FILTERING_MAG_NEAREST:
			desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			break;
		case FILTERING_MIN_LINEAR_MIPMAP_NEAREST | FILTERING_MAG_LINEAR:
			desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		case FILTERING_MIN_LINEAR_MIPMAP_LINEAR | FILTERING_MAG_LINEAR:
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		}

		if ((wrap & WRAP_CLAMP_S) > 0)
		{
			desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		else{
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		}

		if ((wrap & WRAP_CLAMP_T) > 0)
		{
			desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		else{
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		}

		if ((wrap & WRAP_CLAMP_R) > 0)
		{
			desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		else{
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		}

		desc.MipLODBias		= 0.0f;
		desc.MaxAnisotropy	= 1;
		desc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;
		desc.MinLOD			= 0;
		desc.MaxLOD			= D3D11_FLOAT32_MAX;

		ID3D11SamplerState* sampler;
		SamplerInfo samplerInfo;

		device->CreateSamplerState(&desc, &sampler);

		samplerInfo.sampler = sampler;
		samplerInfo.filtering = filtering;
		samplerInfo.wrap = wrap;

		m_samplersData.push_back(samplerInfo);

		return sampler;
	}

	ID3D11SamplerState* SamplerManager::GetSampler(ID3D11Device* device, 
		ID3D11DeviceContext* context, int filtering, int wrap)
	{
		for (size_t i = 0; i < m_samplersData.size(); i++)
		{
			SamplerInfo info = m_samplersData[i];

			if (filtering == info.filtering && wrap == info.wrap)
			{
				return info.sampler;
			}
		}

		return CreateSampler(device, context, filtering, wrap);
	}
}