#pragma once

//class Actor;
#include <string>

template< typename Type >
class State abstract
{
	public:

		//State( std::wstring in_name = std::wstring() ) 
		//	: name( in_name ) {}

		virtual void input( Type & in_actor , const int in_input );
		virtual State * update( Type & in_actor , double time_delta ) = 0;

		//void set_name( const std::wstring in_name ) { name = in_name; }
		//std::wstring get_name() { return name; }

	private:

		//std::wstring name = std::wstring();
};


