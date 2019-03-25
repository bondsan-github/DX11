#pragma once 

#include <stdio.h>
#include <stdarg.h>
#include <strsafe.h>
#include <Windows.h>
#include <string>

void debug_out( const char * format , ... );
void debug_out( wchar_t * message , ... );

void debug_out( std::wstring ouput_string , ... );

void ErrorExit( LPCTSTR lpszFunction );

//void debug_out( wchar_t format[100], ... );




