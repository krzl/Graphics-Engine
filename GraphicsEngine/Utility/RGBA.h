#pragma once

#include <cstdint>

union RGBA
{
	std::uint32_t           color;

	struct
	{
		std::uint8_t        B;
		std::uint8_t        G;
		std::uint8_t        R;
		std::uint8_t        A;
	};
};