#pragma once

#include "globals.h"

class Game_actor abstract
{
	public:
		virtual void try_rotate( Rotation in_rotation ) = 0;
		virtual void try_move( Direction in_direction ) = 0;
};