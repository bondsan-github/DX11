#pragma once

//#include <wrl/client.h> // unique_pointer

#include "Mesh.h"
#include "Texture.h"

class Quad : public Mesh
{
	private:

		uint width{};
		uint height{};
		XMFLOAT4 vertex_colour { 0.0f , 0.0f , 0.0f , 1.0f };

		Texture texture_diffuse;

		rectangle border;

		// Mesh mesh;	

	public:

		Quad() {}// = delete;

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

		Quad( const std::wstring in_filename ) 
		{
			load_diffuse( in_filename );
		}
		
		void create_blank( const uint in_width , const uint in_height , const XMFLOAT4 in_vertex_colour )
		//: width( in_width ) , height( in_height ) , vertex_colour( in_vertex_colour )
		//error C2590 : only a constructor can have a base / member initializer list
		{
			float ½width	= in_width * 0.5f;
			float ½height	= in_height * 0.5f;

			vector< vertex_rgba_uv > quad_vertices
			{
				//					      position									    texture   u    , v
				vertex_rgba_uv( XMFLOAT3( -½width , -½height , 0.0f ) , vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) ,// bottom left
				vertex_rgba_uv( XMFLOAT3( -½width ,  ½height , 0.0f ) , vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) ,// top left
				vertex_rgba_uv( XMFLOAT3(  ½width ,  ½height , 0.0f ) , vertex_colour , XMFLOAT2( 1.0f , 0.0f ) ) ,// top right
				vertex_rgba_uv( XMFLOAT3(  ½width , -½height , 0.0f ) , vertex_colour , XMFLOAT2( 1.0f , 1.0f ) )  // bottom right
			};

			vector< ushort > quad_indices { 0u , 1u , 2u , 0u , 2u , 3u };

			// set_input_layout
			set_topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			set_vertices( quad_vertices );
			set_indices( quad_indices );

			//texture_diffuse.create_blank( width , height , vertex_colour );
		}
		
		void load_diffuse( const std::wstring in_filename )
		{
			texture_diffuse.load( in_filename );

			width	= texture_diffuse.get_width();
			height	= texture_diffuse.get_height();

			float ½width  = 0.5f * texture_diffuse.get_width();
			float ½height = 0.5f * texture_diffuse.get_height();

			vector< vertex_rgba_uv > quad_vertices
			{
				//					   position																  texture u , v
				vertex_rgba_uv( XMFLOAT3( -½width , -½height , 0.0f ) , vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) ,// bottom left
				vertex_rgba_uv( XMFLOAT3( -½width ,  ½height , 0.0f ) , vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) ,// top left
				vertex_rgba_uv( XMFLOAT3(  ½width ,  ½height , 0.0f ) , vertex_colour , XMFLOAT2( 1.0f , 0.0f ) ) ,// top right
				vertex_rgba_uv( XMFLOAT3(  ½width , -½height , 0.0f ) , vertex_colour , XMFLOAT2( 1.0f , 1.0f ) )  // bottom right
			};

			vector< ushort > quad_indices { 0u,1u,2u, 0u,2u,3u };

			set_topology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			set_vertices( quad_vertices );
			set_indices( quad_indices );
		}

		const uint get_width()	const { return texture_diffuse.get_width(); }
		const uint get_height()	const { return texture_diffuse.get_height(); }

		//void line( const XMFLOAT4 in_points , const Colour in_colour )	{ m_texture->line( in_points , in_colour );	}

		void update() 
		{
			texture_diffuse.update( 0.0 );
			Mesh::update();			
		}

		void render()
		{
			texture_diffuse.render();
			Mesh::render();
		}

		rectangle get_border()
		{
			border.top		= get_position().y + ( height * 0.5f );
			border.bottom	= get_position().y - ( height * 0.5f );
			border.left		= get_position().x - ( width  * 0.5f );
			border.right	= get_position().x + ( width  * 0.5f );

			return border;
		}
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
