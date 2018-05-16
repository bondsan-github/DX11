#include "..\Header files\debugging.h"

void debug_out(const char * format, ...)
{
    va_list args;
    char buffer[ 1024 ];

    va_start( args, format );

    //int string_length = _vscprintf( format, args ) + 1; // _vscprintf doesn't count terminating '\0'

    vsprintf_s( buffer, sizeof( buffer ), format, args );   

    OutputDebugStringA( buffer ); //::

    va_end( args );
}

void debug_out( wchar_t * ouput_string , ... )
{
	va_list arguments;
	wchar_t buffer[ 512 ];

	va_start( arguments , ouput_string );
	
	//swprintf_s( buffer, _countof( buffer ), ouput_string, arguments);

	StringCbVPrintf( buffer , _countof( buffer ) , ouput_string , arguments );

	OutputDebugStringW( buffer );


	
	va_end( arguments );
}

void ErrorExit( LPCTSTR lpszFunction )
{
	// Retrieve the system error message for the last-error code

	LPVOID	lpMsgBuf;
	LPVOID	lpDisplayBuf;
	DWORD	dw = GetLastError( );

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
				   FORMAT_MESSAGE_FROM_SYSTEM |
				   FORMAT_MESSAGE_IGNORE_INSERTS ,
				   NULL ,
				   dw ,
				   MAKELANGID( LANG_NEUTRAL , SUBLANG_DEFAULT ) ,
				   ( LPTSTR ) &lpMsgBuf ,
				   0 , 
				   NULL );

	// Display the error message and exit the process

	lpDisplayBuf = ( LPVOID ) LocalAlloc( LMEM_ZEROINIT ,
										  ( lstrlen( ( LPCTSTR ) lpMsgBuf ) + lstrlen( ( LPCTSTR ) lpszFunction ) + 40 ) * sizeof( TCHAR ) );

	StringCchPrintf( ( LPTSTR ) lpDisplayBuf ,
					 LocalSize( lpDisplayBuf ) / sizeof( TCHAR ) ,
					 TEXT( "%s failed with error %d: %s" ) ,
					 lpszFunction ,
					 dw ,
					 lpMsgBuf );

	MessageBox( NULL , ( LPCTSTR ) lpDisplayBuf , TEXT( "Error" ) , MB_OK );

	LocalFree( lpMsgBuf );
	LocalFree( lpDisplayBuf );
	ExitProcess( dw );
}

