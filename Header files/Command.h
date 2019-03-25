#pragma once

#include <map>

template< typename type >
class Command
{
	public:

		virtual void execute( type & actor ) = 0;
};