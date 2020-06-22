
#include "Image.h"

void Bitmap::load( const wstring in_filename )
{
	//File file( in_filename );
		
	ifstream	m_file { };
	m_file.open( in_filename , std::ios::binary | std::ios::beg ); // begining is default

	if( !m_file.fail() && !m_file.eof() )
	{
		//The casting ( char* )&N gets the pointer to N with & the address operator and casts the address to a char pointer
		m_file.read( reinterpret_cast< char * >( & m_file_header ) , sizeof( BITMAPFILEHEADER ) );

		// check file is a bitmap image
		// if(m_bf_header.bfType != "BM" | 0x4d42 )

		m_file.read( reinterpret_cast< char * >( & m_bmp_header ) , sizeof( BITMAPV5HEADER ) );

	}		

	if( m_bmp_header.bV5Height < 0 ) { m_flipped = true; }

	width( m_bmp_header.bV5Width );
	height( m_bmp_header.bV5Height );

	// = make_unique< vector< _int8 > >();
	unique_ptr< vector< _int8 > > m_vec_file = make_unique< vector< _int8 > >();

	switch( m_bmp_header.bV5BitCount )
	{
		case 32:
		{
			/*if( m_bmp_header.bV5SizeImage > 0 )
			{
				m_vec_abgr->resize( m_bmp_header.bV5SizeImage );
			}
			else
			{
				m_vec_abgr->resize( ( m_bmp_header.bV5Width * 4 ) * labs( m_bmp_header.bV5Height ) );
			}*/

			// copy pixel data from file vector into abgr vector
			//m_vec_abgr->assign( m_vec_file->begin() + m_file_header.bfOffBits , m_vec_file->end() );
			
			// swap R and B texel components
			// source bitmap in unsigned int 32 bit - a8 b8 g8 r8  
			// desintation DXGI_FORMAT_R8 G8 B8 A8
						
			//for( auto const & pixel_component : * m_p_vector_image_data_raw.get() ) //const & needs &*

			//seek to pixel array
			//m_file.read();

			vector< _int8 >::const_iterator const_itr;// = m_p_vector_image_data_raw->begin();
			
			for( const_itr = m_vec_file->begin() + m_file_header.bfOffBits; const_itr != m_vec_file->end(); const_itr += 4 )
			{
				_int8 alpha	= *( const_itr + 0 );
				_int8 red	= *( const_itr + 1 );
				_int8 green	= *( const_itr + 2 );
				_int8 blue	= *( const_itr + 3 );

				// unsigned char temp_alpha	= *( const_itr + 0 );
				// alpha = blue
				// blue = temp

				m_rgba->push_back( red );
				vector_rgba()->push_back( green );
				vector_rgba()->push_back( blue );
				vector_rgba()->push_back( alpha );
			}

			// source image iterator pointing to the first element
			vector< uchar >::const_iterator itr_source_begin = vector_rgba()->begin();

			// source iterators defining the region of elements to copy
			vector< uchar >::const_iterator itr_copy_start;
			vector< uchar >::const_iterator itr_copy_end;
			
			// row width in bytes
			unsigned int ui_bytes_per_row = m_width * 4;		

			// temp vector to store the filpped image
			vector< uchar > image_flipped;  // mpv_raw->size() ;

			// iterator pointing to the end of the filpped image to receive bytes
			vector< unsigned char >::iterator itr_image_flipped = image_flipped.end();

			// start at last row in source
			// copy row into new vector
			for( int row = mui_height - 1; row >= 0; --row )
			{
				itr_copy_start	= itr_source_begin	+ ( ui_bytes_per_row * row );
				itr_copy_end	= itr_copy_start	+ ( ui_bytes_per_row );

				image_flipped.insert( itr_image_flipped , itr_copy_start , itr_copy_end );

				itr_image_flipped = image_flipped.end();
			}
			
			*vector_rgba() = image_flipped;

			// Normalise colour components from [0..255] ( 2^8 - 1 ) too [0..1] UNORM range
			/*			
			for( auto const & pixel_component : *m_p_vector_image_data_raw )
			{
				m_p_vector_image_data_uniform->push_back( static_cast< float >( pixel_component ) / 255.0f );
			}
			*/

		} break;


		default:
			break;
	}
}

// if biHeight  < 0 the bitmap is bottom-up with the origin at the lower left corner
// read in data starting from last row --

// The bits representing the bitmap pixels are packed in rows.
// The size of each row is rounded up to a multiple of 4 bytes( a 32 - bit DWORD ) by padding.

// WORD = unsigned short = 2 bytes = 16 bits
//					char = 1 byte  = 8 bits

// stride = ( ( ( ( biWidth * biBitCount ) + 31 ) & ~31 ) >> 3 )  &~31 replaces modulus

// Right shifts preserve the sign bit. When a signed integer shifts right, 
// the most-significant bit remains set. When an unsigned integer shifts right, the most-significant bit is cleared.
// Since the conversions performed by the shift operators do not provide for overflow or underflow conditions,
// information may be lost if the result of a shift operation cannot be represented in the type of the first operand after conversion.

// unsigned int row_size = floor( ( ( bpp * image_width ) + 31 ) / 32 ) * 4 );

// 16 bpp and 32 bpp images are always stored uncompressed.

// bfOffBits;
// Each pixel is represented by four bytes RGBA