#pragma once

//#include "DX11_game_lib.h"

//#include < DxErr.h > // FormatMessage
//#include < Windows.h >
//#include < xnamath.h > // requires windows.h before this inlcude

//#include < d3d11.h >
#include <d3d11_1.h>
#include <DirectXMath.h> 
using namespace DirectX;

#include <random>
#include <vector>

#include "D3Dcommon.h"
#include "vertex.h"
//#include "Mesh templates.h"

#include "debugging.h"

//enum AXIS { X, Y, Z };

//#include "mesh.tpp"

template< typename vertex_t >
class Mesh //abstract
{
	public:
							//Mesh( ) : p_video_device( 0 ) { } // override default copy constructor
                            Mesh( ID3D11Device * const p_video_device ); // : p_game_lib , matrix_world;
        virtual             ~Mesh();

        HRESULT             initialise(); // + create vertex & index buffers ? fill dynamically -> map

        ID3D11DeviceContext * const get_context_video();

        ////ID3D11Buffer *  const get_buffer_vertices() const { return p_buffer_vertex; }
        ////ID3D11Buffer *  const get_buffer_inidces()  const { return p_buffer_index; }
        
        HRESULT             create_buffer_vertices( );//const std::vector< TVertex > * p_vector_vertices );
        HRESULT             create_buffer_indices( );//const std::vector< unsigned int > * p_vector_indices );

        //HRESULT           create_and_fill_buffer( const D3D11_BIND_FLAG flags_bind, const std::vector< TYPE > * const p_vector_data );
        
        ////                create_map_diffuse( * 2D_image );


        const unsigned int  add_vertex_unique( std::vector< vertex_t > p_vector_vertices, const vertex_t new_vertex );
        void                add_vertex( const vertex_t new_vertex );
		//void				add_vertices( std::vector< vertex_t > );

        void                set_indices( const std::vector< unsigned long > vector_indices );

        //void              add_index( const unsigned int index );

        //void                set_total_vertices( const unsigned long total ) { total_vertices = total; }
        //void                set_total_indices( const unsigned long total )  { total_indices = total; }

        //const unsigned int  get_total_vertices() const   { return total_vertices; }
        //const unsigned int  get_total_indices() const    { return total_indices; }
        
        void                translate( float x, float y, float z );
        void                rotate( float x, float y, float z ); // rotate_on_x / y / z ( radians );
        //void                scale( float x, float y, float z  );
        //// translate() |  operator + ( x, y, z );
        
        XMMATRIX            get_matrix_world();

        virtual void        draw();
        void                draw_normals();

       // void                tessalate(const unsigned char iterations);

        void                add_triangle_tessalated( const triangle_indices new_triangle );
        //void              add_triangle( const TVertex new_triangle );

        const XMVECTOR      calc_normal( const vertex_t vector0, const vertex_t vector1, const vertex_t vector2 );  // const function

        void                calc_normals();
        
        //const TYPE        calc_centriod( const triangle_vertices triangle );
        const XMVECTOR      calc_centriod( const vertex_t vector0, const vertex_t vector1, const vertex_t vector2 );
        
        const float         square( const float number );

        const float         magnitude( const vertex_t vertex );

        //const vertex      normalise( const vertex vertex ) {};
        ////void            normalise( vector< TVertex > * const p_vector_vertices );

        const vertex        get_midpoint( const vertex_t vertex0, const vertex_t vertex1 );

	private:
		ID3D11Device				*	m_p_video_device			= nullptr;
		//ID3D11InputLayout			*	input_layout				= nullptr;

		ID3D11Buffer				*	m_p_buffer_vertex			= nullptr;
		unsigned long					m_ulong_total_vertices		= 0; // size_t *

		ID3D11Buffer				*	m_p_buffer_index			= nullptr;
		unsigned long					m_ulong_total_indices		= 0; // size_t *

		std::vector< vertex_t >			m_vector_vertices_tessalated { };
		std::vector< unsigned long >	m_vector_indices_tessalated { };

		XMMATRIX						m_matrix_world;// = XMMatrixIdentity();

		XMFLOAT3						m_float3_size { };
		XMFLOAT3						m_float3_position { };

		XMMATRIX						m_matrix_translate { };
		XMMATRIX						m_matrix_rotate { };
		XMMATRIX						m_matrix_scale { };

		ID3D11ShaderResourceView	*	m_p_diffuse_map			= nullptr; // ( aka colour_map )
		ID3D11SamplerState			*	m_p_diffuse_map_sampler	= nullptr;

		std::vector< vertex_t >			m_vector_vertices { };

		std::vector< unsigned long >	m_vector_indices { };

		std::vector< vertex_t >			m_vector_centroids { };
};
