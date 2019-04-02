#pragma once

#include <vector>
#include <memory>
#include <wrl/client.h>
#include <limits.h> // for FLT_MAX/MIN
#include <d3d11.h>
//#include <DXGI.h>
#include <DirectXMath.h>

#include "Graphics_component.h"
#include "vertex.h"
#include "debugging.h"
#include "types.h"
#include "DX11_math.h"

//#define _USE_MATH_DEFINES // for C++
//#include <math.h>

using Microsoft::WRL::ComPtr;
using std::vector;
using std::make_unique;
using std::shared_ptr;

using DirectX::XMFLOAT3;
using DirectX::XMVECTOR;
using DirectX::XMMATRIX;
using DirectX::XMVectorGetX;
using DirectX::XMMatrixIdentity;
using DirectX::XMVectorGetX;
using DirectX::XMVectorGetY;
using DirectX::XMVectorGetZ;

//template< typename vertex_type > //= vertex >

class Mesh : public Graphics_component
{
	public:

		//Mesh_manager.add_mesh( wstring name, wstring filename );

		Mesh();

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
		
		//void submit_draw();

		void set_position( const XMFLOAT3 in_position );
		void set_position( const XMVECTOR in_position );
		void set_position( const float in_x , const float in_y , const float in_z );

		const XMFLOAT3 get_position( void ) const;

		//const XMVECTOR get_position()
		//{
		//	XMMATRIX temp_matrix = XMMatrixTranspose( world_matrix );
		//	return temp_matrix.r[ 3 ];
		//}
		
		void translate_x( const float in_x );
		void translate_y( const float in_y );
		void translate_z( const float in_z );
		//void set_x( const float in_x );
				
		void set_rotation( const XMFLOAT3 in_rotation );
		//void set_rotation( const XMVECTOR in_rotation )
		//{
		//	rotation.x = XMVectorGetX( in_rotation );
		//	rotation.y = XMVectorGetY( in_rotation );
		//	rotation.z = XMVectorGetZ( in_rotation );
		//}

		//void set_angle_x( const float in_rad );
		//void set_angle_y( const float in_rad );
		//void set_angle_z( const float in_rad );

		void rotate_x( const float in_rad );
		void rotate_y( const float in_rad );
		void rotation_z( const float in_rad );
		
		//void Mesh::rotate_axis( const XMVECTOR in_axis , const float in_radians ) {}

		void rotation_z_about_point( const XMFLOAT3 in_pivot , const float in_angle_z );
		//void rotate_point_z( const XMFLOAT3 in_pivot , const float in_angle_z , const XMFLOAT3 in_translation );

		void orbit_point_z( const XMFLOAT3 in_point , const float in_angle_z );
		void orbit_point_z( const XMVECTOR in_point , const float in_angle_z );

		//void rotate_origin_z( const float in_rad );
		//void rotate_world_z( const float in_rads );

		void orbit_z( const float in_radians_z );

		void set_scale( const XMFLOAT3 in_scale );
		void delta_scale( const XMFLOAT3 in_f3_scale );
				
		void update( double time_delta );
		void update_world_matrix();

		void set_world_matrix( const XMMATRIX in_world ) 
		{ 
			world_matrix = in_world;
			update_world_matrix();
		}

		XMMATRIX get_world_matrix() const { return world_matrix; }

		vector<vertex_rgba_uv> get_world_vertices() const;
		
		// get_AABB
		Bounding_box get_bounding_box(); // (world coordinates)

		void render();
		
		//std::shared_ptr<map> map( maps::diffuse )
	//protected:
		//Texture texture;

	private:

		HRESULT							result { E_FAIL };

		//ComPtr< ID3D11Device >			video_device;
		ComPtr< ID3D11DeviceContext >	video_device_context;

		//vector< vertex_type >			vertices;

		vector< vertex_rgba_uv >		vertices;
		vector< ushort >				indices;

		D3D11_PRIMITIVE_TOPOLOGY		primitive_topology;// = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		ComPtr< ID3D11Buffer >			vertex_buffer;
		unsigned long					vertices_count {};

		ComPtr< ID3D11Buffer >			index_buffer;
		unsigned long					indices_count {};

		//XMFLOAT3						position {};
		//XMFLOAT3						rotation {};
		//XMFLOAT3						scale { 1.0f, 1.0f, 1.0f };

		XMMATRIX						world_matrix		= XMMatrixIdentity();
		XMMATRIX						scale_matrix		= XMMatrixIdentity();
		XMMATRIX						rotation_matrix		= XMMatrixIdentity();
		XMMATRIX						translation_matrix	= XMMatrixIdentity();
		XMMATRIX						orbit_matrix		= XMMatrixIdentity();

		D3D11_BUFFER_DESC				world_buffer_description {};
		//D3D11_SUBRESOURCE_DATA		subresource_data {};
		ComPtr< ID3D11Buffer >			world_matrix_buffer;

