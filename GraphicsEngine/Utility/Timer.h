#pragma once

#include <chrono>

namespace Kz
{
	class Timer
	{
	private:

		std::chrono::high_resolution_clock::time_point startTime;

	public:

		void    Start();
		void    Reset();
		double  GetDelta();
	};
}