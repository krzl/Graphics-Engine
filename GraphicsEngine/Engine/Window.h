#pragma once

#include <string>
#include <vector>

#define MAX_KEY 256

namespace Kz
{
	class Viewport;
	class RenderSystem;
	class GraphicsDevice;

	enum KeyState
	{
		STATE_UP,
		STATE_PRESSED,
		STATE_DOWN,
		STATE_RELEASED
	};

	enum MouseButton
	{
		MOUSE_LB,
		MOUSE_RB,
		MOUSE_MB,
		MOUSE_BUTTONS_COUNT
	};

	class Window
	{
		friend class RenderSystem;

	protected:

		int								m_width;
		int								m_height;
		bool							m_isVisible;
		bool							m_shouldClose;
		GraphicsDevice&					m_gd;

		bool							m_isCursorLocked = true;
		int								m_prevMousePosX;
		int								m_prevMousePosY;
		int								m_mousePosX;
		int								m_mousePosY;

		std::vector<Viewport*>			m_viewports;

		std::vector<KeyState>			m_keys;
		std::vector<KeyState>			m_mouseButtons;

	public:

		Window(int width, int height, GraphicsDevice& gd);

		Viewport&						CreateViewport(float x, float y, float width, float height);
		Viewport&						CreateViewport();
		void							RemoveViewport(Viewport& viewport);
		void							OnResize(int windowWidth, int windowHeight);
		bool							IsClosed() const;
		bool							IsVisible() const;
		float							GetAspectRatio() const;
		int								GetHeight() const;
		int								GetWidth() const;

		virtual void					Close() = 0;
		KeyState						GetKeyState(int key) const;
		KeyState						GetButtonState(MouseButton button) const;
		bool							IsMouseLocked() const;
		virtual void					GetMousePosition(int& xPos, int& yPos) const;
		virtual void					GetMousePositionDiff(int& dx, int& dy) const;
		virtual void					MakeCurrent() const = 0;
		virtual void					SwapBuffers() = 0;
		virtual void					LockMouse() = 0;
		virtual void					UnlockMouse() = 0;
		virtual void					Update(RenderSystem& renderSystem);
	};
}

#define KEY_ESCAPE        0x1B
#define KEY_SPACE         0x20

#define KEY_0			  0x30
#define KEY_1			  0x31
#define KEY_2			  0x32
#define KEY_3			  0x33
#define KEY_4			  0x34
#define KEY_5			  0x35
#define KEY_6			  0x36
#define KEY_7			  0x37
#define KEY_8			  0x38
#define KEY_9			  0x39

#define KEY_A			  0x41
#define KEY_B			  0x42
#define KEY_C			  0x43
#define KEY_D			  0x44
#define KEY_E			  0x45
#define KEY_F			  0x46
#define KEY_G			  0x47
#define KEY_H			  0x48
#define KEY_I			  0x49
#define KEY_J			  0x4A
#define KEY_K			  0x4B
#define KEY_L			  0x4C
#define KEY_M			  0x4D
#define KEY_N			  0x4E
#define KEY_O			  0x4F
#define KEY_P			  0x50
#define KEY_Q			  0x51
#define KEY_R			  0x52
#define KEY_S			  0x53
#define KEY_T			  0x54
#define KEY_U			  0x55
#define KEY_V			  0x56
#define KEY_W			  0x57
#define KEY_X			  0x58
#define KEY_Y			  0x59
#define KEY_Z			  0x5A