		//XMFLOAT2 texture2D_dimensions { };

		// void - catch exceptions
		void create_buffer_vertices();
		void create_buffer_indices();

		void create_buffer_matrix_world();

		// void set_diffuse_map( Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > view );

		// void set_IA_vertex_buffer( ComPtr< ID3D11Buffer > verticies );
		// void set_IA_index_buffer( );
		// void set_PS_resource_diffuse_map( Microsoft::WRL::ComPtr< ID3D11Texture2D > map );
		// void draw_indexed();

		void update_world_matrix_buffer(); // update_buffer_world_matrix();

		void set_VS_constant_buffers();
		void set_IA_vertex_buffers();

		void set_IA_index_buffer()
		{
			video_device_context->IASetIndexBuffer( index_buffer.Get() ,	// A pointer to an ID3D11Buffer object   
													DXGI_FORMAT_R16_UINT ,	// 16-bit or 32-bit unsigned integers 
													0 );						// Offset (in bytes) from the start of the index buffer to the first index to use.
																				// short , unsigned short = 2 bytes = 8 * 2 = 16 bits
																				// long  , unsigned long  = 4 bytes = 8 * 4 = 32 bits	
		}
};

// get_AABB
// for each vertex
//   world_vertex = vertex[i] *= world_matrix;

//vector< XMFLOAT3 > world_vertices;

////XMFLOAT3 point{};
////float x , y , z = 0; // ! ;)

////AABB bob_test {};
////
////for( auto & vertex : vertices )
////{

////	XMVECTOR vertex_world = XMVector3Transform( XMLoadFloat3( &XMFLOAT3( vertex.point.x , vertex.point.y , vertex.point.z ) ) , world_matrix );
////	
////	x = XMVectorGetX( vertex_world );
////	y = XMVectorGetY( vertex_world );
////	z = XMVectorGetZ( vertex_world );

////	debug_out( "\nvertex.x = %.2f , y = %.2f" , vertex.point.x , vertex.point.y );
////	debug_out( "   _world.x = %.2f , y = %.2f" , x , y );
////}
////
////debug_out( "\n" );

//D3D11_MAPPED_SUBRESOURCE mapped_buffer;
//result = video_device_context->Map( vertex_buffer.Get() , 0 , D3D11_MAP_READ , 0 , &mapped_buffer );
//vertex_rgba_uv * world_verticies = reinterpret_cast< vertex_rgba_uv * >( mapped_buffer.pData );
//AABB bounding_box;
//bounding_box.min.x = world_vertices			
//for( uint i = 0; i < vertices.size(); ++i )
//{
//	//temp_verticies[ i ].Pos = mWaves[ i ];
//}
//video_device_context->Unmap( vertex_buffer.Get() , 0 );

//ID3D11Buffer * staging_vertex_buffer;
//D3D11_BUFFER_DESC staging_vertex_buffer_description{};
//staging_vertex_buffer_description.ByteWidth = sizeof( XMMATRIX );
//staging_vertex_buffer_description.Usage = D3D11_USAGE_STAGING;
//staging_vertex_buffer_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			
////D3D11_USAGE_STAGING = A resource that supports data transfer (copy) from the GPU to the CPU.
//video_device_context->CopyResource( vertex_buffer.Get() , &staging_vertex_buffer );
////Create a 2nd buffer with D3D11_USAGE_STAGING; fill the second buffer using ID3D11DeviceContext::Map , ID3D11DeviceContext::Unmap; 
////use ID3D11DeviceContext::CopyResource to copy from the staging buffer to the default buffer.

//void set_rotation( const XMFLOAT3 in_rotation ) 
//{ 
//	rotation = in_rotation; 
//	rotation_matrix = XMMatrixRotationRollPitchYaw( rotation.x , rotation.y , rotation.z );

//	update_world_matrix();
//}

//XMFLOAT3 get_centre()
//{
//XMVECTOR vertex_world{};
//XMFLOAT3 centre {};

//for( auto & vertex : vertices )
//{
//	vertex_world = XMVector3Transform( XMLoadFloat3( &XMFLOAT3( vertex.point.x , vertex.point.y , vertex.point.z ) ) , world_matrix );

//	centre.x += XMVectorGetX( vertex_world );
//	centre.y += XMVectorGetY( vertex_world );
//	centre.z += XMVectorGetZ( vertex_world );
//}

//centre.x /= vertices.size();
//centre.y /= vertices.size();
//centre.z /= vertices.size();

//return centre;
//}

/*XMVECTOR new_position {};
XMVECTOR new_scale {};
XMVECTOR new_rotation {};

XMMatrixDecompose( & new_scale , & new_rotation , & new_position , world_matrix );

x = XMVectorGetX( new_position );
y = XMVectorGetY( new_position );
z = XMVectorGetZ( new_position );*/