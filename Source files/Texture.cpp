#include "Texture.h"

using std::wstring;
using std::make_unique;
using std::unique_ptr;
using std::vector;

using DirectX::XMFLOAT4;
using Microsoft::WRL::ComPtr;

Texture::Texture() {}

//Texture::Texture( const uint in_width , const uint in_height , const XMFLOAT4 in_colour ) : _width( in_width ) , _height( in_height ) {}
//Texture::Texture( const wstring in_filename ) {}

void Texture::create_buffer( const void * in_pixels , DXGI_FORMAT pixel_format )
{
	description_2d.Width				= width;
	description_2d.Height				= height;
	description_2d.MipLevels			= 1;
	description_2d.ArraySize			= 1;
	description_2d.Format				= dxgi_format;// _UNORM; // switch on image bpp
	description_2d.SampleDesc.Count		= 1;
	description_2d.SampleDesc.Quality	= 0;
	description_2d.Usage				= usage; // D3D11_USAGE_IMMUTABLE, _DYNAMIC, _STAGING
	description_2d.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	description_2d.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	description_2d.MiscFlags			= 0;
	
	subresource_data.pSysMem			= in_pixels;
	subresource_data.SysMemPitch = width * bytes_per_pixel;// format.bytes_per_pixel;//* 16; //bytespp - 32bits = 4bytes ; 4 * components=16
	subresource_data.SysMemSlicePitch = height * ( width * bytes_per_pixel );

	/*
	rowPitch = ( width * bitspp + 7 ) / 8;

	( 100 * 32 + 7 ) / 8
	( 200 * 32 + 7 ) / 8
	( 800 * 32 + 7 ) / 8
	( 600 * 32 + 7 ) / 8

	10 * 10 + 7
	10 + 7 * 10

	24+7
	7/8
	*/

	result = get_video_device()->CreateTexture2D( & description_2d ,
												  & subresource_data ,	// initial data
												  & texture_2d );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateTexture2D" );

	view_description.Format						= dxgi_format;// _UINT;// 
	view_description.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;  // ARRAY
	view_description.Texture2D.MostDetailedMip	= 0u;	// number of mips - 1;
	view_description.Texture2D.MipLevels		= 1u;
	
	result = Drawable::get_video_device()->CreateShaderResourceView( texture_2d.Get() ,
																	 & view_description ,
																	 & view_shader_resource );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateShaderResourceView" );
}

void Texture::create_buffer( const uint in_width , const uint in_height )//, const XMFLOAT4 in_colour )
{
	//bytes_per_pixel = sizeof( XMFLOAT4 );
	//uchar sizeoffloat = sizeof( float );

	// render target texture
	description_2d.Width				= width;
	description_2d.Height				= height;
	description_2d.MipLevels			= 1;
	description_2d.ArraySize			= 1;
	description_2d.Format				= dxgi_format;
	description_2d.SampleDesc.Count		= 1;
	description_2d.SampleDesc.Quality	= 0;
	description_2d.Usage				= usage;
	description_2d.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	// if( usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC )
	description_2d.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	description_2d.MiscFlags			= 0;

	// 2D : You set SysMemPitch to the distance between any two adjacent pixels on different lines. 
	//		You set SysMemSlicePitch to the size of the entire 2D surface in bytes. 
	//		To access a specific pixel, you use: (const char*)pSysMem + SysMemPitch*y + (x * BytesPerPixel)

	//unsigned char array_rgba[ 100 * 100 * 4 ];// = { 255u , 0u , 0u , 0u };
	//memset( array_rgba , 255 , 100 * 100 * 4 );  // 255+255+255+255

	subresource_data.pSysMem = pixels.data(); //array_rgba;
	subresource_data.SysMemPitch = width * bytes_per_pixel;// sizeof( XMFLOAT4 ); //bytes_per_pixel; //sizeof( uint ); 
	subresource_data.SysMemSlicePitch = height * ( width * bytes_per_pixel );// sizeof( XMFLOAT4 ) ) *; //bytes_per_pixel );

	result = Drawable::get_video_device()->CreateTexture2D( & description_2d ,
															& subresource_data , // initial data
															& texture_2d );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateTexture2D" );

	view_description.Format						= dxgi_format;
	view_description.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	view_description.Texture2D.MostDetailedMip	= 0u;	// number of mips - 1;
	view_description.Texture2D.MipLevels		= 1u;

	//
	result = Drawable::get_video_device()->CreateShaderResourceView( texture_2d.Get() ,
																	 & view_description ,
																	 & view_shader_resource );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateShaderResourceView" );
}

