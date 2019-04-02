#pragma once

#include <map>

#include "command.h"
//#include "MS_Keyboard.h"
#include "types.h"

//template< typename type >
class Input_handler
{
	public:

		Input_handler()
		{
			// Methods to bind commands...
			//command_map.insert( make_pair( DirectX::Keyboard::Keys::Z , new Rotate_command( Rotation::clock_wise ) ) );
			//command_map.insert( make_pair( DirectX::Keyboard::Keys::X , new Rotate_command( Rotation::counter_clock_wise ) ) );
		}
	
		//Command<type> * handle_input( const  unsigned int in_key )
		Command * handle_input( const unsigned int in_key )
		{
			const auto result = command_map.find( in_key );

			if( result != command_map.end() ) return result->second;
			else return nullptr;
		}

		//void add_command( unsigned int in_key , Command<type> in_command )
		void add_command( unsigned int in_key , Command * in_command )
		{
			command_map.insert( std::make_pair( in_key , in_command ) ); 
		}

	private:

		//std::map< unsigned int , Command<type> * > command_map;
		std::map< unsigned int , Command * > command_map;
};