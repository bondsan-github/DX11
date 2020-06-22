#include "mesh.h"

//template< typename vertex_t >
//Mesh< vertex_t >::Mesh(  ) 

using namespace DirectX;

Mesh::Mesh() //: texture2D_dimensions( in_dimensions )
{
	//video_device = get_video_device();

	video_device->GetImmediateContext( video_device_context.ReleaseAndGetAddressOf() ); //GetAddressOf()

	create_buffer_matrix_world();	
}

void Mesh::create_buffer_vertices()
{
	//m_p_buffer_vertex.Reset();

	D3D11_BUFFER_DESC buffer_description { };

	// buffer size in bytes 
	buffer_description.ByteWidth	= sizeof( vertex_uv ) * static_cast< unsigned long >( vertices.size() );
	// 48 bytes = struct 12 ( 3 floats * 4 bytes ) * verts 4

	buffer_description.Usage		= D3D11_USAGE_DEFAULT; //D3D11_USAGE_DYNAMIC;// 
	buffer_description.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
	//buffer_description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;


	D3D11_SUBRESOURCE_DATA resource_data { };
	resource_data.pSysMem = vertices.data();  // Pointer to the initialisation data.

	vertices_count = static_cast< unsigned long >( vertices.size() );

	result = video_device->CreateBuffer( & buffer_description ,
										 & resource_data ,
										 & vertex_buffer );

	//catch D3D11 errors
	//if (DX11 error) throw ()
	if( FAILED( result ) ) ErrorExit( L"Mesh error; create vertices buffer" );
}

// create_buffer( enum class { vertex , index , constant , shader , stream , render , depth } , const<type> * data ) 

void Mesh::create_buffer_indices()
{
	D3D11_BUFFER_DESC buffer_description { };
	buffer_description.ByteWidth	= sizeof( unsigned short ) * static_cast< unsigned long >( indices.size() ); // buffer size in bytes
	buffer_description.Usage		= D3D11_USAGE_DEFAULT;
	buffer_description.BindFlags	= D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resource_data { };
	resource_data.pSysMem = indices.data();  // Pointer to the initialisation data.

	indices_count = static_cast< unsigned long >( indices.size() ); // change to ull

	result = video_device->CreateBuffer( & buffer_description , & resource_data , & index_buffer );

	if( FAILED( result ) ) ErrorExit( L"Mesh error; create indices buffer" );
}

void Mesh::create_buffer_matrix_world()
{
	world_buffer_description.ByteWidth				= sizeof( XMMATRIX );
	world_buffer_description.Usage					= D3D11_USAGE_DEFAULT;	//D3D11_USAGE_DYNAMIC;
	world_buffer_description.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	//world_buffer_description.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	//world_buffer_description.MiscFlags			= 0;
	//world_buffer_description.StructureByteStride	= sizeof( XMMATRIX );

	// " D3D11_USAGE_DYNAMIC is typically used on resources with vertex data and on constant buffers.
	//   Use the ID3D11DeviceContext::Map and ID3D11DeviceContext::Unmap methods to write data to these resources. "

	// If the bind flag is D3D11_BIND_CONSTANT_BUFFER, you must set the ByteWidth value in multiples of 16, 
	// and less than or equal to D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT = 4096	


	result = video_device->CreateBuffer( & world_buffer_description ,	// buffer description
										 nullptr,						// subresource data description
										 & world_matrix_buffer );		// ID3D11Buffer target

	if( FAILED( result ) ) ErrorExit( L"Mesh error; create_buffer_matrix_world" );
}

void Mesh::set_IA_vertex_buffers()
{
	//unsigned int strides[ 1 ] { sizeof( vertex_t ) };
	//unsigned int offsets[ 1 ] { 0u };

	UINT stride = sizeof( vertex_uv );
	UINT offset = 0u;

	video_device_context->IASetVertexBuffers( 0 ,								// first input slot for binding.
											  1 ,								// number of vertex buffers in the array.
											  vertex_buffer.GetAddressOf() ,	// pointer to vertex buffer array
											  & stride ,						// pointer to stride values array
											  & offset );						// pointer to offset values
}