void Texture::plot( const uint in_x , const uint in_y , const XMFLOAT4 in_colour )
{
	if( in_x >= 0 && in_y >= 0 && in_x <= width && in_y <= height )
	{
		int index = ( in_y * width ) + in_x;

		pixels.at( index ) = in_colour;
	}

	update_PS_buffer();
}

void Texture::line( const XMFLOAT4 in_points , const XMFLOAT4 in_colour )
{
	//assert( in_points within texture size )

	int x_1 = in_points.x;
	int y_1 = in_points.y;
	int x_2 = in_points.z;
	int y_2 = in_points.w;

	// Bresenham's line algorithm
	const bool steep = ( fabs( y_2 - y_1 ) > fabs( x_2 - x_1 ) );
	if( steep )
	{
		std::swap( x_1 , y_1 );
		std::swap( x_2 , y_2 );
	}

	if( x_1 > x_2 )
	{
		std::swap( x_1 , x_2 );
		std::swap( y_1 , y_2 );
	}

	const float dx = x_2 - x_1;
	const float dy = fabs( y_2 - y_1 );

	float error = dx / 2.0f;
	const int y_step = ( y_1 < y_2 ) ? 1 : -1;
	int y = y_1;

	const int max_x = x_2;

	for( int x = x_1; x < max_x; x++ )
	{
		if( steep )
		{
			plot( y , x , in_colour );
		} else
		{
			plot( x , y , in_colour );
		}

		error -= dy;

		if( error < 0 )
		{
			y += y_step;
			error += dx;
		}
	}

	update_PS_buffer();
}

void Texture::set_PS_shader_resources()
{
	ComPtr< ID3D11DeviceContext > video_device_context;
	get_video_device()->GetImmediateContext( & video_device_context );

	// void set_diffuse_map( diffuse_map );
	video_device_context->PSSetShaderResources( 0 ,					// PS resource slot
												1 ,					// count of resources
												view_shader_resource.GetAddressOf() );	// shader resource view
}

void Texture::update( const double time_delta )
{
	//update_PS_buffer();
}

void Texture::render()
{
	set_PS_shader_resources();
}

void Texture::update_PS_buffer()
{
	ComPtr< ID3D11DeviceContext > video_device_context;
	get_video_device()->GetImmediateContext( & video_device_context );

	video_device_context->Map( texture_2d.Get() , 0 , D3D11_MAP_WRITE_DISCARD , 0 , & subresource_mapped );
	//D3D11_MAP_FLAG_DO_NOT_WAIT 

	unsigned int pitch_row		= subresource_mapped.RowPitch;
	unsigned int pitch_height	= subresource_mapped.DepthPitch;

	unsigned char * ptr_dest = reinterpret_cast< unsigned char * >( subresource_mapped.pData );
	//Colour * ptr_dest = reinterpret_cast< Colour * >( m_mapped_subresource.pData );
	// ? memcpy( ptr_dest + ( row * pitch_row ) , m_rgba.data() + ( row * m_width ) , m_width );

	// for each row
	for( unsigned int row = 0; row < height; ++row )
	{							
		memcpy( ptr_dest + ( row * pitch_row ) , pixels.data() + ( row * static_cast< int >( width ) ) , static_cast< int >( width ) * bytes_per_pixel ); // bytespp
	}

	video_device_context->Unmap( texture_2d.Get() , 0 );
}