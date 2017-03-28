#include "MouseMoveCommand.h"

namespace Kz
{
	void MouseMoveCommand::OnMouseMove(int xTranslation, int yTranslation)
	{
		m_dx = xTranslation;
		m_dy = yTranslation;
		Execute();
	}
}