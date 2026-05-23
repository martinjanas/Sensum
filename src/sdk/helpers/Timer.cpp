#include "Timer.h"
#include "../sdk.h"

bool Timer::has_finished()
{
	this->time_remaining = g::global_vars->GetCurtime() - last_time;
	if (this->time_remaining >= timer_duration)
	{
		last_time = g::global_vars->GetCurtime();

		return true;
	}
	return false;
}

void Timer::reset()
{
	last_time = g::global_vars->GetCurtime();
}

void Timer::set_duration(float duration)
{
	timer_duration = duration;
}

float Timer::get_time_remaining()
{
	return this->time_remaining;
}