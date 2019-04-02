#pragma once

#include <vector>

#include "Mesh.h"

class Triangle : public Mesh
{
	public:
		Triangle( const XMFLOAT2 in_dimensions , const XMFLOAT4 in_vertex_colour )
			: m_vertex_colour( in_vertex_colour )
		{
			m_dimension.x = 0.5f * in_dimensions.x;
			m_dimension.y = 0.5f * in_dimensions.y;

			vector< vertex_rgba_uv > triangle_vertices
			{
				vertex_rgba_uv( XMFLOAT3( -m_dimension.x , -m_dimension.y , 1.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 1.0f ) ) , // bottom left
				vertex_rgba_uv( XMFLOAT3( -m_dimension.x ,  m_dimension.y , 1.0f ) , m_vertex_colour , XMFLOAT2( 0.0f , 0.0f ) ) , // top left
				vertex_rgba_uv( XMFLOAT3(  m_dimension.x ,  m_dimension.y , 1.0f ) , m_vertex_colour , XMFLOAT2( 1.0f , 0.0f ) )   // top right
			};
		}

		vector< ushort > triangle_indices { 0u,1u,2u, 0u,2u,3u };

	private:
		XMFLOAT2 m_dimension { };
		XMFLOAT4 m_vertex_colour { 0.0f , 0.0f , 0.0f , 0.0f };
};
