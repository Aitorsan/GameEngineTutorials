#ifndef TIMER_HPP
#define TIMER_HPP
#include <chrono>
#include <iostream>
class Timer
{
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;
	std::chrono::time_point<clock_t> begin;
public:
	Timer():begin( clock_t::now()){ std::cout << "Timer started\n"; }
	void reset() { begin = clock_t::now(); }
	double elapsed() const
	{
		double seconds =  std::chrono::duration_cast<second_t>(clock_t::now() - begin).count();
		std::cout << "Time elapsed: " << seconds << " seconds\n";
		return seconds;
	}

};
#endif // TIMER_HPP