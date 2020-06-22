#include "texture.h"

using std::wstring;
using std::make_unique;
using std::unique_ptr;
using std::vector;

using DirectX::XMFLOAT4;
using Microsoft::WRL::ComPtr;

Texture::Texture() 
{
	video_device->GetImmediateContext( &device_context_video );

	bytes_per_pixel = 4;// R8G8B8A8 32bit
}
//Texture::Texture( const uint in_width , const uint in_height , const XMFLOAT4 in_colour ) : _width( in_width ) , _height( in_height ) {}
//Texture::Texture( const wstring in_filename ) {}

void Texture::create_blank( const uint in_width , const uint in_height , const XMUINT4 in_colour) //const DirectX::XMUINT4 in_colour )
{
	width				= in_width;
	height				= in_height;
	//bytes_per_pixel		= sizeof( XMFLOAT4 );
	colour				= in_colour;

	//uchar sizeoffloat = sizeof( float );

#if defined(_DEBUG) || defined(DEBUG)
	//texture_2d->SetPrivateData( WKPDID_D3DDebugObjectName , sizeof( "Texture 2d" ) - 1 , "Texture 2d" );
	//D3D_SET_OBJECT_NAME_A( texture_2d , "Texture 2d" );
#endif	

	//pixels.clear();
	//pixels.resize( width * height * bytes_per_pixel , 0 );

	pixels = new unsigned char[ width * height * bytes_per_pixel ];

	int row = 0, column = 0;

	for( int y = 0; y < height; y++ )
	{
		row = y * width * bytes_per_pixel;
		for( int x = 0; x < width; x++ )
		{
			column = x * 4;
			pixels[ row + column + 0 ] = in_colour.x;
			pixels[ row + column + 1 ] = in_colour.y;
			pixels[ row + column + 2 ] = in_colour.z;
			pixels[ row + column + 3 ] = in_colour.w;
		}
	}

	create_buffer( width , height , colour );
}

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
	subresource_data.SysMemPitch		= width * bytes_per_pixel;// format.bytes_per_pixel;
	subresource_data.SysMemSlicePitch	= height * ( width * bytes_per_pixel );

	/*
	rowPitch = ( width * bitspp + 7 ) / 8;

	( 100 * 32 + 7 ) / 8 400.875
	( 200 * 32 + 7 ) / 8 800.875
	( 600 * 32 + 7 ) / 8 2400.875
	( 800 * 32 + 7 ) / 8 3200.875
	*/

	result = video_device->CreateTexture2D( & description_2d ,
											& subresource_data ,	// initial data
											& texture_2d );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateTexture2D" );

	view_description.Format						= dxgi_format;// _UINT;// 
	view_description.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;  // ARRAY
	view_description.Texture2D.MostDetailedMip	= 0u;	// number of mips - 1;
	view_description.Texture2D.MipLevels		= 1u;
	
	result = video_device->CreateShaderResourceView( texture_2d.Get() ,
													 & view_description ,
													 & view_shader_resource );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateShaderResourceView" );
}

void Texture::create_buffer( const uint in_width , const uint in_height , const XMUINT4 in_colour)
{
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
		
	subresource_data.pSysMem			= pixels; //array_rgba;
	subresource_data.SysMemPitch		= width * bytes_per_pixel;
	subresource_data.SysMemSlicePitch	= height * ( width * bytes_per_pixel );

	result = video_device->CreateTexture2D( & description_2d ,
											& subresource_data , // initial data
											& texture_2d );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateTexture2D" );

	view_description.Format						= dxgi_format;
	view_description.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	view_description.Texture2D.MostDetailedMip	= 0u;	// number of mips - 1;
	view_description.Texture2D.MipLevels		= 1u;

	//
	result = video_device->CreateShaderResourceView( texture_2d.Get() ,
													 & view_description ,
													 & view_shader_resource );

	if( FAILED( result ) ) ErrorExit( L"Texture::create_buffer() error; CreateShaderResourceView" );
}

void Texture::plot( const uint in_x , const uint in_y , const XMUINT4 in_colour)
{
	// 2D : You set SysMemPitch to the distance between any two adjacent pixels on different lines. 
	//		You set SysMemSlicePitch to the size of the entire 2D surface in bytes. 
	//		To access a specific pixel, you use: (const char*)pSysMem + SysMemPitch * y + (x * BytesPerPixel)
	
	// texture_2D get sytem memory pitch

	if( in_x >= 0 && in_y >= 0 && in_x <= width && in_y <= height )
	{
		int index = ( in_y * width * 4 ) + in_x * 4;

		pixels[ index + 0 ] = in_colour.x;
		pixels[ index + 1 ] = in_colour.y;
		pixels[ index + 2 ] = in_colour.z;
		pixels[ index + 3 ] = in_colour.w;
	}

	
}

void Texture::line( const XMFLOAT4 in_points , const XMUINT4 in_colour )//const DirectX::XMUINT4 in_colour )
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
	//ComPtr< ID3D11DeviceContext > video_device_context;
	//get_video_device()->GetImmediateContext( & video_device_context );

	// void set_diffuse_map( diffuse_map );
	device_context_video->PSSetShaderResources( 0 ,					// PS resource slot
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
	//ComPtr< ID3D11DeviceContext > video_device_context;
	//get_video_device()->GetImmediateContext( & video_device_context );

	device_context_video->Map( texture_2d.Get() ,
							   0 ,							// sub resource
							   D3D11_MAP_WRITE_DISCARD ,	// mapping type
							   0 ,							// D3D11_MAP_FLAG_DO_NOT_WAIT
							   & subresource_mapped );
	

	unsigned int pitch_row		= subresource_mapped.RowPitch;
	unsigned int pitch_height	= subresource_mapped.DepthPitch;

	unsigned char * ptr_dest = reinterpret_cast< unsigned char * >( subresource_mapped.pData );
	//Colour * ptr_dest = reinterpret_cast< Colour * >( m_mapped_subresource.pData );

	unsigned int row_byte_size = width * bytes_per_pixel;

	// for each row
	for( unsigned int row = 0; row < height; row++ )
	{							
		memcpy( ptr_dest + ( row * pitch_row ) , pixels + ( row * static_cast< int >( width ) * 4) , row_byte_size );
		//memcpy( ptr_dest + ( row * pitch_row ) , pixels + ( row * pitch_row ) , row_byte_size );
	}

	device_context_video->Unmap( texture_2d.Get() , 0 );
}

//for( UINT y = 0; y < bitmapHeight; y++ )
//{
//	UINT8 *pScan = m_pDataBegin + placedTexture2D.Offset + y * pitchedDesc.RowPitch;
//	memcpy( pScan , &( pBitmap->pixels[ y * bitmapWidth ] ) , sizeof( DWORD ) * bitmapWidth );
//}