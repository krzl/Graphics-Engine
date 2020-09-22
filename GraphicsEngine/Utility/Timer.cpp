#include "Timer.h"

using namespace std::chrono;

namespace Kz
{
	void Timer::Start()
	{
		startTime = high_resolution_clock::now();
	}

	void Timer::Reset()
	{
		Start();
	}

	double Timer::GetDelta()
	{
		return (double) duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count();
	}
}