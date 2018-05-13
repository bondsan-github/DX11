#pragma once

#include <vector>
#include <Windows.h>
#include <wrl/client.h>
#include <wincodec.h>
#include <memory>

#include <d3d11.h>
#include <DirectXMath.h>

#include "..\DX11\debugging.h"
#include "..\DX11\types.h"
//#include "..\DX11\File.h"
#include "..\DX11\Drawable.h"
#include "WICImage.h"

//using namespace DirectX;			// ONLY in CPP files <- including this header will pollute global namespace
using namespace Microsoft::WRL;
using std::wstring;
using std::make_unique;
using std::unique_ptr;
using std::vector;

// user defined literal
//unsigned char operator "" _uc( char in_uc ) { return static_cast< unsigned char >( in_uc ); }

// material
//enum class texture_type { diffuse , alpha , specular , displacement };


class Colour_rgba128bpp_float
{
	public:
		Colour_rgba128bpp_float() {}
		

		Colour_rgba128bpp_float( const float in_red,  // ? change to unsigned float (PS input) + normalise ?
								 const float in_green,
								 const float in_blue,
								 const float in_alpha) 
			: m_red( in_red ) , m_green(in_green), m_blue(in_blue), m_alpha(in_alpha)
		{		}

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

//static Colour_8bit_rgba white( 1.0f , 1.0f , 1.0f , 1.0f ); #define white

//void line( std::shared_pointer<Texture> in_texture, XMFLOAT4 in_points , colour in_colour );

class Texture : public Drawable
{
	public:
		Texture();

		Texture( const XMFLOAT2 in_dimensions , const XMFLOAT4 in_colour );
		//Texture( const XMFLOAT2 in_dimensions , const XMFLOAT4 in_rgba );

		Texture( const wstring in_filename );

		//void clear( const Colour in_colour ) {}

		void plot( const float in_x , const float in_y , const XMFLOAT4 in_colour );

		void line( const XMFLOAT4 in_points , const XMFLOAT4 in_colour );

		// elipse(center_x, center_y, width, height) 

		//const void * ptr_rgba() const { return m_rgba.data(); }
		
		//void load( const wstring filename ) { m_wic_image.load();	}

		float width( void ) const	{ return m_width; }
		float height( void ) const	{ return m_height; }

		void width( const float in_width )		{ m_width = in_width; }
		void height( const float in_height )	{ m_height = in_height; }

		void update();

	private:

		HRESULT m_result { E_FAIL };
		
		void create_buffer();
		void create_buffer( const void * ptr_memory );
		void PSSetShaderResources();

		/*
		#if defined(_DEBUG) || defined(DEBUG)
		m_p_texture2D_diffuse->SetPrivateData( WKPDID_D3DDebugObjectName , sizeof( "Texture difuse map" ) - 1 , "Texture difuse map" );
		#endif	*/
		
		void update_PS_buffer();

		//texture_type m_type { texture_type::diffuse };
		D3D11_USAGE m_usage { D3D11_USAGE::D3D11_USAGE_DYNAMIC }; // D3D11_USAGE_IMMUTABLE; 
		DXGI_FORMAT m_format { DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT }; //{ DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT };//

		float m_width { };
		float m_height { };

		//unique_ptr <
		vector< XMFLOAT4 > m_pixels;
		// _int8
		//XMFLOAT4 m_colour { };

		//unsigned char m_bpp = 8;
		//DXGI_FORMAT a8r8g8b8 m_format;
			
		D3D11_TEXTURE2D_DESC				m_texture_2d_description { };
		D3D11_SUBRESOURCE_DATA				m_subresource_data { };
		
		D3D11_SHADER_RESOURCE_VIEW_DESC		m_view_description { };
		ComPtr< ID3D11ShaderResourceView >  m_shader_resource_view;

		//ComPtr< ID3D11RenderTargetView >	mp_render_target_view;
		ComPtr< ID3D11Texture2D >			m_texture_2D;

		unique_ptr< WICImage >				m_image;

		D3D11_MAPPED_SUBRESOURCE			m_mapped_subresource { };
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