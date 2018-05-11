#pragma once

#include <vector>
#include <memory>
#include <wrl/client.h>

#include <d3d11.h>
//#include <DXGI.h>

#include "Drawable.h"
#include "Vertex.h"
#include "Texture.h"
#include "debugging.h"
#include "types.h"

#define _USE_MATH_DEFINES // for C++
#include <math.h>

using Microsoft::WRL::ComPtr;
using std::vector;
using std::make_unique;
using std::shared_ptr;

//template< typename vertex_t > //= vertex >
class Mesh : public Drawable//, public Texture // abstract
{
	public:
		//Mesh_manager.add_mesh( wstring name, wstring filename );
		Mesh( );

		void topology( const D3D11_PRIMITIVE_TOPOLOGY in_toplogy ) { m_primitive_topology = in_toplogy; }
		
		//void add_vertex( const vertex_t new_vertex )
		void add_vertex( const vertex_rgba_uv new_vertex )
		{
			m_vector_vertices.push_back( new_vertex );
			create_buffer_vertices();
		}

		//void create_buffer_vertices( vertex_t * vertices , UINT number_of_vertcies );
		//void set_vertices( std::vector< vertex_t > & new_vertices )
		void vertices( const std::vector< vertex_rgba_uv > & new_vertices )
		{
			m_vector_vertices = new_vertices;
			create_buffer_vertices();
		}
		void indices( const std::vector< unsigned short > & vector_indices )
		{
			m_vector_indices = vector_indices;
			create_buffer_indices();
		}

		void IASetVertexBuffers()
		{
			//unsigned int strides[ 1 ] { sizeof( vertex_t ) };
			//unsigned int offsets[ 1 ] { 0u };

			UINT stride = sizeof( vertex_rgba_uv );
			UINT offset = 0u;
			
			mp_video_device_context->IASetVertexBuffers( 0 ,								// The first input slot for binding.
														 1 ,								// The number of vertex buffers in the array.
														 mp_buffer_vertex.GetAddressOf() ,	// A pointer to an array of vertex buffers 
														 &stride ,							// Pointer to an array of stride values
														 &offset );							// Pointer to an array of offset values
		}

		void IASetIndexBuffer()
		{
			mp_video_device_context->IASetIndexBuffer( mp_buffer_index.Get() ,	// A pointer to an ID3D11Buffer object   
													   DXGI_FORMAT_R16_UINT ,	// 16-bit or 32-bit unsigned integers 
													   0 );						// Offset (in bytes) from the start of the index buffer to the first index to use.
																				// short , unsigned short = 2 bytes = 8 * 2 = 16 bits
																				// long  , unsigned long  = 4 bytes = 8 * 4 = 32 bits	
		}

		void submit_draw();

		void position( const XMFLOAT3 in_position );

		const XMFLOAT3 position( void ) const { return m_f3_position; }			
		
		void translate_x( const float in_x );
		void translate_y( const float in_y );
		void translate_z( const float in_z );

		//void x( const float in_x );
				
		//void set_rotation( const XMFLOAT3 in_f3_rads );
		//void set_angle_x( const float in_rad );
		//void set_angle_y( const float in_rad );
		//void set_angle_z( const float in_rad );

		void rotate_x( const float in_rad );
		void rotate_y( const float in_rad );
		void rotate_z( const float in_rad ); // local z

		//void rotate_origin_z( const float in_rad );
		//void rotate_world_z( const float in_rads );

		//void rotate_axis( const XMVECTOR in_axis , const float in_radians );

		void scale( const XMFLOAT3 in_f3_scale );
		void delta_scale( const XMFLOAT3 in_f3_scale );

		//std::shared_ptr<map> map( maps::diffuse )
		
		void update();
		void update_matrix_world();

		void matrix_world( const XMMATRIX in_world ) 
		{ 
			m_matrix_world = in_world; 
			update_matrix_world();
		}

		void rotation( const XMFLOAT3 in_rotation ) 
		{ 
			m_f3_rotation = in_rotation; 
			m_matrix_rotation = XMMatrixRotationRollPitchYaw( m_f3_rotation.x , m_f3_rotation.y , m_f3_rotation.z );

			update_matrix_world();
		}

		//XMMATRIX get_matrix_world() const { return m_matrix_world; }	

