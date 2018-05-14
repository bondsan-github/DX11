#pragma once

#include <wrl/client.h> // unique_pointer?

//#include "..\DX11\Mesh.h"
#include "..\DX11\Texture.h"
//#include "..\DX11\WICImage.h"

//using std::vector;

class Quad : public Mesh // make mesh a component
{
	public:

		Quad() = delete;
		
		Quad( const uint in_width , const uint in_height , const XMFLOAT4 in_vertex_colour )
			: m_width(in_width) , m_height( in_height ) , m_vertex_colour( in_vertex_colour ) //Mesh( in_dimensions ),
		{
			float width_half = in_width * 0.5f;
			float height_half = in_height * 0.5f;

			vector< vertex_rgba_uv > quad_vertices
			{
				//					   position																  texture u , v
				vertex_rgba_uv( XMFLOAT3( -width_half , -height_half , 0.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) ,// bottom left
				vertex_rgba_uv( XMFLOAT3( -width_half ,  height_half , 0.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) ,// top left
				vertex_rgba_uv( XMFLOAT3(  width_half ,  height_half , 0.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 0.0f ) ) ,// top right
				vertex_rgba_uv( XMFLOAT3(  width_half , -height_half , 0.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 1.0f ) )  // bottom right
			};

			vector< ushort > quad_indices { 0u,1u,2u, 0u,2u,3u };

			// set_input_layout
			topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			vertices( quad_vertices );
			indices( quad_indices );

			m_diffuse = make_unique< Texture >( m_width, m_height , m_vertex_colour );
		}
		
		// from image
		Quad( const std::wstring in_filename )// , const bool use_image_dimensions , const XMFLOAT2 in_dimensions )
			//: Mesh( in_dimensions )
		{
			// maps.diffuse->load( in_filename );
			m_diffuse = make_unique< Texture >( in_filename );

			float width_half = 0.5f * m_diffuse->width();
			float height_half = 0.5f * m_diffuse->height();

			vector< vertex_rgba_uv > quad_vertices
			{
				//					   position																  texture u , v
				vertex_rgba_uv( XMFLOAT3( -width_half , -height_half , 0.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) ,// bottom left
				vertex_rgba_uv( XMFLOAT3( -width_half ,  height_half , 0.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) ,// top left
				vertex_rgba_uv( XMFLOAT3(  width_half ,  height_half , 0.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 0.0f ) ) ,// top right
				vertex_rgba_uv( XMFLOAT3(  width_half , -height_half , 0.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 1.0f ) )  // bottom right
			};

			vector< ushort > quad_indices { 0u,1u,2u, 0u,2u,3u };

			topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			vertices( quad_vertices );
			indices( quad_indices );

			// bool use_image_dimensions
			//float ½width = static_cast< float >( m_diffuse_map.width() ) / 2.0f;
			//float ½height = static_cast< float >( m_diffuse_map.height() ) / 2.0f;
		}

		const uint width() { return m_width; }
		const uint height() { return m_height; }

		//void line( const XMFLOAT4 in_points , const Colour in_colour )	{ m_texture->line( in_points , in_colour );	}

		void update()
		{
			Mesh::update();			
		}

		void render()
		{
			m_diffuse->update();
			Mesh::render();
		}

	private:
		//XMFLOAT2 m_dimension { };
		uint m_width { };
		uint m_height { };
		XMFLOAT4 m_vertex_colour { 1.0f , 0.0f , 0.0f , 1.0f };

		std::unique_ptr< Texture > m_diffuse = nullptr;

		// Mesh m_mesh;	
};

//normals required ?

		/*
		// copy constructor
		Quad_from_image( const Quad_from_image & in_move_from )
		{
			m_p_image = std::move( in_move_from.m_p_image );
		}

		// move constructor
		Quad_from_image( Quad_from_image && in_move_from )
		{
			m_p_image = std::move( in_move_from.m_p_image );
		}

		// move assignment operator
		Quad_from_image operator = ( Quad_from_image && in_move_from )
		{
			m_p_image = std::move( in_move_from.m_p_image );
		}
		*/
