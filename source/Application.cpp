#include "Application.h"

//template Game< Tetrimino >

Application::Application( const HWND in_window , const unsigned int in_resolution_width , const unsigned int in_resolution_height )
	:window( in_window )
{
	//window_handle = window.initialise( in_instance , in_resolution_width , in_resolution_height );

	graphics.initialise( in_window );

	Graphics_component::video_device = graphics.get_video_device();

	timer.start();
	timer.tick();
	
	//window_message_loop(); // must be in thread that created the window i.e. not the Game Class 
}

//void Game::update( )
//{
//	timer.tick();
//
//	graphics.clear_render_target( DirectX::Colors::Red );
//	
//	//State< Game > * new_state = state->update( * this , 0.0 );
//
//	//if( new_state )
//	//{
//	//	delete state;
//	//	state = new_state;
//	//}
//
//	graphics.present();	
//}