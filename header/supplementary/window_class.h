#pragma once

#include <Windows.h>

#include "MS_keyboard.h"

class Window
{
	public:

		Window() {}
				
		HWND				initialise( const HINSTANCE h_instance , const unsigned int in_width , const unsigned int in_height );
		//void size_update();

		const HWND			get_window() const { return window; }

		LRESULT CALLBACK	messaging( HWND hwnd , unsigned int message , WPARAM wParam , LPARAM lParam );

	private:

		HRESULT result	{ E_FAIL };

		LPTSTR			name = TEXT( "DX11 game" );
		HINSTANCE		instance {};
		HWND			window {};
		RECT			client_area {};
		int				display_options {0};
		unsigned long	style {};
		unsigned int	width { 800 };
		unsigned int	height { 600 };

		MSG				win_msg{};
		/*    HWND        hwnd;
			  UINT        message;
			  WPARAM      wParam;
			  LPARAM      lParam;
			  DWORD       time;
			  POINT       pt; */
 		WPARAM			message_loop();

		WNDCLASSEX		classex {};
		RECT			size{};

		std::unique_ptr<DirectX::Keyboard> keyboard;
};

static Window * global_window = nullptr;