#pragma once

#include <string>
#include <vector>
#include <map>
#include <Windows.h>

#include "Engine\Window.h"

namespace Kz
{
	class Viewport;
	class RenderSystem;

	class WinWindow : public Window
	{
		friend LRESULT CALLBACK WndProcCB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:

		bool							m_updatePosition = true;
		int								m_mouseHiddenPosX = 0;
		int								m_mouseHiddenPosY = 0;

		LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT KeyboardCallback(int key, unsigned int message);
		LRESULT MouseCallback(int x, int y, unsigned int message);
		LRESULT MouseMoveCallback(int x, int y);

		static std::vector<WinWindow*> m_windows;

	public:

		HWND							m_window;

		WinWindow(int width, int height, GraphicsDevice& gd, std::string windowTitle);

		virtual void					Close();
		virtual void					MakeCurrent() const;
		virtual void					SwapBuffers();
		virtual void					Update(RenderSystem& renderSystem);
		virtual void					LockMouse();
		virtual void					UnlockMouse();
		void							SetMousePosition(int xPos, int yPos);


		static WinWindow*				GetWindow(HWND hwnd);
	};
}