#include "stdafx.h"
#include "Window.h"
#include "Viewport.h"
#include "RenderSystem.h"
#include "GraphicsDevice.h"

namespace Kz
{
	Window::Window(int width, int height, GraphicsDevice& gd) :
		m_gd(gd)
	{
		m_width		= width;
		m_height	= height;

		m_shouldClose	= false;
		m_isVisible		= true;

		m_keys.resize(MAX_KEY);

		for (int i = 0; i < MAX_KEY; i++)
			m_keys[i] = STATE_UP;

		m_mouseButtons.resize(MOUSE_BUTTONS_COUNT);
	}

	Viewport& Window::CreateViewport()
	{
		Viewport* viewport = new Viewport(*this, m_gd, 0.0f, 0.0f, 1.0f, 1.0f);
		m_viewports.push_back(viewport);

		return *viewport;
	}

	Viewport& Window::CreateViewport(float x, float y, float width, float height)
	{
		Viewport* viewport = new Viewport(*this, m_gd, x, y, width, height);
		m_viewports.push_back(viewport);

		return *viewport;
	}

	void Window::RemoveViewport(Viewport& viewport)
	{
		std::vector<Viewport*>::iterator it = 
			std::find(m_viewports.begin(), m_viewports.end(), &viewport);

		if (it != m_viewports.end())
		{
			m_viewports.erase(it);
		}
	}

	bool Window::IsClosed() const
	{
		return m_shouldClose;
	}

	bool Window::IsVisible() const
	{
		return m_isVisible;
	}

	float Window::GetAspectRatio() const
	{
		return (float)m_width / (float)m_height;
	}

	int Window::GetHeight() const
	{
		return m_height;
	}

	int Window::GetWidth() const
	{
		return m_width;
	}

	void Window::OnResize(int width, int height)
	{
		m_width		= width;
		m_height	= height;

	}

	void Window::FinalizeResize() {
		for (Viewport* viewport : m_viewports)
		{
			viewport->Resize();
		}

		m_gd.OnResize( m_width, m_height );
	}

	void Window::Update(RenderSystem& renderSystem)
	{
		for (Viewport* viewport : m_viewports)
		{
			renderSystem.Render(*viewport);
		}
	}

	KeyState Window::GetKeyState(int key) const
	{
		return m_keys[key];
	}

	KeyState Window::GetButtonState(MouseButton button) const
	{
		return m_mouseButtons[button];
	}

	void Window::GetMousePosition(int& xPos, int& yPos) const
	{
		xPos = m_mousePosX;
		yPos = m_mousePosY;
	}

	void Window::GetMousePositionDiff(int& dx, int& dy) const
	{
		dx = m_mousePosX - m_prevMousePosX;
		dy = m_mousePosY - m_prevMousePosY;
	}

	bool Window::IsMouseLocked() const
	{
		return m_isCursorLocked;
	}

}
