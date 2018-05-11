#pragma once

#include <Windows.h>
#include <D3D11.h>
#include <directxmath.h>

#include "utilities.h"
#include "Mesh.h"

class Sprite : public Mesh< vertex_texture > // public Quad_textured<>
{
	public:

		// sprite_manager.add_sprite( "brick_red , "/images/brick_red.bmp" );

		Sprite( Microsoft::WRL::ComPtr< ID3D11Device > p_video_device , LPCTSTR image_filename )
			: Mesh( p_video_device )
		{
			std::vector< vertex_texture > quad_vertices =
			{ 
				//				position					  texture u , v
				vertex_texture( XMFLOAT3( -0.5 , -0.5 , 0 ) , XMFLOAT2( 0.0 , 0.0 ) ) ,
				vertex_texture( XMFLOAT3( -0.5 ,  0.5 , 0 ) , XMFLOAT2( 1.0 , 0.0 ) ) ,
				vertex_texture( XMFLOAT3(  0.5 , -0.5 , 0 ) , XMFLOAT2( 1.0 , 1.0 ) ) ,
				vertex_texture( XMFLOAT3(  0.5 ,  0.5 , 0 ) , XMFLOAT2( 0.0 , 1.0 ) )
			};
			
			set_topology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

			set_vertices( quad_vertices );

			quad_vertices.clear();

			// Triangle strip
			// ABCD
			// sequence will decoded as a set of triangles ABC, BCD
			// every even-numbered (from one) triangle will be reversed resulting in the original triangles

			// std::vector< size_t > quad_indices = { }
		}

		~Sprite()
		{
			m_texture2d_image = nullptr;
		}

	private:

		// File m_file_texture;
		//Mesh m_mesh_sprite;
		ID3D11Texture2D * m_texture2d_image = nullptr;

		/*
		HRESULT CreateTexture2D(
			[ in ]            const D3D11_TEXTURE2D_DESC   *pDesc ,
			[ in , optional ]  const D3D11_SUBRESOURCE_DATA *pInitialData ,
			[ out , optional ]       ID3D11Texture2D        **ppTexture2D
		);
		*/


};

void add_sprite( LPCTSTR sprite_name , LPCTSTR image_filename )
{
	/*
	Image sprite_image( image_filename );

	D3D11_TEXTURE2D_DESC texture2d_description;
	texture2d_description.Width				= sprite_image.get_width();
	texture2d_description.Height			= sprite_image.get_height();
	texture2d_description.MipLevels			= 1;
	texture2d_description.ArraySize			= 1;
	texture2d_description.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2d_description.SampleDesc.Count	= 1;
	texture2d_description.Usage				= D3D11_USAGE_DYNAMIC;
	texture2d_description.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texture2d_description.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	texture2d_description.MiscFlags			= 0;
	*/
	/*
	typedef struct D3D11_SUBRESOURCE_DATA {
	const void *pSysMem;
	UINT       SysMemPitch;
	UINT       SysMemSlicePitch;
	} D3D11_SUBRESOURCE_DATA;
	*/

	// (const char*)pSysMem + SysMemPitch*y + (x * BytesPerPixel)

	/*
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = temp.get( );
	initData.SysMemPitch = sprite_image.get_row_pitch(); // static_cast<UINT>( rowPitch );
	initData.SysMemSlicePitch = static_cast<UINT>( imageSize );

	ID3D11Texture2D * texture = nullptr;
	hr = m_p_video_device->CreateTexture2D( &texture2d_description , &initData , &texture );
	*/

	// sprite_animated();
}