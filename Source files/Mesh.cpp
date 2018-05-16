#include "..\Header files\Mesh.h"

//template< typename vertex_t >
//Mesh< vertex_t >::Mesh(  ) 

Mesh::Mesh() //: texture2D_dimensions( in_dimensions )
{
	mp_video_device = get_video_device();

	mp_video_device->GetImmediateContext( mp_video_device_context.ReleaseAndGetAddressOf() );

	create_buffer_matrix_world();	
}

void Mesh::create_buffer_vertices()
{
	//m_p_buffer_vertex.Reset();

	D3D11_BUFFER_DESC buffer_description { };

	// buffer size in bytes 
	buffer_description.ByteWidth	= sizeof( vertex_rgba_uv ) * static_cast< unsigned long >( m_vector_vertices.size() );
	// 48 bytes = struct 12 ( 3 floats * 4 bytes ) * verts 4

	buffer_description.Usage		= D3D11_USAGE_DEFAULT;
	buffer_description.BindFlags	= D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resource_data { };
	resource_data.pSysMem = m_vector_vertices.data();  // Pointer to the initialisation data.

	mul_total_vertices = static_cast< unsigned long >( m_vector_vertices.size() );

	mh_result = mp_video_device->CreateBuffer( & buffer_description , & resource_data , m_buffer_vertex.ReleaseAndGetAddressOf() );

	//catch D3D11 errors
	//if (DX11 error) throw ()
	if( FAILED( mh_result ) ) ErrorExit( L"Mesh error; create vertices buffer" );
}

// create_buffer( enum class { vertex , index , constant , shader , stream , render , depth } , const & data ) 
// passing address and pointer differences

void Mesh::create_buffer_indices()
{
	D3D11_BUFFER_DESC buffer_description { };
	buffer_description.ByteWidth	= sizeof( unsigned short ) * static_cast< unsigned long >( m_vector_indices.size() ); // buffer size in bytes
	buffer_description.Usage		= D3D11_USAGE_DEFAULT;
	buffer_description.BindFlags	= D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resource_data { };
	resource_data.pSysMem = m_vector_indices.data();  // Pointer to the initialisation data.

	mul_total_indices = static_cast< unsigned long >( m_vector_indices.size() ); // change to ull

	mh_result = mp_video_device->CreateBuffer( & buffer_description , & resource_data , & m_buffer_index );

	if( FAILED( mh_result ) ) ErrorExit( L"Mesh error; create indices buffer" );
}

void Mesh::create_buffer_matrix_world()
{
	m_buffer_world_description.ByteWidth				= sizeof( XMMATRIX );
	m_buffer_world_description.Usage					= D3D11_USAGE_DEFAULT;	//D3D11_USAGE_DYNAMIC;
	m_buffer_world_description.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	//m_struct_buffer_description.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	//m_struct_buffer_description.MiscFlags				= 0;
	//m_struct_buffer_description.StructureByteStride	= sizeof( XMMATRIX );

	// " D3D11_USAGE_DYNAMIC is typically used on resources with vertex data and on constant buffers.
	//   Use the ID3D11DeviceContext::Map and ID3D11DeviceContext::Unmap methods to write data to these resources. "

	// If the bind flag is D3D11_BIND_CONSTANT_BUFFER, you must set the ByteWidth value in multiples of 16, 
	// and less than or equal to D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT = 4096	


	mh_result = mp_video_device->CreateBuffer( & m_buffer_world_description,					// buffer description
											   nullptr,											// subresource data description
											   mp_buffer_matrix_world.ReleaseAndGetAddressOf() ); // ID3D11Buffer target

	VSSetConstantBuffers();

	if( FAILED( mh_result ) ) ErrorExit( L"Mesh error; create_buffer_matrix_world" );
}

void Mesh::update_matrix_world()
{
	m_matrix_world = m_matrix_scale * m_matrix_rotation * m_matrix_translation;

	m_matrix_world = XMMatrixTranspose( m_matrix_world );

	update_buffer_matrix_world();
}

void Mesh::update_buffer_matrix_world()
{
	//  ID3D11Buffer inherits from D3DResource 
	mp_video_device_context->UpdateSubresource( mp_buffer_matrix_world.Get() , // ID3D11Resource destination
												0 ,				// destination subresource
												nullptr ,		// destination subresource box to copy the resource data into
												& m_matrix_world ,// source data
												0 ,				// source data row size
												0 );			// source data depth slice.
}

