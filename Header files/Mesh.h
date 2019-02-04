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

//template< typename vertex_type > //= vertex >

class Mesh : public Drawable // abstract
{
	private:

		HRESULT							result { E_FAIL }; // result

		ComPtr< ID3D11Device >			video_device;
		ComPtr< ID3D11DeviceContext >	video_device_context;

		//vector< vertex_type >			vertices;
		vector< vertex_rgba_uv >		vertices;
		vector< ushort >				indices;

		D3D11_PRIMITIVE_TOPOLOGY		primitive_topology;// = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ComPtr< ID3D11Buffer >			vertex_buffer;
		unsigned long					vertices_count {};

		ComPtr< ID3D11Buffer >			index_buffer;
		unsigned long					indices_count {};

		XMFLOAT3						position {};
		XMFLOAT3						rotation {};
		XMFLOAT3						scale { 1.0f, 1.0f, 1.0f };

		XMMATRIX						world_matrix		= XMMatrixIdentity();
		//XMMATRIX						matrix_local		= XMMatrixIdentity();
		XMMATRIX						scale_matrix		= XMMatrixIdentity();
		XMMATRIX						rotation_matrix		= XMMatrixIdentity();
		XMMATRIX						translation_matrix	= XMMatrixIdentity();

		D3D11_BUFFER_DESC				world_buffer_description {};
		//D3D11_SUBRESOURCE_DATA		subresource_data {};
		ComPtr< ID3D11Buffer >			world_matrix_buffer;

		//XMFLOAT2 texture2D_dimensions { };

		// void - catch exceptions
		void create_buffer_vertices();
		void create_buffer_indices();

		void create_buffer_matrix_world();

		// void set_diffuse_map( Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > view );

		// void set_IA_vertex_buffer( Microsoft::WRL::ComPtr< ID3D11Buffer > verticies );
		// void set_IA_index_buffer( );
		// void set_PS_resource_diffuse_map( Microsoft::WRL::ComPtr< ID3D11Texture2D > map );
		// void draw_indexed();

		// void set_matrix_world_buffer( XMMATRIX world )
		//		void update_matrix_world();
		void update_matrix_world_buffer();
		void set_VS_constant_buffers();

		void set_IA_vertex_buffers() //
		{
			//unsigned int strides[ 1 ] { sizeof( vertex_t ) };
			//unsigned int offsets[ 1 ] { 0u };

			UINT stride = sizeof( vertex_rgba_uv );
			UINT offset = 0u;

			video_device_context->IASetVertexBuffers( 0 ,								// first input slot for binding.
													  1 ,								// number of vertex buffers in the array.
													  vertex_buffer.GetAddressOf() ,	// pointer to vertex buffer array
													  & stride ,						// pointer to stride values array
													  & offset );						// pointer to offset values
		}

	public:

		//Mesh_manager.add_mesh( wstring name, wstring filename );

		Mesh( );

		void set_topology( const D3D11_PRIMITIVE_TOPOLOGY in_toplogy )
		{ 
			primitive_topology = in_toplogy;
		}
		
		//void add_vertex( const vertex_type new_vertex )

		void add_vertex( const vertex_rgba_uv new_vertex )
		{
			vertices.push_back( new_vertex );
			create_buffer_vertices();
		}

		//void create_buffer_vertices( vertex_type * vertices , UINT vertcies_count );
		//void vertices( std::vector< vertex_type > & new_vertices )

		void set_vertices( const vector< vertex_rgba_uv > & new_vertices )
		{
			vertices = new_vertices;
			create_buffer_vertices();
		}

		void set_indices( const vector< unsigned short > & vector_indices )
		{
			indices = vector_indices;
			create_buffer_indices();
		}
		
		void set_IA_index_buffer()
		{
			video_device_context->IASetIndexBuffer( index_buffer.Get() ,	// A pointer to an ID3D11Buffer object   
													DXGI_FORMAT_R16_UINT ,	// 16-bit or 32-bit unsigned integers 
													0 );						// Offset (in bytes) from the start of the index buffer to the first index to use.
																				// short , unsigned short = 2 bytes = 8 * 2 = 16 bits
																				// long  , unsigned long  = 4 bytes = 8 * 4 = 32 bits	
		}

		void submit_draw();

		void set_position( const XMFLOAT3 in_position );

		const XMFLOAT3 get_position( void ) const 
		{ 
			return position; 
		}
		
		void translate_x( const float in_x );
		void translate_y( const float in_y );
		void translate_z( const float in_z );

		//void set_x( const float in_x );
				
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

		void set_scale( const XMFLOAT3 in_f3_scale );
		void delta_scale( const XMFLOAT3 in_f3_scale );

		//std::shared_ptr<map> map( maps::diffuse )
		
		void update();
		void update_world_matrix();

		void set_world_matrix( const XMMATRIX in_world ) 
		{ 
			world_matrix = in_world;
			update_world_matrix();
		}

		void set_rotation( const XMFLOAT3 in_rotation ) 
		{ 
			rotation = in_rotation; 
			rotation_matrix = XMMatrixRotationRollPitchYaw( rotation.x , rotation.y , rotation.z );

			update_world_matrix();
		}

		//XMMATRIX get_matrix_world() const { return m_matrix_world; }	

		void rotate_point_z( const XMFLOAT3 in_pivot , const float in_angle_z )
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
			float fullrotation_1s = 1.0f / ( 2.0f * 3.1415926535f );// M_PI);

			const float theta = 0.1f;

			//XMFLOAT2 pivot = current_pos - pivot;
			XMFLOAT3 pivot { in_pivot.x , in_pivot.y , in_pivot.z };

			XMFLOAT3 current_postion = position;

			// negate pivot_vector
			XMFLOAT3 pivot_to_origin = XMFLOAT3( current_postion.x - pivot.x , current_postion.y - pivot.y , current_postion.z - pivot.z );

			// absolute translation 
			XMMATRIX MMove_to_origin = XMMatrixTranslation( pivot_to_origin.x , pivot_to_origin.y , pivot_to_origin.z );

			// rotate verts at unknown tangential speed
			XMMATRIX MRotate = XMMatrixRotationZ( theta );
						
			// add pivot vector
			XMFLOAT3 origin_to_pivot = XMFLOAT3(  pivot.x , pivot.y , pivot.z );

			XMMATRIX MMove_to_pivot = XMMatrixTranslation( origin_to_pivot.x , origin_to_pivot.y , origin_to_pivot.z );
						
			world_matrix = MMove_to_origin * MRotate * MMove_to_pivot;

			XMVECTOR current_position { };
			XMVECTOR scale { };
			XMVECTOR rotation { };

			XMMatrixDecompose( &scale , &rotation , &current_position , world_matrix );

			position.x = XMVectorGetX( current_position );
			position.y = XMVectorGetY( current_position );
			position.z = XMVectorGetZ( current_position );

			world_matrix = XMMatrixTranspose( world_matrix );

			set_position( position );

			update_matrix_world_buffer();
		}

		void render();
		
	//protected:
		//Texture texture;
};
