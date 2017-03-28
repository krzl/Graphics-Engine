#pragma once

#include "Command.h"

namespace Kz
{
	class MouseMoveCommand : public Command
	{
		friend class MouseInputHandler;

	private:

		void OnMouseMove(int xTranslation, int yTranslation);

	protected:

		int			m_dx;
		int			m_dy;

	public:

		virtual void Execute() = 0;
	};
}