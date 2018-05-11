#pragma once

template< typename T >
void safe_release( T * release_pointer )
{
	if ( release_pointer != nullptr ) release_pointer->Release( );

	release_pointer = nullptr;
}

//#include "utilities.tpp";

// void safe_release( void * pointer) { if( pointer != nullptr) { pointer->Release(); pointer = nullptr; } }