		void rotate_point_z( const XMFLOAT2 in_pivot , const float in_angle_z )
		{
			// capture current position

			// matrix: translate - pivot vector
			// matrix: rotate z
			// matrix: translate + pivot vector

			// One revolution is equal to 2π

			// input = radians ( per ms or seconds )


			// tangential speed v = rotational speed ω * radial distance r
			// v = rω

			// r = pivot - m_f3_position
			// ω = 1 rotation (2π) per second

			// full circle S = 2πr
			// theta = S / r

			// 1s / 2π  = 0.1591549437
			float fullrotation_1s = 1.0 / (2.0 * M_PI);

			const float theta = 0.1;

			//XMFLOAT2 pivot = current_pos - pivot;
			XMFLOAT3 pivot { -150.0f, 0.0f, 0.0f };

			XMFLOAT3 current_postion = m_f3_position;

			// negate pivot_vector
			XMFLOAT3 pivot_to_origin = XMFLOAT3( current_postion.x - pivot.x , current_postion.y - pivot.y , current_postion.z - pivot.z );

			// absolute translation 
			XMMATRIX MMove_to_origin = XMMatrixTranslation( pivot_to_origin.x , pivot_to_origin.y , pivot_to_origin.z );

			// rotate verts at unknown tangential speed
			XMMATRIX MRotate = XMMatrixRotationZ( theta );
						
			// add pivot vector
			XMFLOAT3 origin_to_pivot = XMFLOAT3(  pivot.x , pivot.y , pivot.z );

			XMMATRIX MMove_to_pivot = XMMatrixTranslation( origin_to_pivot.x , origin_to_pivot.y , origin_to_pivot.z );
						
			m_matrix_world = MMove_to_origin * MRotate * MMove_to_pivot;			

			XMVECTOR current_position { };
			XMVECTOR scale { };
			XMVECTOR rotation { };

			XMMatrixDecompose( &scale , &rotation , &current_position , m_matrix_world );

			m_f3_position.x = XMVectorGetX( current_position );
			m_f3_position.y = XMVectorGetY( current_position );
			m_f3_position.z = XMVectorGetZ( current_position );

			m_matrix_world = XMMatrixTranspose( m_matrix_world );

			update_buffer_matrix_world();

			position( m_f3_position );			
		}

		void render();

	private:

		HRESULT							mh_result { E_FAIL };
				
		ComPtr< ID3D11Device >			mp_video_device;
		ComPtr< ID3D11DeviceContext >	mp_video_device_context;

		//vector< vertex_t >			m_vector_vertices;
		vector< vertex_rgba_uv >		m_vector_vertices;
		vector< ushort >				m_vector_indices;

		D3D11_PRIMITIVE_TOPOLOGY		m_primitive_topology;// = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ComPtr< ID3D11Buffer >			mp_buffer_vertex;	// change name to m_p_buffer_vertices
		unsigned long					mul_total_vertices {};

		ComPtr< ID3D11Buffer >			mp_buffer_index;
		unsigned long					mul_total_indices {};

		XMFLOAT3						m_f3_position {};
		XMFLOAT3						m_f3_rotation {};
		XMFLOAT3						m_f3_scale { 1.0f, 1.0f, 1.0f };
		
		XMMATRIX						m_matrix_world			= XMMatrixIdentity();
		//XMMATRIX						matrix_local = XMMatrixIdentity();

		XMMATRIX						m_matrix_scale			= XMMatrixIdentity();
		XMMATRIX						m_matrix_rotation		= XMMatrixIdentity();
		XMMATRIX						m_matrix_translation	= XMMatrixIdentity();

		D3D11_BUFFER_DESC				m_buffer_world_description { };
		//D3D11_SUBRESOURCE_DATA		m_struct_subresource_data {};
		ComPtr< ID3D11Buffer >			mp_buffer_matrix_world;						
		
		//XMFLOAT2 texture2D_dimensions { };
		
		// void - catch exceptions
		void create_buffer_vertices();
		void create_buffer_indices();

		void create_buffer_matrix_world();
				
		// void set_diffuse_map( Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > in_shader_resource_view );

		// void set_IA_vertex_buffer( Microsoft::WRL::ComPtr< ID3D11Buffer >	in_vertex_buffer );
		// void set_IA_index_buffer( );
		// void set_PS_resource_diffuse_map( Microsoft::WRL::ComPtr< ID3D11Texture2D > in_diffuse_map );
		// void draw_indexed();

		// void set_buffer_matrix_world( XMMATRIX in_matrix_world )
//		void update_matrix_world();
		void update_buffer_matrix_world(); 		
		void VSSetConstantBuffers();

	//protected:
		//std::unique_ptr< Texture > m_texture;
};
