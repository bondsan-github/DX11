#pragma once

//#include "..\DX11\debugging.h"
#include "types.h"
#include "Graphics_component.h"
#include "WICImage.h"

#include <d3d11.h>
#include <DirectXMath.h>// XM types

#include <vector>
//#include <Windows.h>
#include <wrl/client.h> // ComPtr
#include <wincodec.h>	// WICImage
#include <memory>		// unique_ptr

using std::vector;  // pollutes global namespace
using DirectX::XMFLOAT4;
using DirectX::XMUINT4;
using Microsoft::WRL::ComPtr;
using std::unique_ptr;
using std::wstring;


//using namespace DirectX;			// ONLY in CPP files <- include in header will pollute global namespace
//using namespace Microsoft::WRL;

// material
//enum class texture_type { diffuse , alpha , specular , displacement };

//void line( std::shared_pointer<Texture> in_texture, XMFLOAT4 in_points , colour in_colour );

//template< typename pixel_format >
class Texture : public Graphics_component
{
	public:

		Texture();

		Texture( vector< XMUINT4 > in_data, const uint in_width, const uint in_height ); // , const uint bits_per_pixel );
		//Texture( const uint in_width, const uint in_height , const XMFLOAT4 in_colour );
		//Texture( const XMFLOAT2 in_dimensions , const XMFLOAT4 in_rgba );
		//Texture( const wstring in_filename );

		void create_blank( const uint in_width , const uint in_height , const XMUINT4 in_colour );// const DirectX::XMUINT4 in_colour );

		void load( const wstring in_filename )
		{
			image.load( in_filename );

			width = image.get_width();
			height = image.get_height();
			//pixel_format = image.format();

			create_buffer( image.get_pixels() , dxgi_format );  //image.format()
		}

		//void clear( const Colour in_colour ) {}

		void plot( const uint in_x , const uint in_y , const XMUINT4 in_colour );// const DirectX::XMUINT4 in_colour );

		void line( const  XMFLOAT4 in_points , const XMUINT4 in_colour );// const DirectX::XMUINT4 in_colour );

		// elipse( center_x, center_y, width, height, [pen] ) 
		//const void * ptr_rgba() const { return m_rgba.data(); }		
		//void load( const wstring filename ) { m_wic_image.load();	}

		uint get_width( void ) const	{ return width; } // ** get from view_description
		uint get_height( void ) const	{ return height; }

		void set_width( const uint in_width )	{ width = in_width; }
		void set_height( const uint in_height )	{ height = in_height; }

		void update( const double time_delta );
		void update_PS_buffer();

		void render();

	private:
	   
		void create_buffer( const uint in_width , const uint in_height , const XMUINT4 in_colour ); //const DirectX::XMUINT4 in_colour );
		void create_buffer( const void * ptr_memory , DXGI_FORMAT pixel_format );

		void set_PS_shader_resources();

	private:

		HRESULT result{ E_FAIL };

		ComPtr< ID3D11DeviceContext >	device_context_video;

		//type use { diffuse };
		D3D11_USAGE	usage		{ D3D11_USAGE_DYNAMIC }; // _DEFAULT, _IMMUTABLE, _STAGING; 
		DXGI_FORMAT	dxgi_format	{ DXGI_FORMAT_R8G8B8A8_UNORM }; //layout

		uint		width	{};
		uint		height	{};

		uchar		bytes_per_pixel { 4 }; // struct pixel_formats { DXGI_FORMAT ... , WIC , uchar bytespp };

		// 1 float = 1 pixel component e.g. at(0) = R , at(1) = G

		//vector< unsigned char > pixels;
		XMUINT4								colour{};
		//vector< unsigned char >			pixels;
		unsigned char *						pixels;

		D3D11_TEXTURE2D_DESC				description_2d {};
		D3D11_SUBRESOURCE_DATA				subresource_data {};

		D3D11_SHADER_RESOURCE_VIEW_DESC		view_description {};
		ComPtr< ID3D11ShaderResourceView >  view_shader_resource {};

		//ComPtr< ID3D11RenderTargetView >	render_target_view;
		ComPtr< ID3D11Texture2D >			texture_2d {};

		WICImage							image;

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