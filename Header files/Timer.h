#pragma once

#include <Windows.h>

class Timer
{
	public:
		Timer()
		{
			QueryPerformanceFrequency( & m_frequency );

			//unsigned long mask_previous = SetThreadAffinityMask( GetCurrentThread , 1 );
			// and then set it back at the end 
			//   SetThreadAffinityMask( GetCurrentThread , old_mask ) 

			// Variability of the CPU's frequency ** windows message ?

			reset();
		}

		void reset()
		{
			QueryPerformanceCounter( & m_start );

			m_current.QuadPart = 0;
			m_delta.QuadPart = 0;
		}

		void tick()
		{
			QueryPerformanceCounter( & m_current );

			m_delta.QuadPart = m_current.QuadPart - m_start.QuadPart;

			//m_delta.QuadPart /= m_frequency.QuadPart;

			//( ( ctr2 - ctr1 ) * 1.0 / freq )
		}

		long long delta() const 
		{ 
			return m_delta.QuadPart; 
		}

		void start()
		{
			QueryPerformanceCounter( & m_start );
		}

		void stop() {}
		//void pause();
		//void fps();

		//double elapsed() { return m_start - m_last; }

		void single_step() {}

	private:
		//double		m_durration_per_tick {}; //  ? ms / us 
		LARGE_INTEGER	m_frequency {};
		//const double	one_second = 1.0;
		//double		current_durration = 0.0;

		LARGE_INTEGER	m_start {};
		LARGE_INTEGER	m_current {};
		LARGE_INTEGER	m_delta { };

		bool			m_paused = true;
};