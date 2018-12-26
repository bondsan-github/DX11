#pragma once

//#include "..\DX11\debugging.h"
#include "types.h"
#include "Drawable.h"
#include "WICImage.h"

#include <d3d11.h>
#include <DirectXMath.h>// XM types

#include <vector>
//#include <Windows.h>
#include <wrl/client.h> // ComPtr
#include <wincodec.h>	// WICImage
#include <memory>		// unique_ptr

using std::vector;
using DirectX::XMFLOAT4;
using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::wstring;

//using namespace DirectX;			// ONLY in CPP files <- ? include in header will pollute global namespace
//using namespace Microsoft::WRL;


// user defined literal
//unsigned char operator "" _uc( char in_uc ) { return static_cast< unsigned char >( in_uc ); }

// material
//enum class texture_type { diffuse , alpha , specular , displacement };

/*
class Colour_rgba128bpp_float
{
	public:
		Colour_rgba128bpp_float() {}		

		Colour_rgba128bpp_float( const float in_red,  // ? change to unsigned float (PS input) + normalise ?
								 const float in_green,
								 const float in_blue,
								 const float in_alpha ) 
			: m_red( in_red ) , m_green( in_green ) , m_blue( in_blue ) , m_alpha( in_alpha ) { }

		//operator *
		// return int = m_red << 24 && 
		//get ? ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
		//unsigned int as_int() {	//int pixel = ( m_red << 24u ) | ( m_green << 16u ) | ( m_blue << 8u ) | m_alpha ) ; }	
		
	private:
		float m_red { };	// 4 byte
		float m_green { };	// 4 byte
		float m_blue { };	// 4 byte
		float m_alpha { };	// 4 byte
};
*/
//static Colour_8bit_rgba white( 1.0f , 1.0f , 1.0f , 1.0f ); #define white

//void line( std::shared_pointer<Texture> in_texture, XMFLOAT4 in_points , colour in_colour );

//template< typename pixel_format >
class Texture : public Drawable
{
	public:
		Texture();

		Texture( const uint in_width, const uint in_height , const XMFLOAT4 in_colour );
		//Texture( const XMFLOAT2 in_dimensions , const XMFLOAT4 in_rgba );

		Texture( const wstring in_filename );

		//void clear( const Colour in_colour ) {}

		void plot( const uint in_x , const uint in_y , const  XMFLOAT4 in_colour );

		void line( const  XMFLOAT4 in_points , const  XMFLOAT4 in_colour );

		// elipse( center_x, center_y, width, height, [pen] ) 

		//const void * ptr_rgba() const { return m_rgba.data(); }
		
		//void load( const wstring filename ) { m_wic_image.load();	}

		uint width( void ) const	{ return _width; }
		uint height( void ) const	{ return _height; }

		void width( const uint in_width )	{ _width = in_width; }
		void height( const uint in_height )	{ _height = in_height; }

		void update( const double time_delta );

	private:

		HRESULT result { E_FAIL };
		
		void create_buffer();
		void create_buffer( const void * ptr_memory , DXGI_FORMAT pixel_format );// pixel_format format );
		void PSSetShaderResources();
				
		void update_PS_buffer();

		//type use { diffuse };
		D3D11_USAGE write_access	{ D3D11_USAGE_DYNAMIC }; // _DEFAULT, _IMMUTABLE, _STAGING; 
		//DXGI_FORMAT layout	{ DXGI_FORMAT_R8G8B8A8_UNORM }; 

		uint _width {};
		uint _height {};
		uchar bytespp {};

		//unique_ptr <
		vector< XMFLOAT4 > pixels {}; //_128bpp // two input format types :( // _int8

		//XMFLOAT4 m_colour { };	
					
		D3D11_TEXTURE2D_DESC				description_2d {};
		D3D11_SUBRESOURCE_DATA				subresource_data {};
		
		D3D11_SHADER_RESOURCE_VIEW_DESC		view_description {};
		ComPtr< ID3D11ShaderResourceView >  view_shader_resource {};

		//ComPtr< ID3D11RenderTargetView >	render_target_view;
		ComPtr< ID3D11Texture2D >			texture_2d {};

		unique_ptr< WICImage >				image {};

		D3D11_MAPPED_SUBRESOURCE			subresource_mapped {};
};

//Pixel( x,y, const & Pixel ) { }

/*
class Bitmap : public Image
{
	public:

		Bitmap() {};
		Bitmap( const wstring in_filename ) 
		{ 
			load( in_filename ); 
		}
		//~Bitmap( );			
		void load( const wstring in_filename );
	
	private:

		BITMAPFILEHEADER		m_file_header {};
		BITMAPV5HEADER			m_bmp_header {};

		bool m_flipped { false };
};
*/

/*
// copy assignment operator
WICImage operator = ( const WICImage & in_copy_from )
{
m_p_vector_image_data = std::move( in_copy_from.m_p_vector_image_data );
//						  ^
// unique_ptr< vector<uchar> > operator = ( const unique_ptr< vector<uchar> > & in_pointer ) = default_delete
}

// copy constructor
WICImage( const WICImage & in_copy_from )
{
m_p_vector_image_data = std::move( in_copy_from.m_p_vector_image_data );
}
*/

/*
class Textures
{
	public:
	//Textures();

	void load( const texture_type in_type , const wstring filename )
	{
		switch( in_type )
		{
			case texture_type::diffuse:
				break;
		}
	}

	void load( const texture_type in_type , const vector<Colour>& in_pixels )
	{
	}

	line()

	private:
	//texture_diffuse m_diffuse;
	//Texture alpha;
	//Texture m_specular;
	//Texture m_reflection;
	//Texture m_displacement;

};
*/

/*
class texture_diffuse : public Texture
{
public:
virtual void load( const wstring filename )
{
}
};
*/

/*unsigned char pixels()
{
unsigned char * pixel_array = new unsigned char[ m_width * m_height * 4 ];

for( int i = 0; i < m_width * m_height * 4; ++i )
{
pixel_array[ i ] = m_rgba[ i ].m_red;
}
}*/