void Mesh::update_world_matrix()
{
	world_matrix = scale_matrix * rotation_matrix * translation_matrix * orbit_matrix;

	world_matrix = XMMatrixTranspose( world_matrix );
}

void Mesh::update_world_matrix_buffer()
{
	//  ID3D11Buffer inherits from D3DResource 
	video_device_context->UpdateSubresource( world_matrix_buffer.Get() ,	// ID3D11Resource destination
											 0 ,							// destination subresource
											 nullptr ,						// destination subresource box to copy the resource data into
											 & world_matrix ,				// source data
											 0 ,							// source data row size
											 0 );							// source data depth slice.
}

void Mesh::set_VS_constant_buffers()
{
	// no need to re-bind the constant buffers unless the layout changes
	video_device_context->VSSetConstantBuffers( 0,	// VS_BUFFER_WORLD ,// constant buffer index  // Buffers 
												1 ,	// buffer count
												world_matrix_buffer.GetAddressOf() ); // Array of constant buffers
}

void Mesh::update( double time_delta )
{
	//update_matrix_world_buffer();
}

void Mesh::set_position( const XMFLOAT3 in_position )
{
	translation_matrix = XMMatrixTranslation( in_position.x , in_position.y , in_position.z );

	update_world_matrix();
};

void Mesh::set_position( const XMVECTOR in_position )
{
	translation_matrix = XMMatrixTranslationFromVector( in_position );

	update_world_matrix();
}

void Mesh::set_position( const float in_x , const float in_y , const float in_z )
{
	translation_matrix = XMMatrixTranslation( in_x , in_y , in_z );

	update_world_matrix();
}

const XMFLOAT3 Mesh::get_position( void ) const
{
	float x , y , z = 0.0f;

	XMMATRIX temp_matrix = XMMatrixTranspose( world_matrix );

	x = XMVectorGetX( temp_matrix.r[ 3 ] );
	y = XMVectorGetY( temp_matrix.r[ 3 ] );
	z = XMVectorGetZ( temp_matrix.r[ 3 ] );

	XMFLOAT3 position( x , y , z );

	return position;
}

void Mesh::translate_x( const float in_x ) // translate_local_z
{
	// "XMLoadFloat3 = The w member of the returned XMVECTOR is initialized to 0."
	translation_matrix *= XMMatrixTranslationFromVector( XMLoadFloat3( & XMFLOAT3( in_x , 0.0f , 0.0 ) ) );

	update_world_matrix();
}

void Mesh::translate_y( const float in_y )
{	
	translation_matrix *= XMMatrixTranslationFromVector( XMLoadFloat3( & XMFLOAT3( 0.0f , in_y , 0.0 ) ) );

	update_world_matrix();
}

void Mesh::translate_z( const float in_z )
{
	translation_matrix *= XMMatrixTranslationFromVector( XMLoadFloat3( & XMFLOAT3( 0.0f , 0.0f , in_z ) ) );

	update_world_matrix();
}

// !!! to test
void Mesh::set_rotation( const XMFLOAT3 in_radians ) 
{ 
	rotation_matrix = XMMatrixRotationRollPitchYawFromVector( XMLoadFloat3( & XMFLOAT3( in_radians.x , in_radians.y , in_radians.z ) ) );

	update_world_matrix();
}

void Mesh::rotate_x( const float in_radians ) 
{ 
	// Angles are measured clockwise when looking along the rotation axis toward the origin.
	rotation_matrix *= XMMatrixRotationX( in_radians );

	update_world_matrix();
}

void Mesh::rotate_y( const float in_radians )
{ 
	rotation_matrix *= XMMatrixRotationY( in_radians );

	update_world_matrix();
}

void Mesh::rotation_z( const float in_radians ) 
{	
	rotation_matrix *= XMMatrixRotationZ( in_radians );

	update_world_matrix();
}

void Mesh::orbit_z( const float in_radians_z )
{
	orbit_matrix *= XMMatrixRotationZ( in_radians_z );

	update_world_matrix();
}

void Mesh::orbit_point_z( const XMVECTOR in_point , const float in_angle_z ) {}

