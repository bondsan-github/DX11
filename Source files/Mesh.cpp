#include "Mesh.h"

//template< typename vertex_t >
//Mesh< vertex_t >::Mesh(  ) 

Mesh::Mesh() //: texture2D_dimensions( in_dimensions )
{
	video_device = get_video_device();

	video_device->GetImmediateContext( video_device_context.ReleaseAndGetAddressOf() );

	create_buffer_matrix_world();	
}

void Mesh::create_buffer_vertices()
{
	//m_p_buffer_vertex.Reset();

	D3D11_BUFFER_DESC buffer_description { };

	// buffer size in bytes 
	buffer_description.ByteWidth	= sizeof( vertex_rgba_uv ) * static_cast< unsigned long >( vertices.size() );
	// 48 bytes = struct 12 ( 3 floats * 4 bytes ) * verts 4

	buffer_description.Usage		= D3D11_USAGE_DEFAULT;
	buffer_description.BindFlags	= D3D11_BIND_VERTEX_BUFFER;

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

// create_buffer( enum class { vertex , index , constant , shader , stream , render , depth } , const & data ) 
// passing address and pointer differences

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

	UINT stride = sizeof( vertex_rgba_uv );
	UINT offset = 0u;

	video_device_context->IASetVertexBuffers( 0 ,								// first input slot for binding.
											  1 ,								// number of vertex buffers in the array.
											  vertex_buffer.GetAddressOf() ,	// pointer to vertex buffer array
											  & stride ,						// pointer to stride values array
											  & offset );						// pointer to offset values
}

void Mesh::update_world_matrix()
{
	world_matrix = scale_matrix * rotation_matrix * translation_matrix;

	world_matrix = XMMatrixTranspose( world_matrix );
}

void Mesh::update_world_matrix_buffer()
{
	//  ID3D11Buffer inherits from D3DResource 
	video_device_context->UpdateSubresource( world_matrix_buffer.Get() ,	// ID3D11Resource destination
											 0 ,							// destination subresource
											 nullptr ,						// destination subresource box to copy the resource data into
											 &world_matrix ,				// source data
											 0 ,							// source data row size
											 0 );							// source data depth slice.
}

void Mesh::set_VS_constant_buffers()
{
	// no need to re-bind the constant buffers unless the layout changes
	video_device_context->VSSetConstantBuffers( 0,	//VS_BUFFER_MESH_WORLD ,// constant buffer index
												1 ,	// buffer count
												world_matrix_buffer.GetAddressOf() ); // Array of constant buffers
}

void Mesh::update()
{
	//update_matrix_world_buffer();
}

void Mesh::set_position( const XMFLOAT3 in_position )
{
	position = in_position;
	translation_matrix = XMMatrixTranslation( position.x , position.y , position.z );

	update_world_matrix();
};

void Mesh::set_position( const XMVECTOR in_position )
{
	XMStoreFloat3( & position , in_position );
	translation_matrix = XMMatrixTranslationFromVector( in_position );

	update_world_matrix();
}

void Mesh::set_position( const float in_x , const float in_y , const float in_z )
{
	position = XMFLOAT3( in_x , in_y , in_z );
	translation_matrix = XMMatrixTranslation( position.x , position.y , position.z );

	update_world_matrix();
}

void Mesh::translate_x( const float in_x )
{
	position.x += in_x;
	translation_matrix = XMMatrixTranslation( position.x , position.y , position.z );

	update_world_matrix();
}

void Mesh::translate_y( const float in_y )
{
	position.y += in_y;
	translation_matrix = XMMatrixTranslation( position.x , position.y , position.z );

	update_world_matrix();
}

void Mesh::translate_z( const float in_z )
{
	position.y += in_z;
	translation_matrix = XMMatrixTranslation( position.x , position.y , position.z );

	update_world_matrix();
}

//void Mesh::set_rotation( const XMFLOAT3 in_f3_radians ) { m_f3_rotation = in_f3_radians; }
//void Mesh::set_angle_x( const float in_angle_radians ) { m_f3_rotation.x = in_angle_radians; }
//void Mesh::set_angle_y( const float in_angle_radians ) { m_f3_rotation.y = in_angle_radians; }
//void Mesh::set_angle_z( const float in_angle_radians ) { m_f3_rotation.z = in_angle_radians; }

