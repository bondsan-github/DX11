#pragma once

#include <Windows.h>

class Clock
{
	//unsigned __int64 m_timeCycles;
	ULARGE_INTEGER m_ll_time_cycles;
	ULARGE_INTEGER m_ll_time_scale;
	bool	  m_b_is_paused;
	static LARGE_INTEGER s_ll_cycles_per_second;

	// BOOL WINAPI QueryPerformanceCounter( _Out_ LARGE_INTEGER * lpPerformanceCount );
	 
	static inline unsigned __int64 secondsToCycles( float timeSeconds )
	{
		return ( unsigned __int64 ) ( timeSeconds * s_ll_cycles_per_second );
	}

	// WARNING: Dangerous -- only use to convert small
	// durations into seconds.
	static inline float cyclesToSeconds( unsigned __int64 timeCycles )
	{
		return ( float ) timeCycles / s_cyclesPerSecond;
	}

	public:
	// Call this when the game first starts up.
	static void init()
	{
		// s_cyclesPerSecond= ( float ) readHiResTimerFrequency();

		QueryPerformanceFrequency( & s_ll_cycles_per_second );
	}
	// Construct a clock. (Notice the use of 'explicit' to
	// prevent automatic conversion from F32 to Clock.)
	explicit Clock( float startTimeSeconds = 0.0f ) :
		m_timeCycles( secondsToCycles( startTimeSeconds ) ) ,
		m_timeScale( 1.0f ) , // default to unscaled
		m_isPaused( false ) // default to running
	{ }

	// Return the current time in cycles. NOTE that we do
	// not return absolute time measurements in floating-
	// point seconds, because a 32-bit float doesn't have
	// enough precision. See calcDeltaSeconds().
	unsigned __int64 getTimeCycles() const
	{
		return m_timeCycles;
	}
	// Determine the difference between this clock's
	// absolute time and that of another clock, in
	// seconds. We only return time deltas as floating-
	// point seconds, due to the precision limitations of
	// a 32-bit float.
	float calcDeltaSeconds( const Clock& other )
	{
		unsigned __int64 dt = m_timeCycles - other.m_timeCycles;
		return cyclesToSeconds( dt );
	}
	// This function should be called once per frame,
	// with the real measured frame time delta in seconds.
	void update( float dtRealSeconds )
	{
		if( !m_isPaused )
		{
			unsigned __int64 dtScaledCycles
				= secondsToCycles( dtRealSeconds * m_timeScale );
			m_timeCycles += dtScaledCycles;
		}
	}
	void setPaused( bool wantPaused )
	{
		m_isPaused = wantPaused;
	}
	bool isPaused() const
	{
		return m_isPaused;
	}
	void setTimeScale( float scale )
	{
		m_timeScale = scale;
	}
	float getTimeScale() const
	{
		return m_timeScale;
	}
	void singleStep()
	{
		if( m_isPaused )
		{
			// Add one ideal frame interval; don't forget
			// to scale it by our current time scale!
			unsigned __int64 dtScaledCycles = secondsToCycles(	( 1.0f / 30.0f ) * m_timeScale );
			m_timeCycles += dtScaledCycles;
		}
	}
};