#pragma once

#include "Mesh_2.h"

template< typename vertex_t >
class Quad_colour : public Mesh< vertex_t >
{
	public:

		Quad_colour( Microsoft::WRL::ComPtr< ID3D11Device > p_video_device )
			: Mesh( p_video_device )
		{
			std::vector< vertex_t > quad_vertices 
			{
				//		  position							colour	  
				vertex_t( XMFLOAT3( -0.5f ,  0.5f , 0.0f ), XMFLOAT4( 1.0f , 0.0f , 0.0f , 1.0f ) ),	// 0
				vertex_t( XMFLOAT3(  0.5f ,  0.5f , 0.0f ), XMFLOAT4( 0.0f , 1.0f , 0.0f , 1.0f ) ),	// 1
				vertex_t( XMFLOAT3(  0.5f , -0.5f , 0.0f ), XMFLOAT4( 0.0f , 0.0f , 1.0f , 1.0f ) ),	// 2
				vertex_t( XMFLOAT3( -0.5f , -0.5f , 0.0f ), XMFLOAT4( 1.0f , 1.0f , 1.0f , 1.0f ) )		// 3
			};
			
			std::vector< unsigned short > quad_indices
			{
				0,1,3, 3,1,2
			};

			set_topology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
			set_vertices( quad_vertices );
			set_indices( quad_indices );

			//quad_vertices.clear(); vectors going out of scope - auto destructor called?
			//quad_indices.clear();
		}
};	