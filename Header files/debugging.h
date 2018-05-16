#pragma once 

#include <stdio.h>
#include <stdarg.h>
#include <strsafe.h>
#include <Windows.h>

void debug_out( const char * format , ... );
void debug_out( wchar_t * message , ... );

void ErrorExit( LPCTSTR lpszFunction );

//void debug_out( wchar_t format[100], ... );




