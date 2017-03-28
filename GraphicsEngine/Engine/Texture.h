#pragma once

#include <cstdint>

namespace Kz
{
	enum CPUAccess
	{
		ACCESS_NO_ACCESS,
		ACCESS_READ_ONLY,
		ACCESS_WRITE_ONLY,
		ACCESS_READ_WRITE
	};

	enum DataFormat
	{
		FORMAT_R,
		FORMAT_RG,
		FORMAT_RGBX,
		FORMAT_RGBA,
		FORMAT_RGB32F,
		FORMAT_RGBA32F,
		FORMAT_DEPTH24F_STENCIL8
	};

	enum Filtering
	{
		FILTERING_MAG_LINEAR = 0,
		FILTERING_MAG_NEAREST = 1,
		FILTERING_MIN_LINEAR = 0,
		FILTERING_MIN_NEAREST = 2,
		FILTERING_MIN_LINEAR_MIPMAP_NEAREST = 4,
		FILTERING_MIN_NEAREST_MIPMAP_NEAREST = 8,
		FILTERING_MIN_LINEAR_MIPMAP_LINEAR = 16,
		FILTERING_MIN_NEAREST_MIPMAP_LINEAR = 32
	};

	enum Usage
	{
		USAGE_DEFAULT,
		USAGE_IMMUTABLE,
		USAGE_DYNAMIC,
		USAGE_RENDER_TARGET,
		USAGE_DEPTH_STENCIL
	};

	enum Wrap
	{
		WRAP_REPEAT_S = 0,
		WRAP_REPEAT_T = 0,
		WRAP_REPEAT_R = 0,
		WRAP_REPEAT = 0,
		WRAP_CLAMP_S = 1,
		WRAP_CLAMP_T = 2,
		WRAP_CLAMP_R = 4,
		WRAP_CLAMP = 7
	};

	class Texture
	{
	protected:
		virtual void VTableFix(){}
	};

	class Texture1D : public Texture
	{
	public:

		virtual Vector4f GetTextureData(int x) = 0;
	};

	class Texture2D : public Texture
	{
	public:

		virtual Vector4f GetTextureData(int x, int y) = 0;
	};

	class TextureCubemap : public Texture
	{
	public:

		virtual Vector4f GetTextureData(int x, int y, int z) = 0;
	};
}