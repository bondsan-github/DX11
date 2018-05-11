//#define UNICODE //LPCWSTR lpText

//#include "DX11_game_lib.h"
#include "DX11_base.h"
#include < memory >


int WINAPI wWinMain( HINSTANCE h_instance, HINSTANCE prev_instance, LPWSTR command_line, int window_display_options )
{
	UNREFERENCED_PARAMETER( prev_instance ); 
	UNREFERENCED_PARAMETER( command_line );


    //std::unique_ptr< DX11_derived > demo( new DX11_derived( h_instance , h_window ) ); // pointer or full class?
	//DX11_derived demo( h_instance , h_window ); 
    //bool result = demo.create_dx11_device();

	DX11 demo( h_instance, window_display_options );

    return 0;
}
