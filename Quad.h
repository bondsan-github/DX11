#pragma once

#include <wrl/client.h> // unique_pointer?

#include "Mesh.h"
#include "Texture.h"
#include "WICImage.h"

using std::vector;

class Quad : public Mesh
{
	public:

		Quad() = delete;



		Quad( const XMFLOAT2 in_dimensions , const Colour_32bit_rgba in_vertex_colour )
			: m_vertex_colour( in_vertex_colour ) //Mesh( in_dimensions ),
		{
			m_dimension.x = in_dimensions.x * 0.5f;
			m_dimension.y = in_dimensions.y * 0.5f;

			vector< vertex_rgba_uv > quad_vertices
			{
				//					   position																  texture u , v
				vertex_rgba_uv( XMFLOAT3( -m_dimension.x , -m_dimension.y , 0.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) ,// bottom left
				vertex_rgba_uv( XMFLOAT3( -m_dimension.x ,  m_dimension.y , 0.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) ,// top left
				vertex_rgba_uv( XMFLOAT3(  m_dimension.x ,  m_dimension.y , 0.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 0.0f ) ) ,// top right
				vertex_rgba_uv( XMFLOAT3(  m_dimension.x , -m_dimension.y , 0.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 1.0f ) )  // bottom right
			};

			vector< ushort > quad_indices { 0u,1u,2u, 0u,2u,3u };

			// set_input_layout
			topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			vertices( quad_vertices );
			indices( quad_indices );

			m_diffuse = make_unique< Texture >( in_dimensions , in_vertex_colour );
		}
		
		// from image
		Quad( const wstring in_filename )// , const bool use_image_dimensions , const XMFLOAT2 in_dimensions )
			//: Mesh( in_dimensions )
		{
			// maps.diffuse->load( in_filename );
			m_diffuse = make_unique< Texture >( in_filename );

			m_dimension.x = 0.5 * m_diffuse->width();
			m_dimension.y = 0.5 * m_diffuse->height();

			vector< vertex_rgba_uv > quad_vertices
			{
				//					   position																  texture u , v
				vertex_rgba_uv( XMFLOAT3( -m_dimension.x , -m_dimension.y , 0.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) ,// bottom left
				vertex_rgba_uv( XMFLOAT3( -m_dimension.x ,  m_dimension.y , 0.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) ,// top left
				vertex_rgba_uv( XMFLOAT3(  m_dimension.x ,  m_dimension.y , 0.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 0.0f ) ) ,// top right
				vertex_rgba_uv( XMFLOAT3(  m_dimension.x , -m_dimension.y , 0.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 1.0f ) )  // bottom right
			};

			vector< ushort > quad_indices { 0u,1u,2u, 0u,2u,3u };

			topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			vertices( quad_vertices );
			indices( quad_indices );

			// bool use_image_dimensions
			//float ½width = static_cast< float >( m_diffuse_map.width() ) / 2.0f;
			//float ½height = static_cast< float >( m_diffuse_map.height() ) / 2.0f;
		}

		const float width() { return m_dimension.x;	}
		const float height() { return m_dimension.y; }

		//void line( const XMFLOAT4 in_points , const Colour in_colour )	{ m_texture->line( in_points , in_colour );	}

		void update()
		{
			Mesh::update();
			m_diffuse->update();
		}

	private:
		XMFLOAT2 m_dimension { };
		Colour_32bit_rgba m_vertex_colour { 1.0f , 0.0f , 0.0f , 1.0f };

		std::unique_ptr< Texture > m_diffuse = nullptr;

		//Mesh m_mesh;	
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
