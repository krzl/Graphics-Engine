#pragma once

#include <vector>

#include "../DXDirectives.h"

namespace Kz
{
	struct SamplerInfo
	{
		int wrap;
		int filtering;
		ID3D11SamplerState* sampler;
	};

	class SamplerManager
	{
	private:

		static std::vector<SamplerInfo> m_samplersData;

		static ID3D11SamplerState* CreateSampler(ID3D11Device* device,
			ID3D11DeviceContext* context, int filtering, int wrap);

	public:

		static ID3D11SamplerState* GetSampler(ID3D11Device* device,
			ID3D11DeviceContext* context, int filtering, int wrap);
	};
}