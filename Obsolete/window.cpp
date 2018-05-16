#include "window.h"
#include "debugging.h"



HWND create_window( const HINSTANCE in_h_instance , const UINT in_client_width , const UINT in_client_height, RECT * out_client_size ) // int window_display_options )
{
	//debug_out( "\n***create window debug out ***\n" ); 
	//CBrush brush_background( 0x00ff0000 );  //0x00bbggrr

	// static Window::window_messaging

	// SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()) );
	// VOID WINAPI GetStartupInfo( _Out_ LPSTARTUPINFO lpStartupInfo );

	WNDCLASSEX window_class { };

	window_class.cbSize			= sizeof( WNDCLASSEX );
	window_class.style			= CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc	= window_messaging; 
	window_class.cbClsExtra		= 0;									//extra bytes to allocate following the window-class structure
	window_class.cbWndExtra		= 0;									//extra bytes to allocate following the window instance
	window_class.hInstance		= in_h_instance;
	window_class.hIcon			= LoadIcon( in_h_instance , L"IDI_ICON" );	// a handle to an icon resource
	window_class.hCursor		= LoadCursor( nullptr , IDC_ARROW );
	window_class.hbrBackground	= ( HBRUSH )( COLOR_GRAYTEXT + 1 );		//brush_background;
	window_class.lpszMenuName	= nullptr;								//name of the class menu, as the name appears in the resource file
	window_class.lpszClassName	= L"DX11 version1";
	window_class.hIconSm		= LoadIcon( in_h_instance , L"IDI_ICON" );	//A handle to a small icon that is associated with the window class

	if( ! RegisterClassEx( & window_class ) ) { ErrorExit( L"RegisterClassEX error" ); }

	RECT desktop_size { };
	GetWindowRect( GetDesktopWindow() , & desktop_size );

	int center_x = ( desktop_size.right - in_client_width ) / 2;
	int center_y = ( desktop_size.bottom - in_client_height ) / 2;

	RECT rectangle_window { 0l, 0l, in_client_width, in_client_height };
			
	AdjustWindowRect( & rectangle_window , WS_OVERLAPPEDWINDOW , FALSE );

	unsigned long window_styles = WS_OVERLAPPEDWINDOW;//WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	
	HWND h_window = CreateWindowEx( 0,//WS_EX_OVERLAPPEDWINDOW ,	// Extended Window Style
									L"DX11 version1" ,			// must be in the low-order word of lpClassName
									L"" ,						// window title displayed in the title bar
									window_styles ,				// window style
									center_x ,					// initial horizontal position 
									center_y ,					// initial vertical position of the window.
									rectangle_window.right - rectangle_window.left,				// width, in device units, of the window
									rectangle_window.bottom - rectangle_window.top,				// height, in device units, of the window
									nullptr ,					// handle to the parent or owner window 
									nullptr ,					// handle to a menu - name of menu resource e.g. "MainMenu"
									in_h_instance ,				// handle to the instance of the module to be associated with the window
									nullptr );					// Pointer to a value to be passed to the window through the CREATESTRUCT structure 

	if ( ! h_window ) {	ErrorExit( L"CreateWindowEx failed" ); }	
	
	ShowWindow( h_window , SW_SHOWNORMAL );// window_display_options );	

	//RECT client_area {};
	GetClientRect( h_window , out_client_size );

	return h_window;
}

