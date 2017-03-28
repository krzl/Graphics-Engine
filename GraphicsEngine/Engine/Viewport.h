#pragma once

#include "GBuffer.h"
#include "Camera.h"

namespace Kz
{
	class Window;
	class GraphicsDevice;

	class Viewport
	{
		friend class RenderSystem;
		friend class Window;

	private:

		float							m_x;
		float							m_y;
		float							m_width;
		float							m_height;
		GBuffer*						m_gBuffer;

		Window&							m_window;

		Viewport(Window& m_window, GraphicsDevice& gd, float xStart, float yStart, float width, float height);

	public:

		~Viewport();

		Camera							m_camera;

		void							Resize();
		float							GetAspectRatio() const;
		GBuffer&						GetGBuffer();
		int								GetX() const;
		int								GetY() const;
		int								GetWidth() const;
		int								GetHeight() const;
		void							SetX(float x);
		void							SetY(float y);
		void							SetWidth(float width);
		void							SetHeight(float height);
	};
}