#pragma once

#include <wrl/client.h> // unique_pointer

#include "Mesh.h"
#include "Texture.h"
//#include "WICImage.h"

class Quad : public Mesh // make mesh a component
{
	public:

	Quad();// = delete;

		// copy constructor
		//Quad( const Quad & ) = delete;					// unique_pointer cannot be copied

		// copy assignment constructor
		//Quad & operator = ( const Quad & ) = delete;	// unique_pointer cannot be copied

		//Quad( Quad&& in_quad ) // move constructor
		//{
		//	m_diffuse = move( in_quad.m_diffuse );
		//	in_quad.m_diffuse = nullptr;

		//	m_width = in_quad.m_width;
		//	m_height = in_quad.m_height;
		//	m_vertex_colour = in_quad.m_vertex_colour;
		//}
		
		void create_blank( const uint in_width , const uint in_height , const XMFLOAT4 in_vertex_colour )
			//: m_width(in_width) , m_height( in_height ) , m_vertex_colour( in_vertex_colour ) //Mesh( in_dimensions ),
		{
			_width = in_width; _height = in_height;  vertex_colour = in_vertex_colour;

			float ½width = in_width * 0.5f;
			float ½height = in_height * 0.5f;

			vector< vertex_rgba_uv > quad_vertices
			{
				//					      position												  texture u    , v
				vertex_rgba_uv( XMFLOAT3( -½width , -½height , 0.0f ) , vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) ,// bottom left
				vertex_rgba_uv( XMFLOAT3( -½width ,  ½height , 0.0f ) , vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) ,// top left
				vertex_rgba_uv( XMFLOAT3(  ½width ,  ½height , 0.0f ) , vertex_colour , XMFLOAT2( 1.0f , 0.0f ) ) ,// top right
				vertex_rgba_uv( XMFLOAT3(  ½width , -½height , 0.0f ) , vertex_colour , XMFLOAT2( 1.0f , 1.0f ) )  // bottom right
			};

			vector< ushort > quad_indices { 0u,1u,2u, 0u,2u,3u };

			// set_input_layout
			topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			vertices( quad_vertices );
			indices( quad_indices );

			//map_diffuse = make_unique< Texture >( m_width, m_height , m_vertex_colour );
			map_diffuse.create_blank( _width , _height , vertex_colour );
		}
		
		void load_diffuse( const std::wstring in_filename )
		{
			// maps.diffuse->load( in_filename );
			//map_diffuse = make_unique< Texture >( in_filename ); //copy / move constructor required
			map_diffuse.load( in_filename );

			float ½width  = 0.5f * map_diffuse.width();
			float ½height = 0.5f * map_diffuse.height();

			vector< vertex_rgba_uv > quad_vertices
			{
				//					   position																  texture u , v
				vertex_rgba_uv( XMFLOAT3( -½width , -½height , 0.0f ) , vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) ,// bottom left
				vertex_rgba_uv( XMFLOAT3( -½width ,  ½height , 0.0f ) , vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) ,// top left
				vertex_rgba_uv( XMFLOAT3(  ½width ,  ½height , 0.0f ) , vertex_colour , XMFLOAT2( 1.0f , 0.0f ) ) ,// top right
				vertex_rgba_uv( XMFLOAT3(  ½width , -½height , 0.0f ) , vertex_colour , XMFLOAT2( 1.0f , 1.0f ) )  // bottom right
			};

			vector< ushort > quad_indices { 0u,1u,2u, 0u,2u,3u };

			topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			vertices( quad_vertices );
			indices( quad_indices );
		}

		const uint width()	{ return map_diffuse.width(); }
		const uint height()	{ return  map_diffuse.height(); }

		//void line( const XMFLOAT4 in_points , const Colour in_colour )	{ m_texture->line( in_points , in_colour );	}

		void update() 
		{
			Mesh::update();			
		}

		void render()
		{
			map_diffuse.update( 0.0 );
			Mesh::render();
		}

	private:
		//XMFLOAT2 m_dimension { };
		uint _width {};
		uint _height {};
		XMFLOAT4 vertex_colour { 1.0f , 0.0f , 0.0f , 1.0f };

		//std::unique_ptr< Texture > m_diffuse = nullptr;
		Texture map_diffuse;

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
