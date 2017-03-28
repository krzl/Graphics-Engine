#include "stdafx.h"
#include "GraphicsDevice.h"
#include "DirectX\GraphicsDeviceDX.h"

namespace Kz
{
	GraphicsDevice* GraphicsDevice::CreateGraphicsDevice(SupportedAPI chosenAPI)
	{
		if (chosenAPI == SupportedAPI::DIRECTX)
		{
			return new GraphicsDeviceDX();
		}
		return NULL;
	}

	Texture1D* GraphicsDevice::CreateRandomTexture1D(int width)
	{
		float *data = new float[width * 3];

		srand((unsigned int)time(NULL));

		for (int i = 0; i < width; i++)
		{
			data[i * 3 + 0] = (float)(rand()) / (float)(RAND_MAX);
			data[i * 3 + 1] = (float)(rand()) / (float)(RAND_MAX);
			data[i * 3 + 2] = (float)(rand()) / (float)(RAND_MAX);
		}

		return CreateTexture(width, (void*)data, FORMAT_RGB32F);
	}

	Texture2D* GraphicsDevice::Create1x1Texture(float r, float g, float b)
	{
		float *data = new float[4];

		data[0] = r;
		data[1] = g;
		data[2] = b;

		return CreateTexture(1, 1, (void*)data, FORMAT_RGBX);
	}
}