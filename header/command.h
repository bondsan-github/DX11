#pragma once

#include <map>

template< typename Type >
class Command abstract
{
	public:

		virtual void execute( Type & actor, const double delta ) = 0;

	//private:
		//float delay;
};