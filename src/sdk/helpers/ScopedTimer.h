#pragma once
#include <chrono>

class ScopedTimer
{
public:
	ScopedTimer(const char* func)
	{
		this->func = func;
		start = std::chrono::high_resolution_clock::now();
	}

	ScopedTimer(ScopedTimer& timer) = delete;

	~ScopedTimer()
	{
		end = std::chrono::high_resolution_clock::now();

		auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		printf("%s took %lld ms, %lld ns\n", func, duration_ms, duration_ns);
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	const char* func;
};