void Mesh::rotation_z_about_point( const XMFLOAT3 in_point , const float in_angle_z )
{
	world_matrix = XMMatrixTranspose( world_matrix );

	world_matrix.r[3] -= XMVectorSet( in_point.x , in_point.y , in_point.z, 0 );

	world_matrix *= XMMatrixRotationZ( in_angle_z );

	world_matrix.r[ 3 ] += XMVectorSet( in_point.x , in_point.y , in_point.z , 0 );

	translation_matrix.r[ 3 ] = world_matrix.r[ 3 ];

	rotation_matrix.r[ 0 ] = world_matrix.r[ 0 ];
	rotation_matrix.r[ 1 ] = world_matrix.r[ 1 ];
	rotation_matrix.r[ 2 ] = world_matrix.r[ 2 ];
	
	world_matrix = XMMatrixTranspose( world_matrix );

	update_world_matrix_buffer();
}

/*

-|----(axis_x)-----(ox)-----

If you rotate point( px , py ) around axis( axis_x , axis_y ) by angle theta you'll get:

p'x = cos(theta) * (p_x - axis_x) - sin(theta) * (p_y - axis_y) + axis_x
p'y = sin(theta) * (px - axis_x) + cos(theta) * (py - axis_y) + axis_y
*/


// One revolution is equal to 2π

// input = radians ( per ms or seconds )

// tangential speed v = rotational speed ω * radial distance r
// v = rω

// r = pivot - position
// ω = 1 rotation (2π) per second

// full circle S = 2πr
// theta θ = S / r

// 1s / 2π  = 0.1591549437
//float full_rotation_1s = 1.0f / ( 2.0f * 3.1415926535f );// M_PI);

void Mesh::set_scale( const XMFLOAT3 in_scale )
{
	scale_matrix = XMMatrixScaling( in_scale.x , in_scale.y , in_scale.z );

	update_world_matrix();
}

void Mesh::delta_scale( const XMFLOAT3 in_scale )
{
	scale_matrix *= XMMatrixScaling( in_scale.x , in_scale.y , in_scale.z );

	update_world_matrix();
}

//void Mesh::submit_draw() { // void draw_point_list() //m_p_video_device_context->Draw( m_ul_total_vertices , 0 ); }

vector< vertex_uv > Mesh::get_world_vertices() const
{
	vector< vertex_uv > world_vertices;
	vertex_uv world_transformed;
	XMMATRIX world_matrix_transposed = XMMatrixTranspose( world_matrix );

	//XMFLOAT3 test_vertex = vertices.at( 0 ).point;
	//debug_out( "\nobject.x = %.2f , y = %.2f" , test_vertex.x , test_vertex.y );

	for( const auto & vertex : vertices )
	{
		XMVECTOR vertex_world = XMVector3Transform( XMLoadFloat3( &XMFLOAT3( vertex.point.x , vertex.point.y , vertex.point.z ) ) , world_matrix_transposed );

		world_transformed.point.x = XMVectorGetX( vertex_world );
		world_transformed.point.y = XMVectorGetY( vertex_world );
		world_transformed.point.z = XMVectorGetZ( vertex_world );

		world_vertices.push_back( world_transformed );
	}

	return world_vertices;
}

Bounding_box Mesh::get_bounding_box() const // (world coordinates)
{
	Bounding_box box;

	vector< vertex_uv > world_vertices = get_world_vertices();

	box.min = XMFLOAT3(  FLT_MAX ,  FLT_MAX ,  FLT_MAX );
	box.max = XMFLOAT3( -FLT_MAX , -FLT_MAX , -FLT_MAX );

	for( const auto & vertex : world_vertices )
	{
		//box.min = minimum( block_box.min , tetri_box.min );	// always returns a value
		//box.max = maximum( block_box.max , tetri_box.max );

		if( vertex.point.x < box.min.x ) box.min.x = vertex.point.x;
		if( vertex.point.x > box.max.x ) box.max.x = vertex.point.x;

		if( vertex.point.y < box.min.y ) box.min.y = vertex.point.y;
		if( vertex.point.y > box.max.y ) box.max.y = vertex.point.y;

		if( vertex.point.z < box.min.z ) box.min.z = vertex.point.z;
		if( vertex.point.z > box.max.z ) box.max.z = vertex.point.z;
	}

	return box;
}

