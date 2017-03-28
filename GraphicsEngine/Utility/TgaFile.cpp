#include "TgaFile.h"
#include "RGBA.h"

#include <fstream>

namespace Kz
{
	TgaFile::TgaFile(const std::string& filepath)
	{
		int size = 0;

		std::fstream file(filepath.c_str(), std::ios::in | std::ios::binary);

		if (!file.is_open())
		{
			__debugbreak(); //TODO: replace with logging
			return;
		}

		std::uint8_t header[18] = { 0 };
		static std::uint8_t deCompressedHeader[12] = 
		{ 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

		static std::uint8_t isCompressedHeader[12] = 
		{ 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

		file.read(reinterpret_cast<char*>(&header), sizeof(header));

		if (!std::memcmp(deCompressedHeader, &header, sizeof(deCompressedHeader)))
		{
			m_width         = header[13] * 256 + header[12];
			m_height        = header[15] * 256 + header[14];
			m_bitsPerPixel  = header[16];
			size            = ((m_width * 32 + 31) / 32) * 4 * m_height;

			if ((m_bitsPerPixel != 24) && (m_bitsPerPixel != 32))
			{
				file.close();
			}

			std::vector<std::uint8_t> tempData;

			m_imageData.resize(m_bitsPerPixel == 24 ? size / 3 * 4 : size);
			tempData.resize(size);

			file.read(reinterpret_cast<char*>(tempData.data()), size);

			int readIndex = 0;
			int writeIndex = 0;

			while (readIndex < size - 1)
			{
				m_imageData[writeIndex++] = tempData[readIndex + 2];
				m_imageData[writeIndex++] = tempData[readIndex + 1];
				m_imageData[writeIndex++] = tempData[readIndex + 0];

				if (m_bitsPerPixel == 32)
				{
					m_imageData[writeIndex++] = tempData[readIndex + 3];
					readIndex += 4;
				}
				else
				{
					m_imageData[writeIndex++] = 0;
					readIndex += 3;
				}
			}
		}
		else if (!std::memcmp(isCompressedHeader, &header, sizeof(isCompressedHeader)))
		{
			m_width         = header[13] * 256 + header[12];
			m_height        = header[15] * 256 + header[14];
			m_bitsPerPixel  = header[16];
			size            = ((m_width * m_bitsPerPixel + 31) / 32) * 4 * m_height;

			if ((m_bitsPerPixel != 24) && (m_bitsPerPixel != 32))
			{
				file.close();
			}

			RGBA pixel = { 0 };
			int currentByte = 0;
			std::size_t currentPixel = 0;
			std::uint8_t chunkHeader = { 0 };
			int bytesPerPixel = (m_bitsPerPixel / 8);
			m_imageData.resize(m_width * m_height * sizeof(RGBA));

			do
			{
				file.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader));

				if (chunkHeader < 128)
				{
					chunkHeader++;
					for (int i = 0; i < chunkHeader; i++, currentPixel++)
					{
						file.read(reinterpret_cast<char*>(&pixel), bytesPerPixel);

						m_imageData[currentByte++] = pixel.R;
						m_imageData[currentByte++] = pixel.G;
						m_imageData[currentByte++] = pixel.B;
						if (m_bitsPerPixel > 24)
							m_imageData[currentByte++] = pixel.A;
						else
							m_imageData[currentByte++] = 0;
					}
				}
				else
				{
					chunkHeader -= 127;
					file.read(reinterpret_cast<char*>(&pixel), bytesPerPixel);

					for (int i = 0; i < chunkHeader; i++, currentPixel++)
					{
						m_imageData[currentByte++] = pixel.R;
						m_imageData[currentByte++] = pixel.G;
						m_imageData[currentByte++] = pixel.B;
						if (m_bitsPerPixel > 24)
							m_imageData[currentByte++] = pixel.A;
						else
							m_imageData[currentByte++] = 0;
					}
				}
			} while (currentPixel < (unsigned int)(m_width * m_height));
		}

		file.close();
	}

	int TgaFile::GetWidth()
	{
		return m_width;
	}

	int TgaFile::GetHeight()
	{
		return m_height;
	}

	int TgaFile::GetBitsPerPixel()
	{
		return m_bitsPerPixel;
	}

	std::vector<std::uint8_t>& TgaFile::GetImageData()
	{
		return m_imageData;
	}
}