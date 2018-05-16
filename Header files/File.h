#pragma once

#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <memory>

#include <Windows.h>

#include "..\Header files\debugging.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::wstring;
using std::ifstream;

//implemented format loaders
//enum class format { bitmap, wicimage, truetype };

/*
inline format operator++( format& x ) { return x = ( format ) ( std::underlying_type<format>::type( x ) + 1 ); }
inline format operator*( format c ) { return c; }
inline format begin( format r ) { return format::First; }
inline format end( format r ) { format l = format::Last; return l++; }
*/

class File // : public stream
{
	public:

		File();
		File( const wstring filename );

		// delete copy construcors and opperators
		
		~File();// { };

		void load( const wstring filename );

		//const type what_is( void ) const { return m_format; }

		const char * content() const { m_content->data(); }
		
		const unsigned long size( void ) { return m_size; }

	private:

		ifstream			m_in_file_stream {};
		//HANDLE			m_h_file_handle {};

		unsigned long		m_size {}; //size_t
		//unsigned long		m_ul_bytes_read {};

		//format m_format {};

		// char to pass on negative values
		unique_ptr< vector< char > > m_content = make_unique< vector< char > >();
};