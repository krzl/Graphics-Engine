#include "WinWindow.h"
#include "Engine/RenderSystem.h"
#include "Engine/Viewport.h"
#include "Engine\DirectX\GraphicsDeviceDX.h"

#include <iostream>
#include <windowsx.h>

#include "Utility\Logger.h"

namespace Kz
{

	std::vector<WinWindow*> WinWindow::m_windows;

	LRESULT CALLBACK WndProcCB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		for (size_t i = 0; i < WinWindow::m_windows.size(); i++){
			if (WinWindow::m_windows[i]->m_window == hWnd){
				return WinWindow::m_windows[i]->WndProc(hWnd, message, wParam, lParam);
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WinWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_ACTIVATE:
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			Close();
			return 0;

		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);

		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;

		case WM_KEYDOWN:
		case WM_KEYUP:
			return KeyboardCallback(wParam, message);
			
		case WM_SIZE:
			OnResize( LOWORD( lParam ), HIWORD( lParam ) );
			return 0;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			return MouseCallback(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), message);

		case WM_MOUSEMOVE:
			return MouseMoveCallback(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WinWindow::KeyboardCallback(int key, unsigned int message)
	{
		m_keys[key] = message == WM_KEYDOWN ? (m_keys[key] == STATE_DOWN ? STATE_DOWN : STATE_PRESSED) : STATE_RELEASED;
		return 0;
	}

	LRESULT WinWindow::MouseCallback(int x, int y, unsigned int message)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
			m_mouseButtons[MOUSE_LB] = STATE_PRESSED;
			break;
		case WM_LBUTTONUP:
			m_mouseButtons[MOUSE_LB] = STATE_RELEASED;
			break;
		case WM_RBUTTONDOWN:
			m_mouseButtons[MOUSE_RB] = STATE_PRESSED;
			break;
		case WM_RBUTTONUP:
			m_mouseButtons[MOUSE_RB] = STATE_RELEASED;
			break;
		}
		return 0;
	}

	LRESULT WinWindow::MouseMoveCallback(int x, int y)
	{
		if (m_updatePosition)
		{
			m_mousePosX = x;
			m_mousePosY = y;
		}

		return 0;
	}

	WinWindow::WinWindow(int width, int height, GraphicsDevice& gd, std::string windowName) : 
		Window(width, height, gd)
	{
		WNDCLASS wc;
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= WndProcCB;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= GetModuleHandle(NULL);
		wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
		wc.hCursor			= LoadCursor(0, IDC_ARROW);
		wc.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName		= 0;
		wc.lpszClassName	= "WindowClass";

		RegisterClass(&wc);

		RECT windowRect = { 0, 0, width, height };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
		int totalWindowWidth = windowRect.right - windowRect.left;
		int totalWindowHeight = windowRect.bottom - windowRect.top;

		m_window = CreateWindow("WindowClass", windowName.c_str(), WS_OVERLAPPEDWINDOW, 
			CW_USEDEFAULT, CW_USEDEFAULT, totalWindowWidth, totalWindowHeight, 0, 0, NULL, 0);

		m_width = width;
		m_height = height;

		ShowWindow(m_window, SW_SHOW);
		SetForegroundWindow(m_window);
		SetFocus(m_window);


		MSG msg = { 0 };
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!m_window)
			Close();
		else
			MakeCurrent();

		GraphicsDeviceDX::SetHWnd(m_window);
		m_windows.push_back(this);
	}

	void WinWindow::MakeCurrent() const
	{
	}

	void WinWindow::Update(RenderSystem& renderSystem)
	{
		for (int i = 0; i < MAX_KEY; i++)
		{
			if (m_keys[i] == STATE_PRESSED)
				m_keys[i] = STATE_DOWN;
			if (m_keys[i] == STATE_RELEASED)
				m_keys[i] = STATE_UP;
		}

		for (int i = 0; i < MOUSE_BUTTONS_COUNT; i++)
		{
			if (m_mouseButtons[i] == STATE_PRESSED)
				m_mouseButtons[i] = STATE_DOWN;
			if (m_mouseButtons[i] == STATE_RELEASED)
				m_mouseButtons[i] = STATE_UP;
		}

		MSG msg = { 0 };

		if (m_isCursorLocked)
		{
			m_prevMousePosX = m_mousePosX;
			m_prevMousePosY = m_mousePosY;
		}
		else
		{
			m_prevMousePosX	= m_width / 2;
			m_prevMousePosY	= m_height / 2;
			m_mousePosX		= m_width / 2;
			m_mousePosY		= m_height / 2;
		}

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!m_isCursorLocked)
		{
			SetMousePosition(m_width / 2, m_height / 2);

			m_updatePosition = true;
		}

		Window::Update(renderSystem);
		SwapBuffers();
	}

	void WinWindow::SwapBuffers()
	{
	}

	void WinWindow::Close()
	{
		m_shouldClose = true;
	}

	void WinWindow::LockMouse()
	{
		if (m_isCursorLocked)
		{
			ShowCursor(false);
			GetMousePosition(m_mouseHiddenPosX, m_mouseHiddenPosY);
			SetMousePosition(m_width / 2, m_height / 2);
			m_isCursorLocked = false;
		}
	}

	void WinWindow::UnlockMouse()
	{
		if (!m_isCursorLocked)
		{
			ShowCursor(true);
			SetMousePosition(m_mouseHiddenPosX, m_mouseHiddenPosY);
			m_updatePosition = true;
			m_isCursorLocked = true;
			m_prevMousePosX	= m_mousePosX = m_mouseHiddenPosX;
			m_prevMousePosY = m_mousePosY = m_mouseHiddenPosY;
		}
	}

	void WinWindow::SetMousePosition(int xPos, int yPos)
	{
		POINT point;
		point.x = xPos;
		point.y = yPos;
		m_updatePosition = false;
		ClientToScreen(m_window, &point);
		SetCursorPos(point.x, point.y);

		MSG msg = { 0 };
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	WinWindow* WinWindow::GetWindow(HWND hwnd)
	{
		for (size_t i = 0; i < m_windows.size(); i++)
		{
			if (m_windows[i]->m_window == hwnd)
			{
				return m_windows[i];
			}
		}

		return NULL;
	}
}