void Mesh::render()
{
	//render();

	video_device_context->IASetPrimitiveTopology( primitive_topology );

	update_world_matrix_buffer();

	set_IA_vertex_buffers();
	set_IA_index_buffer();

	set_VS_constant_buffers();

	//set_PS_resources();
	//update_VS_buffer();
	//submit_draw();	

	video_device_context->DrawIndexed( indices_count ,	// Number of index's to draw.
									   0 ,					// The location of the first index read by the GPU from the index buffer
									   0 );					// A value added to each index before reading a vertex from the vertex buffer
}

/*
void Mesh< vertex_t >::update()
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR( md3dImmediateContext->Map( mWavesVB , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &mappedData ) );

	Vertex* v = reinterpret_cast< Vertex* >( mappedData.pData );
	for( UINT i = 0; i < mWaves.VertexCount(); ++i )
	{
		v[ i ].Pos = mWaves[ i ];
		v[ i ].Color = XMFLOAT4( 0.0f , 0.0f , 0.0f , 1.0f );
	}

	md3dImmediateContext->Unmap( mWavesVB , 0 );
}
*/

// orbit_and_rotate_point_z

//void Mesh::orbit_point_z( const XMFLOAT3 in_point , const float in_angle_z )
//{
//	XMVECTOR scale =	XMLoadFloat3( & XMFLOAT3( 1.0f , 1.0f , 1.0f ) );	
//	XMVECTOR origin =	XMLoadFloat3( & XMFLOAT3( in_point.x , in_point.y , in_point.z ) );
//	XMVECTOR axis_z =	XMLoadFloat4( & XMFLOAT4( 0.0f , 0.0f , 1.0f , 0.0f ) ); // Z-axis
//
//	XMVECTOR rotation_quaternion = XMQuaternionRotationAxis( axis_z , in_angle_z ); //XMMatrixRotationQuaternion
//
//	//XMVECTOR translation = XMLoadFloat3( & XMFLOAT3( in_translation.x , in_translation.y , in_translation.z ) );
//	XMVECTOR translation = XMLoadFloat3( & XMFLOAT3( 0 , 0 , 0 ) );
//
//	world_matrix = XMMatrixTranspose( world_matrix );
//
//	world_matrix *= XMMatrixAffineTransformation(
//		scale ,
//		origin ,
//		rotation_quaternion ,
//		translation	);
//
//	world_matrix = XMMatrixTranspose( world_matrix );
//
//	update_world_matrix_buffer();
//	//update_world_matrix();
//}

//void Mesh::orbit_point_z( const XMFLOAT3 in_point , const float in_angle_z )
//{
//	XMVECTOR Scaling =				XMLoadFloat3( & XMFLOAT3( 0 , 0 , 0 ) );
//	XMVECTOR RotationOrigin =		XMLoadFloat3( & XMFLOAT3( 0 , 0 , 0 ) );
//	XMVECTOR RotationQuaternion =	XMLoadFloat4( & XMFLOAT4( 0 , 0 , 0 , 0 ) );
//	XMVECTOR Translation =			XMLoadFloat3( & XMFLOAT3( 0 , 0 , 0 ) );
//
//	// M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;
//
//	XMMATRIX MScaling = XMMatrixScalingFromVector( Scaling );
//	XMVECTOR VRotationOrigin = XMVectorSelect( g_XMSelect1110.v , RotationOrigin , g_XMSelect1110.v ); // select x,y,z only from RotationOrigin
//	XMMATRIX MRotation = XMMatrixRotationQuaternion( RotationQuaternion );
//	XMVECTOR VTranslation = XMVectorSelect( g_XMSelect1110.v , Translation , g_XMSelect1110.v );// select x,y,z only from Translation
//
//	XMMATRIX M;
//	M =			MScaling;
//	M.r[ 3 ] =	XMVectorSubtract( M.r[ 3 ] , VRotationOrigin );	// position -= VRotationOrigin
//	M =			XMMatrixMultiply( M , MRotation );				// M *= MRotation
//	M.r[ 3 ] =	XMVectorAdd( M.r[ 3 ] , VRotationOrigin );		// position += VRotationOrigin
//	M.r[ 3 ] =	XMVectorAdd( M.r[ 3 ] , VTranslation );			// ( position += VTranslation )
//	return M;
//}