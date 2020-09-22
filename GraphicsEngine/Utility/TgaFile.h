#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace Kz
{
	class TgaFile
	{
	private:

		int							m_width;
		int							m_height;
		int							m_bitsPerPixel;

		std::vector<std::uint8_t>	m_imageData;

	public:

		TgaFile(const std::string& filepath);

		int							GetWidth();
		int							GetHeight();
		int							GetBitsPerPixel();
		std::vector<std::uint8_t>&	GetImageData();
	};
}