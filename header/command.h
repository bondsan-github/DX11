#pragma once

#include <map>

#include "game_actor.h"

//template< typename type >
class Command abstract
{
	public:

		virtual void execute( Game_actor & actor ) = 0;
};