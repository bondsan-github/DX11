#include "window.h"

class Game;
extern Game * global_game;

LRESULT CALLBACK global_window_messaging( HWND hwnd , unsigned int message , WPARAM wParam , LPARAM lParam )
{
	return global_window->messaging( hwnd , message , wParam , lParam );
}


HWND Window::initialise( const HINSTANCE in_instance , const unsigned int in_width , const unsigned int in_height ) //Game * const in_game
{
	global_window = this;

	instance	= in_instance;
	width		= in_width;
	height		= in_height;

	classex.cbSize			= sizeof( WNDCLASSEX );
	classex.style			= CS_HREDRAW | CS_VREDRAW;
	classex.lpfnWndProc		= global_window_messaging;
	classex.cbClsExtra		= 0;									// extra bytes to allocate following the window-class structure
	classex.cbWndExtra		= 0;									// extra bytes to allocate following the window instance
	classex.hInstance		= instance;
	classex.hIcon			= LoadIcon( instance , L"IDI_ICON" );	// a handle to an icon resource
	classex.hCursor			= LoadCursor( nullptr , IDC_ARROW );
	classex.hbrBackground	= ( HBRUSH )( COLOR_GRAYTEXT + 1 );		// brush_background;
	classex.lpszMenuName	= nullptr;								// name of the class menu, as the name appears in the resource file
	classex.lpszClassName	= name;									// * Must match CreateWindowEx 2nd arg = lpClassName
	classex.hIconSm			= LoadIcon( instance , L"IDI_ICON" );	// small icon

	RegisterClassEx( &classex );
	//if( !RegisterClassEx( &classex ) ) { ErrorExit( L"RegisterClassEX error" ); }

	RECT desktop_size {};
	GetWindowRect( GetDesktopWindow() , & desktop_size );

	int center_x = ( desktop_size.right - width ) / 2;
	int center_y = ( desktop_size.bottom - height ) / 2;

	size = { 0l, 0l, static_cast< long >( width ), static_cast< long >( height ) };

	AdjustWindowRect( & size , WS_OVERLAPPEDWINDOW , FALSE );

	unsigned long window_styles = WS_OVERLAPPEDWINDOW;//WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;

	window = CreateWindowEx( 0 ,						// Extended Window Style // WS_EX_OVERLAPPEDWINDOW ,
							 name ,						// must be in the low-order word of lpClassName
							 L"" ,						// window title displayed in the title bar
							 window_styles ,			// window style
							 center_x ,					// initial horizontal position 
							 center_y ,					// initial vertical position of the window.
							 size.right - size.left ,	// width, in device units, of the window
							 size.bottom - size.top ,	// height, in device units, of the window
							 nullptr ,					// handle to the parent or owner window 
							 nullptr ,					// handle to a menu - name of menu resource e.g. "MainMenu"
							 instance ,					// handle to the instance of the module to be associated with the window
							 nullptr );					// Pointer to a value to be passed to the window through the CREATESTRUCT structure 


	ShowWindow( window , SW_SHOWNORMAL );// window_display_options );	

	//RECT client_area {};
	//GetClientRect( h_window , & client_area );

	std::unique_ptr< DirectX::Keyboard > keyboard = std::make_unique< DirectX::Keyboard >();

	return window;
}
	
LRESULT CALLBACK Window::messaging( HWND hwnd , unsigned int message , WPARAM wParam , LPARAM lParam )
{
	//PAINTSTRUCT paint_struct;
	//HDC hDC;
	// auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch( message )
	{
		//case WM_PAINT:
		//	hDC = BeginPaint( hwnd , &paint_struct );
		//	EndPaint( hwnd , &paint_struct );
		//	break;

		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		case WM_ACTIVATEAPP:
		{
			DirectX::Keyboard::ProcessMessage( message , wParam , lParam );
			//DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			return 0;
		}

		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
		{
			//DirectX::Mouse::ProcessMessage( message , wParam , lParam );
			return 0;
		}

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		{
			DirectX::Keyboard::ProcessMessage( message , wParam , lParam );
			return 0;
		}

		default:
			return DefWindowProc( hwnd , message , wParam , lParam );
	}

	return 0;
}

WPARAM Window::message_loop()
{
	while( win_msg.message != WM_QUIT )
	{
		if( PeekMessage( &win_msg , nullptr , 0 , 0 , PM_REMOVE ) )
		{
			TranslateMessage( &win_msg );	// receive character input from the keyboard.
			DispatchMessage( &win_msg );
		}
		else
		{
			global_game->update();
			// add_event(game_update);
		}
	}

	return win_msg.wParam;
}