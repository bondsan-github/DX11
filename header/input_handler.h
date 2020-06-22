#pragma once

#include <map>

#include "command.h"
#include "types.h"

template< typename Type >
class Input_handler
{
	public:

		Input_handler()	{}
	
		Command< Type > * handle_input( const unsigned int in_key )
		{
			const auto result = command_map.find( in_key );

			if( result != command_map.end() ) return result->second;
			else return nullptr;
		}

		void add_command( unsigned int in_key , Command< Type > * in_command )
		{
			command_map.insert( std::make_pair( in_key , in_command ) ); 
		}

	private:

		std::map< unsigned int , Command< Type > * > command_map;
};