void Mesh::rotate_x( const float in_angle_radians ) 
{ 
	rotation.x += in_angle_radians; 
	//matrix_rotation = XMMatrixRotationX( m_f3_rotation.x );
	rotation_matrix = XMMatrixRotationRollPitchYaw( rotation.x , rotation.y , rotation.z );

	update_world_matrix();
}

void Mesh::rotate_y( const float in_angle_radians ) 
{ 
	rotation.y += in_angle_radians;
	rotation_matrix = XMMatrixRotationRollPitchYaw( rotation.x , rotation.y , rotation.z );

	update_world_matrix();
}

void Mesh::rotate_z( const float in_angle_z ) 
{	
	//debug_out( "\ndelta angle: %.20f" , rotation.z);

	rotation.z += in_angle_z;

	if( rotation.z > 2 * 3.14159265358979323846f ) rotation.z = 0;

	rotation_matrix = XMMatrixRotationRollPitchYaw( rotation.x , rotation.y , rotation.z );

	update_world_matrix();

	//debug_out( "\ndelta angle: %.20f" , rotation.z );
}

void Mesh::rotate_point_z( const XMFLOAT3 in_point , const float in_angle_z )
{
	// capture current position
	// matrix: translate - ( point to origin vector )
	// matrix: rotate z
	// matrix: translate + ( point to origin vector )
	
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

	XMFLOAT3 point_to_origin_vector = position;
	// position - point // add -/+/*/*= etc to XMFLOAT* - ? friend class

	point_to_origin_vector = XMFLOAT3( position.x - in_point.x , position.y - in_point.y , position.z - in_point.z );

	// move mesh to origin by point vector
	///XMMATRIX move_to_origin = XMMatrixTranslation( - in_point.x , - in_point.y , - in_point.z );
	XMMATRIX move_to_origin = XMMatrixTranslation( point_to_origin_vector.x , point_to_origin_vector.y , point_to_origin_vector.z );
	
	////set_position( -in_point.x , -in_point.y , -in_point.z );

	rotation.z += in_angle_z;
	XMMATRIX rotate = XMMatrixRotationZ( rotation.z );
	////rotate_z( in_angle_z );
	
	// move/translate back to previous poistion
	// position + point

	XMMATRIX previous_position = XMMatrixTranslation( +point_to_origin_vector.x ,  +point_to_origin_vector.y ,  +point_to_origin_vector.z );
	///set_position( + in_point.x , + in_point.y , + in_point.z );

	world_matrix = move_to_origin * rotate * previous_position;

	// find new position/centre
	XMVECTOR new_position {};
	XMVECTOR scale {};
	XMVECTOR rotation {};

	XMMatrixDecompose( & scale , & rotation , & new_position , world_matrix );

	position.x = XMVectorGetX( new_position );
	position.y = XMVectorGetY( new_position );
	position.z = XMVectorGetZ( new_position );
	
	world_matrix = XMMatrixTranspose( world_matrix );

	set_position( position );
}

void Mesh::set_scale( const XMFLOAT3 in_scale )
{
	scale = in_scale;
	scale_matrix = XMMatrixScaling( scale.x , scale.y , scale.z );
	//m_matrix_world *= XMMatrixScaling( XMVectorGetX( in_scale_x ) , XMVectorGetY( in_scale_y ) , XMVectorGetZ( in_scale_z ) )

	update_world_matrix();
}

void Mesh::delta_scale( const XMFLOAT3 in_scale )
{
	scale.x += in_scale.x;
	scale.y += in_scale.y;
	scale.z += in_scale.z;

	scale_matrix = XMMatrixScaling( scale.x , scale.y , scale.z );

	update_world_matrix();
}

//void Mesh::submit_draw() { // void draw_point_list() //m_p_video_device_context->Draw( m_ul_total_vertices , 0 ); }

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