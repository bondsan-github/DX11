#pragma once

#include <Windows.h>

class Timer
{
	public:

		Timer()
		{
			QueryPerformanceFrequency( & ticks_per_second );

			// Variability of the CPU's frequency ** windows message ?
			// unsigned long mask_previous = SetThreadAffinityMask( GetCurrentThread , 1 );
			// and then set it back at the end 
			// SetThreadAffinityMask( GetCurrentThread , old_mask ) 

			//reset();
		}

		void reset()
		{
			QueryPerformanceCounter( & ticks_start );
			ticks_current = ticks_start;
		}

		void tick()
		{
			ticks_previous = ticks_current;

			QueryPerformanceCounter( & ticks_current );

			ticks_elapsed.QuadPart = ticks_current.QuadPart - ticks_start.QuadPart;
						
			//m_delta.QuadPart /= m_frequency.QuadPart;
			//( ( ctr2 - ctr1 ) * 1.0 / freq )
		}

		double get_delta()
		{ 
			LARGE_INTEGER ticks_delta { 0 };

			ticks_delta.QuadPart = ticks_current.QuadPart - ticks_previous.QuadPart;

			double time_delta = static_cast< double >( ticks_delta.QuadPart ) / static_cast< double >( ticks_per_second.QuadPart );

			// if( time_delta < 0.0f ) time_delta = 0.0f;

			return time_delta;	
		}

		double get_total()
		{
			double total = static_cast< double >( ticks_elapsed.QuadPart ) / static_cast< double >( ticks_per_second.QuadPart );

			return total;
		}

		void start()
		{
			QueryPerformanceCounter( & ticks_start );
		}

		void stop() {}
		//void pause();
		//void fps();

		//double elapsed() { return m_start - m_last; }

		void single_step() {}

		// set_alarm

	private:

		LARGE_INTEGER	ticks_per_second { 0 };
		//const double	one_second = 1.0;
		double			seconds_per_tick { 0 };

		LARGE_INTEGER	ticks_start { 0 } ,
						ticks_current { 0 },
						ticks_previous { 0 },
						ticks_elapsed { 0 };

		
		// frames_last_main_loop
		// int framesThisSecond;
		// static int framesSkipped;

		bool			paused = true;
};