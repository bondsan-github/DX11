#pragma once

#include "graphics.h"
#include "graphics_component.h"
#include "command.h"
#include "input_handler.h"
#include "state.h"
#include "timer.h"

class Game
{
	public:

		Game( const HWND in_window , const unsigned int in_resolution_width , const unsigned int in_resolution_height );

		virtual void update() = 0;
		virtual void render() = 0;

		virtual void input( const int in_key ) = 0;
		
	protected:

		Timer		timer;
		Graphics	graphics;
		HWND		window;

		Input_handler	input_handler;
		Command *		command = nullptr;
		//Input
		//Sound
		// (physics)				

		State< Game > *	state = nullptr; //unique_ptr
};