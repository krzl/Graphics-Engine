#include "stdafx.h"
#include "Shader.h"
#include "UserDefBuffer.h"

#define PREDEF_BUFFERS_SIZE 4

namespace Kz
{
	const std::string predefinedShaderResourceBuffers[] =
	{
		"PerFrame",
		"PerLight",
		"PerEntity",
		"PerSkinning"
	};

	bool Shader::IsPredefinedBuffer(const std::string& name)
	{
		for (int i = 0; i < PREDEF_BUFFERS_SIZE; i++)
		{
			if (name == predefinedShaderResourceBuffers[i])
			{
				return true;
			}
		}

		return false;
	}
}