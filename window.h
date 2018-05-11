#pragma once

//#define UNICODE

//#include <afxwin.h>
//#include <afx.h>
#include <Windows.h>

LRESULT CALLBACK	window_messaging( HWND hwnd , UINT message , WPARAM wParam , LPARAM lParam );
HWND				create_window( HINSTANCE h_instance , UINT window_width, UINT window_height, RECT * out_client_size );
					// int window_display_options );

//int				message_loop( );