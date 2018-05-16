#include "..\Header files\Texture.h"

using std::wstring;
using std::make_unique;
using std::unique_ptr;
using std::vector;

using DirectX::XMFLOAT4;
using Microsoft::WRL::ComPtr;

Texture::Texture(){}

Texture::Texture( const uint in_width , const uint in_height , const XMFLOAT4 in_colour )
	: m_width( in_width ) , m_height( in_height )
{
	m_pixels.clear();

	m_pixels.resize( m_width * m_height , in_colour );

	create_buffer(); // m_pixels );
}

Texture::Texture( const wstring in_filename )
{
	m_image = make_unique< WICImage >( in_filename );

	m_width  = m_image->width();
	m_height = m_image->height();

	//m_format = m_image->format();

	create_buffer( m_image->pixels() );
}

void Texture::create_buffer( const void * in_pixels ) // bool use_image_dimensions
{
	m_texture_2d_description.Width              = m_width;
	m_texture_2d_description.Height             = m_height;
	m_texture_2d_description.MipLevels          = 1;
	m_texture_2d_description.ArraySize          = 1;
	m_texture_2d_description.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// DXGI_FORMAT_R8G8B8A8_UINT;// DXGI_FORMAT_R8G8B8A8_UNORM; // switch on image bpp
	m_texture_2d_description.SampleDesc.Count   = 1;
	m_texture_2d_description.SampleDesc.Quality = 0;
	m_texture_2d_description.Usage              = m_usage;
	m_texture_2d_description.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	m_texture_2d_description.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;
	m_texture_2d_description.MiscFlags          = 0;

	m_subresource_data.pSysMem					= in_pixels;
	m_subresource_data.SysMemPitch				= m_width * 16; //m_image->bytepp()
	m_subresource_data.SysMemSlicePitch			= ( m_width * 4 ) * m_height;

	m_result = get_video_device()->CreateTexture2D( & m_texture_2d_description ,
													& m_subresource_data ,	// initial data
													m_texture_2D.ReleaseAndGetAddressOf() );

	if( FAILED( m_result ) ) ErrorExit( L"Texture::create_buffer() error; CreateTexture2D" );

	m_view_description.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// DXGI_FORMAT_R8G8B8A8_UINT;// 
	m_view_description.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	m_view_description.Texture2D.MostDetailedMip = 0u;	// number of mips - 1;
	m_view_description.Texture2D.MipLevels       = 1u;

	m_result = Drawable::get_video_device()->CreateShaderResourceView( m_texture_2D.Get() ,
																	   & m_view_description ,
																	   m_shader_resource_view.ReleaseAndGetAddressOf() );

	if( FAILED( m_result ) ) ErrorExit( L"Texture::create_buffer() error; CreateShaderResourceView" );

	PSSetShaderResources();
}

