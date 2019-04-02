#pragma once

template < typename type >
class State abstract
{
	public:
		virtual State * update( type & actor , double time_delta ) = 0;
};