// update_vertex_shader_buffer

void Mesh::VSSetConstantBuffers()
{
	// no need to re-bind the constant buffers unless the layout changes
	mp_video_device_context->VSSetConstantBuffers( 0,	//VS_BUFFER_MESH_WORLD ,// constant buffer index
												   1 ,	// buffer count
												   mp_buffer_matrix_world.GetAddressOf() ); // Array of constant buffers
}

void Mesh::update()
{
	update_matrix_world();
	//VSSetConstantBuffers();
	

	//update_buffer_matrix_world();
	//update_VS_buffer_matrix_world();

	
}

void Mesh::position( const XMFLOAT3 in_position )
{
	m_f3_position = in_position;
	m_matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );

	update_matrix_world();
};

void Mesh::translate_x( const float in_x )
{
	m_f3_position.x += in_x;
	m_matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );

	update_matrix_world();
}

void Mesh::translate_y( const float in_y )
{
	m_f3_position.y += in_y;
	m_matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );

	update_matrix_world();
}

void Mesh::translate_z( const float in_z )
{
	m_f3_position.y += in_z;
	m_matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );

	update_matrix_world();
}

//void Mesh::set_rotation( const XMFLOAT3 in_f3_radians ) { m_f3_rotation = in_f3_radians; }
//void Mesh::set_angle_x( const float in_angle_radians ) { m_f3_rotation.x = in_angle_radians; }
//void Mesh::set_angle_y( const float in_angle_radians ) { m_f3_rotation.y = in_angle_radians; }
//void Mesh::set_angle_z( const float in_angle_radians ) { m_f3_rotation.z = in_angle_radians; }

void Mesh::rotate_x( const float in_angle_radians ) 
{ 
	m_f3_rotation.x += in_angle_radians; 
	//matrix_rotation = XMMatrixRotationX( m_f3_rotation.x );
	m_matrix_rotation = XMMatrixRotationRollPitchYaw( m_f3_rotation.x , m_f3_rotation.y , m_f3_rotation.z );

	update_matrix_world();
}

void Mesh::rotate_y( const float in_angle_radians ) 
{ 
	m_f3_rotation.y += in_angle_radians; 
	m_matrix_rotation = XMMatrixRotationRollPitchYaw( m_f3_rotation.x , m_f3_rotation.y , m_f3_rotation.z );

	update_matrix_world();
}

void Mesh::rotate_z( const float in_angle_z ) 
{	
	debug_out( "\ndelta angle: %.20f" , m_f3_rotation.z);

	m_f3_rotation.z += in_angle_z;

	if( m_f3_rotation.z > 2 * M_PI ) m_f3_rotation.z = 0;

	m_matrix_rotation = XMMatrixRotationRollPitchYaw( m_f3_rotation.x , m_f3_rotation.y , m_f3_rotation.z );

	update_matrix_world();

	debug_out( "\ndelta angle: %.20f" , m_f3_rotation.z );
}

//void Mesh::rotate_axis( const XMVECTOR in_axis , const float in_radians ) {}

void Mesh::scale( const XMFLOAT3 in_scale )
{
	m_f3_scale = in_scale;
	m_matrix_scale = XMMatrixScaling( m_f3_scale.x , m_f3_scale.y , m_f3_scale.z );
	//m_matrix_world *= XMMatrixScaling( XMVectorGetX( in_scale_x ) , XMVectorGetY( in_scale_y ) , XMVectorGetZ( in_scale_z ) )
}

void Mesh::delta_scale( const XMFLOAT3 in_scale )
{
	m_f3_scale.x += in_scale.x;
	m_f3_scale.y += in_scale.y;
	m_f3_scale.z += in_scale.z;

	m_matrix_scale = XMMatrixScaling( m_f3_scale.x , m_f3_scale.y , m_f3_scale.z );
}

void Mesh::submit_draw()
{
	// void draw_point_list()
	//m_p_video_device_context->Draw( m_ul_total_vertices , 0 );	

	
}

void Mesh::render()
{
	mp_video_device_context->IASetPrimitiveTopology( m_primitive_topology );

	IASetVertexBuffers(); // update_VS_buffer_vertex
	IASetIndexBuffer();	 // update_VS_buffer_index

	VSSetConstantBuffers();

	//set_PS_resources();
	//update_VS_buffer();

	//submit_draw();	

	mp_video_device_context->DrawIndexed( mul_total_indices ,	// Number of index's to draw.
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