#pragma once

#include "window.h"
#include "graphics.h"
#include "graphics_component.h"
#include "command.h"
#include "input_handler.h"
#include "state.h"
#include "timer.h"

class Application abstract
{
	public:

		Application( const HWND in_window , const unsigned int in_resolution_width , const unsigned int in_resolution_height );

		virtual void update() = 0;
		virtual void render() = 0;
		virtual void input( const int in_key ) = 0; // unsigned ?

		//event_handler
		
	protected:

		HWND		window;
		Timer		timer;
		Graphics	graphics;
		//Audio		audio;
		

		//DirectX::Keyboard		keyboard;
		//Camera					camera;
		
		// class Event_listener abstract {};

		//Input
		//Sound
		// (physics)				

		//State< app > *	state = nullptr; //unique_ptr
};