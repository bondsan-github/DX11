#pragma once

#include "globals.h"

//class State;

class Actor abstract
{
	public:

		virtual void try_rotate( Rotation in_rotation ) = 0;
		virtual bool try_move( Direction in_direction ) = 0;
		virtual void set_state( State * state_in ) = 0;
		virtual float get_fall_rate() const = 0;

		virtual void set_next_position() = 0;
		virtual void set_initial_position() = 0;
};