void Texture::create_buffer()
{
	// render target texture
	m_texture_2d_description.Width				= m_width;
	m_texture_2d_description.Height				= m_height;
	m_texture_2d_description.MipLevels			= 1;
	m_texture_2d_description.ArraySize			= 1;
	m_texture_2d_description.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;// DXGI_FORMAT_R32G32B32A32_FLOAT; // passing in an array of floats
	m_texture_2d_description.SampleDesc.Count	= 1;
	m_texture_2d_description.SampleDesc.Quality	= 0;
	m_texture_2d_description.Usage				= m_usage;
	m_texture_2d_description.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	// if( m_usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC )
	m_texture_2d_description.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	m_texture_2d_description.MiscFlags			= 0;

	// 2D : You set SysMemPitch to the distance between any two adjacent pixels on different lines. 
	//		You set SysMemSlicePitch to the size of the entire 2D surface in bytes. 
	//		To access a specific pixel, you use: (const char*)pSysMem + SysMemPitch*y + (x * BytesPerPixel)

	//unsigned int array_rgba[ 100 * 100 * 4 ] = { 255u , 0u , 0u , 0u };
	//memset( array_rgba , 255 , 100 * 100 * 4 );  // 255+255+255+255

	m_subresource_data.pSysMem				= m_pixels.data();
	m_subresource_data.SysMemPitch			= m_width * sizeof( XMFLOAT4 ); // m_bpp
	m_subresource_data.SysMemSlicePitch		= ( m_width * sizeof( XMFLOAT4 ) ) * m_height;

	m_result = Drawable::get_video_device()->CreateTexture2D( & m_texture_2d_description ,
															  & m_subresource_data ,	// initial data
															  m_texture_2D.ReleaseAndGetAddressOf() );

	if( FAILED( m_result ) ) ErrorExit( L"Texture::create_buffer() error; CreateTexture2D" );

	m_view_description.Format						= DXGI_FORMAT_R32G32B32A32_FLOAT;// m_format;
	m_view_description.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	m_view_description.Texture2D.MostDetailedMip	= 0u;	// number of mips - 1;
	m_view_description.Texture2D.MipLevels			= 1u;

	//
	m_result = Drawable::get_video_device()->CreateShaderResourceView( m_texture_2D.Get() ,
																	   &m_view_description ,
																	   m_shader_resource_view.ReleaseAndGetAddressOf() );

	if( FAILED( m_result ) ) ErrorExit( L"Texture::create_buffer() error; CreateShaderResourceView" );

	PSSetShaderResources();
}

void Texture::plot( const uint in_x , const uint in_y , const XMFLOAT4 in_colour )
{
	if( in_x >= 0 && in_y >= 0 && in_x <= m_width && in_y <= m_height )
	{
		int index = ( in_y * m_width ) + in_x;

		m_pixels.at( index ) = in_colour;
	}
}

void Texture::line( const XMFLOAT4 in_points , const XMFLOAT4 in_colour )
{
	int x1 = in_points.x;
	int y1 = in_points.y;
	int x2 = in_points.z;
	int y2 = in_points.w;

	// Bresenham's line algorithm
	const bool steep = ( fabs( y2 - y1 ) > fabs( x2 - x1 ) );
	if( steep )
	{
		std::swap( x1 , y1 );
		std::swap( x2 , y2 );
	}

	if( x1 > x2 )
	{
		std::swap( x1 , x2 );
		std::swap( y1 , y2 );
	}

	const float dx = x2 - x1;
	const float dy = fabs( y2 - y1 );

	float error = dx / 2.0f;
	const int ystep = ( y1 < y2 ) ? 1 : -1;
	int y = y1;

	const int maxX = x2;

	for( int x = x1; x < maxX; x++ )
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
			y += ystep;
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
												  m_shader_resource_view.GetAddressOf() );	// shader resource view
}

void Texture::update()
{
	PSSetShaderResources();
}

void Texture::update_PS_buffer()//_PS_buffer()
{
	ComPtr< ID3D11DeviceContext > p_video_device_context;
	get_video_device()->GetImmediateContext( &p_video_device_context );

	p_video_device_context->Map( m_texture_2D.Get() , 0 , D3D11_MAP_WRITE_DISCARD , 0 , & m_mapped_subresource );
	//D3D11_MAP_FLAG_DO_NOT_WAIT 

	unsigned int pitch_row		= m_mapped_subresource.RowPitch;
	unsigned int pitch_height	= m_mapped_subresource.DepthPitch;

	unsigned char * ptr_dest = reinterpret_cast< unsigned char * >( m_mapped_subresource.pData );
	//Colour * ptr_dest = reinterpret_cast< Colour * >( m_mapped_subresource.pData );
	// ? memcpy( ptr_dest + ( row * pitch_row ) , m_rgba.data() + ( row * m_width ) , m_width );

	// for each row
	for( unsigned int row = 0; row < m_height; ++row )
	{							
		memcpy( ptr_dest + ( row * pitch_row ) , m_pixels.data() + ( row * static_cast<int>(m_width) ) , static_cast<int>(m_width) * 4 );
	}

	p_video_device_context->Unmap( m_texture_2D.Get() , 0 );
}