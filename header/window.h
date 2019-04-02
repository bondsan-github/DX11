#pragma once

#include <Windows.h>
#include "MS_keyboard.h"
#include "game.h"
		
inline LRESULT CALLBACK window_procedure( HWND hwnd , unsigned int message , WPARAM wParam , LPARAM lParam )
{
	auto game = reinterpret_cast<Game*>(GetWindowLongPtr( hwnd , GWLP_USERDATA));

	switch( message )
	{
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
			game->input( static_cast< int >( wParam ) );
			//DirectX::Keyboard::ProcessMessage( message , wParam , lParam );
			return 0;
		}

		default:
			return DefWindowProc( hwnd , message , wParam , lParam );
	}

	return 0;
}

inline HWND initialise( const HINSTANCE in_instance , const unsigned int in_width , const unsigned int in_height )
{
	WNDCLASSEX		classex{};
	LPTSTR			name = TEXT( "DX11 game" );
	RECT			size{};
	HWND			window{};

	classex.cbSize = sizeof( WNDCLASSEX );
	classex.style = CS_HREDRAW | CS_VREDRAW;
	classex.lpfnWndProc = window_procedure;
	classex.cbClsExtra = 0;									// extra bytes to allocate following the window-class structure
	classex.cbWndExtra = 0;									// extra bytes to allocate following the window instance
	classex.hInstance = in_instance;
	classex.hIcon = LoadIcon( in_instance , L"IDI_ICON" );// a handle to an icon resource
	classex.hCursor = LoadCursor( nullptr , IDC_ARROW );
	classex.hbrBackground = ( HBRUSH )( COLOR_GRAYTEXT + 1 );		// brush_background;
	classex.lpszMenuName = nullptr;								// name of the class menu, as the name appears in the resource file
	classex.lpszClassName = name;									// * Must match CreateWindowEx 2nd arg = lpClassName
	classex.hIconSm = LoadIcon( in_instance , L"IDI_ICON" );// small icon

	RegisterClassEx( &classex );

	RECT desktop_size{};
	GetWindowRect( GetDesktopWindow() , &desktop_size );

	int center_x = ( desktop_size.right - in_width ) / 2;
	int center_y = ( desktop_size.bottom - in_height ) / 2;

	size = { 0l, 0l, static_cast< long >( in_width ), static_cast< long >( in_height ) };

	AdjustWindowRect( &size , WS_OVERLAPPEDWINDOW , FALSE );

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
							 in_instance ,				// handle to the instance of the module to be associated with the window
							 nullptr );					// Pointer to a value to be passed to the window through the CREATESTRUCT structure 


	ShowWindow( window , SW_SHOWNORMAL );// window_display_options );	

	return window;
}


