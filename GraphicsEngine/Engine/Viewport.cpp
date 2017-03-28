#include "stdafx.h"
#include "Viewport.h"
#include "RenderSystem.h"
#include "Window.h"
#include "GraphicsDevice.h"

namespace Kz
{
	Viewport::Viewport(Window& window, GraphicsDevice& gd, float xStart, float yStart, 
		float width, float height) :
		m_window(window),
		m_x(xStart), m_y(yStart),
		m_width(width),
		m_height(height)
	{
		m_gBuffer = gd.CreateGBuffer(GetWidth(), GetHeight());
	}

	float Viewport::GetAspectRatio() const
	{
		float width	= (float)GetWidth();
		float height = (float)GetHeight();
		return width / height;
	}

	void Viewport::Resize()
	{
		m_gBuffer->Resize(GetWidth(), GetHeight());
	}

	GBuffer& Viewport::GetGBuffer()
	{
		return *m_gBuffer;
	}

	int Viewport::GetWidth() const
	{
		return (int)(m_width * m_window.GetWidth());
	}

	int Viewport::GetHeight() const
	{
		return (int)(m_height * m_window.GetHeight());
	}

	int Viewport::GetX() const
	{
		return (int)(m_x * m_window.GetWidth());
	}

	int Viewport::GetY() const
	{
		return (int)(m_y * m_window.GetHeight());
	}

	void Viewport::SetX(float x)
	{
		m_x = x;
	}

	void Viewport::SetY(float y)
	{
		m_y = y;
	}

	void Viewport::SetWidth(float width)
	{
		m_width = width;
	}

	void Viewport::SetHeight(float height)
	{
		m_height = height;
	}

	Viewport::~Viewport()
	{
		delete m_gBuffer;
		m_window.RemoveViewport(*this);
	}
}