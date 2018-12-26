#include "Texture.h"

using std::wstring;
using std::make_unique;
using std::unique_ptr;
using std::vector;

using DirectX::XMFLOAT4;
using Microsoft::WRL::ComPtr;

Texture::Texture(){}

Texture::Texture( const uint in_width , const uint in_height , const XMFLOAT4 in_colour )
	: _width( in_width ) , _height( in_height )
{

#if defined(_DEBUG) || defined(DEBUG)
	//texture_2d->SetPrivateData( WKPDID_D3DDebugObjectName , sizeof( "Texture 2d" ) - 1 , "Texture 2d" );
	//D3D_SET_OBJECT_NAME_A( texture_2d , "Texture 2d" );
#endif	

	pixels.clear();

	pixels.resize( _width * _height , in_colour );

	create_buffer(); // m_pixels );
}

Texture::Texture( const wstring in_filename )
{
	image = make_unique< WICImage >( in_filename );

	_width  = image->width();
	_height = image->height();

	//pixel_format = image->format();

	//create_buffer( image->pixels(), image->format() );
	create_buffer( image->pixels() , DXGI_FORMAT_R8G8B8A8_UNORM );
}

void Texture::create_buffer( const void * in_pixels , DXGI_FORMAT pixel_format )
{
	description_2d.Width				= _width;
	description_2d.Height				= _height;
	description_2d.MipLevels			= 1;
	description_2d.ArraySize			= 1;
	description_2d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// format.dxgi_format;// DXGI_FORMAT_R32G32B32A32_FLOAT;// DXGI_FORMAT_R8G8B8A8_UINT;// DXGI_FORMAT_R8G8B8A8_UNORM; // switch on image bpp
	description_2d.SampleDesc.Count		= 1;
	description_2d.SampleDesc.Quality	= 0;
	description_2d.Usage				= write_access; // D3D11_USAGE_IMMUTABLE, _DYNAMIC, _STAGING
	description_2d.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	description_2d.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	description_2d.MiscFlags			= 0;
	
	subresource_data.pSysMem					= in_pixels;
	subresource_data.SysMemPitch = _width * 4;// format.bytes_per_pixel;//* 16; //bytespp - 32bits = 4bytes ; 4 * components=16
	subresource_data.SysMemSlicePitch			= _height * ( _width * 4 );

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
												  & texture_2d );// .ReleaseAndGetAddressOf() );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateTexture2D" );

	view_description.Format						= DXGI_FORMAT_R32G32B32A32_FLOAT;// DXGI_FORMAT_R8G8B8A8_UINT;// 
	view_description.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;  // ARRAY
	view_description.Texture2D.MostDetailedMip	= 0u;	// number of mips - 1;
	view_description.Texture2D.MipLevels		= 1u;
	
	result = Drawable::get_video_device()->CreateShaderResourceView( texture_2d.Get() ,
																	 & view_description ,
																	 view_shader_resource.ReleaseAndGetAddressOf() );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateShaderResourceView" );

	PSSetShaderResources();
}

void Texture::create_buffer()
{
	// render target texture
	description_2d.Width				= _width;
	description_2d.Height				= _height;
	description_2d.MipLevels			= 1;
	description_2d.ArraySize			= 1;
	description_2d.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;// DXGI_FORMAT_R32G32B32A32_FLOAT; // passing in an array of floats
	description_2d.SampleDesc.Count		= 1;
	description_2d.SampleDesc.Quality	= 0;
	description_2d.Usage				= write_access;
	description_2d.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	// if( usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC )
	description_2d.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	description_2d.MiscFlags			= 0;

	// 2D : You set SysMemPitch to the distance between any two adjacent pixels on different lines. 
	//		You set SysMemSlicePitch to the size of the entire 2D surface in bytes. 
	//		To access a specific pixel, you use: (const char*)pSysMem + SysMemPitch*y + (x * BytesPerPixel)

	//unsigned int array_rgba[ 100 * 100 * 4 ] = { 255u , 0u , 0u , 0u };
	//memset( array_rgba , 255 , 100 * 100 * 4 );  // 255+255+255+255

	subresource_data.pSysMem				= pixels.data();
	subresource_data.SysMemPitch			= _width * sizeof( XMFLOAT4 ); // m_bytespp
	subresource_data.SysMemSlicePitch		= ( _width * sizeof( XMFLOAT4 ) ) * _height;

	result = Drawable::get_video_device()->CreateTexture2D( & description_2d ,
															& subresource_data ,	// initial data
															texture_2d.ReleaseAndGetAddressOf() );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateTexture2D" );

	view_description.Format						= DXGI_FORMAT_R32G32B32A32_FLOAT;// m_format;
	view_description.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	view_description.Texture2D.MostDetailedMip	= 0u;	// number of mips - 1;
	view_description.Texture2D.MipLevels		= 1u;

	//
	result = Drawable::get_video_device()->CreateShaderResourceView( texture_2d.Get() ,
																	 &view_description ,
																	 view_shader_resource.ReleaseAndGetAddressOf() );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateShaderResourceView" );

	PSSetShaderResources();
}

void Texture::plot( const uint in_x , const uint in_y , const XMFLOAT4 in_colour )
{
	if( in_x >= 0 && in_y >= 0 && in_x <= _width && in_y <= _height )
	{
		int index = ( in_y * _width ) + in_x;

		pixels.at( index ) = in_colour;
	}
}

void Texture::line( const XMFLOAT4 in_points , const XMFLOAT4 in_colour )
{
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

void Texture::PSSetShaderResources()
{
	ComPtr< ID3D11DeviceContext > p_video_device_context;
	get_video_device()->GetImmediateContext( &p_video_device_context );

	// void set_diffuse_map( ComPtr< ID3D11Texture2D >& diffuse_map );
	p_video_device_context->PSSetShaderResources( 0 ,					// PS resource slot
												  1 ,					// count of resources
												  view_shader_resource.GetAddressOf() );	// shader resource view
}

void Texture::update( const double time_delta )
{
	PSSetShaderResources();
}

void Texture::update_PS_buffer()//_PS_buffer()
{
	ComPtr< ID3D11DeviceContext > p_video_device_context;
	get_video_device()->GetImmediateContext( &p_video_device_context );

	p_video_device_context->Map( texture_2d.Get() , 0 , D3D11_MAP_WRITE_DISCARD , 0 , & subresource_mapped );
	//D3D11_MAP_FLAG_DO_NOT_WAIT 

	unsigned int pitch_row		= subresource_mapped.RowPitch;
	unsigned int pitch_height	= subresource_mapped.DepthPitch;

	unsigned char * ptr_dest = reinterpret_cast< unsigned char * >( subresource_mapped.pData );
	//Colour * ptr_dest = reinterpret_cast< Colour * >( m_mapped_subresource.pData );
	// ? memcpy( ptr_dest + ( row * pitch_row ) , m_rgba.data() + ( row * m_width ) , m_width );

	// for each row
	for( unsigned int row = 0; row < _height; ++row )
	{							
		memcpy( ptr_dest + ( row * pitch_row ) , pixels.data() + ( row * static_cast<int>(_width) ) , static_cast<int>(_width) * 4 ); // bytespp
	}

	p_video_device_context->Unmap( texture_2d.Get() , 0 );
}