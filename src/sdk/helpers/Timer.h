#pragma once

class Timer
{
public:
	Timer(float duration)
	{
		this->timer_duration = duration;
		this->last_time = 0.f;
		this->time_remaining = 0.f;
	}

	//Make sure the timer var is static
	bool has_finished();
	void reset();
	void set_duration(float duration);
	float get_time_remaining();

private:
	float timer_duration;
	float last_time;
	float time_remaining;
};

