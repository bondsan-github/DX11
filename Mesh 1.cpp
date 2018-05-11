#include "Mesh.h"

//template< typename vertex_t >
//Mesh< vertex_t >::Mesh(  ) 
Mesh::Mesh()
{
	mp_video_device = get_video_device();

	mp_video_device->GetImmediateContext( mp_video_device_context.ReleaseAndGetAddressOf() );

	create_buffer_matrix_world();
}

//template< typename vertex_t >
//Mesh< vertex_t >::~Mesh() { }
//Mesh::~Mesh() { }

//template< typename vertex_t >
//void Mesh< vertex_t >::create_buffer_vertices()
void Mesh::create_buffer_vertices()
{
	//m_p_buffer_vertex.Reset();

	D3D11_BUFFER_DESC buffer_description { };

	buffer_description.ByteWidth	= sizeof( vert_col_diff ) * static_cast< unsigned long >( m_vector_vertices.size() ); // buffer size in bytes 
	// 48 bytes = struct 12 ( 3 floats * 4 bytes ) * verts 4
	buffer_description.Usage		= D3D11_USAGE_DEFAULT;
	buffer_description.BindFlags	= D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resource_data { };
	resource_data.pSysMem = m_vector_vertices.data();  // Pointer to the initialisation data.

	mul_total_vertices = static_cast< unsigned long >( m_vector_vertices.size() );

	mh_result = mp_video_device->CreateBuffer( & buffer_description , & resource_data , mp_buffer_vertex.ReleaseAndGetAddressOf() );

	//catch D3D11 errors
	//if (DX11 error) throw ()
	if( FAILED( mh_result ) ) ErrorExit( L"Mesh error; create vertices buffer" );
}

//template< typename vertex_t >
//void Mesh< vertex_t >::create_buffer_indices() 
void Mesh::create_buffer_indices()
{
	D3D11_BUFFER_DESC buffer_description { };
	buffer_description.ByteWidth	= sizeof( unsigned short ) * static_cast< unsigned long >( m_vector_indices.size() ); // buffer size in bytes
	buffer_description.Usage		= D3D11_USAGE_DEFAULT;
	buffer_description.BindFlags	= D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA resource_data { };
	resource_data.pSysMem = m_vector_indices.data();  // Pointer to the initialisation data.

	mul_total_indices = static_cast< unsigned long >( m_vector_indices.size() ); // change to ull

	mh_result = mp_video_device->CreateBuffer( & buffer_description , & resource_data , & mp_buffer_index );

	if( FAILED( mh_result ) ) ErrorExit( L"Mesh error; create indices buffer" );
}

//template< typename vertex_t >
//void Mesh< vertex_t >::create_buffer_matrix_world()
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

	if( FAILED( mh_result ) ) ErrorExit( L"Mesh error; create_buffer_matrix_world" );
}

void Mesh::update_matrix_WVP()
{
	matrix_world = matrix_scale * matrix_rotation * matrix_translation;// *matrix_rotation_origin;

	matrix_world = XMMatrixTranspose( matrix_world );
}

//template< typename vertex_t >
//void Mesh< vertex_t >::update_buffer_matrix_world()
void Mesh::update_buffer_matrix_world()
{
	//matrix_local = matrix_scale * matrix_rotation * matrix_translation;
	//matrix_world = matrix_world_translation * matrix_world_rotation;

	//XMMATRIX matrix_final = matrix_local * matrix_world;
	
	// S * R * T
	//matrix_world = XMMatrixScaling( m_f3_scale.x , m_f3_scale.y , m_f3_scale.z );
	//matrix_world *= XMMatrixRotationRollPitchYaw( m_f3_rotation.x , m_f3_rotation.y , m_f3_rotation.z );
	//matrix_world *= XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );	
	
	// local_roate = s * r * t

	//matrix_local = matrix_scale;
	//matrix_local *= matrix_rotation;
	//matrix_local *= matrix_translation;
	//matrix_local *= matrix_rotation_origin;

	//update_matrix_WVP();

	// matrix * vector

	//  ID3D11Buffer inherits from D3DResource 
	mp_video_device_context->UpdateSubresource( mp_buffer_matrix_world.Get() , // ID3D11Resource destination
												0 ,				// zero-based index of destination subresource
												nullptr ,		// box that defines the portion of the destination subresource to copy the resource data into
												& matrix_world ,// source data
												0 ,				// size of one row of the source data.
												0 );			// size of one depth slice of source data.
}

//template< typename vertex_t >
//void Mesh< vertex_t >::update_VS_buffer_matrix_world()
void Mesh::update_VS_buffer_matrix_world()
{
	mp_video_device_context->VSSetConstantBuffers( 0, //VS_BUFFER_MESH ,// Index into the device's zero-based array to begin setting constant buffers to
												   1 ,	// Number of buffers to set
												   mp_buffer_matrix_world.GetAddressOf() ); // Array of constant buffers
}

void Mesh::update()
{
	update_matrix_WVP();

	update_buffer_matrix_world();
	update_VS_buffer_matrix_world();
}

/*
void Mesh::position( const XMFLOAT3 in_position )
{
	m_f3_position = in_position;
	matrix_translation = XMMatrixTranslation( in_position.x , in_position.y , in_position.z );
}
*/
/*void Mesh::z_pos_world( const float in_z )
{
	m_f3_position.z = in_z;
	matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );
}*/
/*
void Mesh::x_pos_world( const float in_x )
{
	m_f3_position.x = in_x;
	matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );
	//matrix_translation = XMMatrixTranspose( matrix_translation );

	//matrix_rotation = XMMatrixRotationZ( XMConvertToRadians( -90 ) );
	//matrix_rotation = XMMatrixTranspose( matrix_rotation );

	//matrix_local *= matrix_rotation;
	//matrix_local *= matrix_translation;
	

	//matrix_translation = XMMatrixTranslation( position.x + 100, position.y , position.z );
	//matrix_translation = XMMatrixTranspose( matrix_translation );

	//matrix_local *= matrix_translation;
	//matrix_local *= matrix_translation;
}
*/
/*
void Mesh::y_pos_world( const float in_y )
{
	m_f3_position.y = in_y;
	matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );
}
*/
void Mesh::translate_x( const float in_x )
{
	m_f3_position.x += in_x;
	matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );

	//matrix_local = matrix_scale * matrix_rotation * matrix_translation;
	//matrix_local = XMMatrixTranspose( matrix_local );
}
void Mesh::translate_y( const float in_y )
{
	m_f3_position.y += in_y;
	matrix_translation = XMMatrixTranslation( m_f3_position.x , m_f3_position.y , m_f3_position.z );
}

void Mesh::set_rotation( const XMFLOAT3 in_f3_radians )
{
	m_f3_rotation = in_f3_radians;
}

void Mesh::set_angle_x( const float in_angle_radians )
{
	m_f3_rotation.x = in_angle_radians;
}
void Mesh::set_angle_y( const float in_angle_radians )
{
	m_f3_rotation.y = in_angle_radians;
}
void Mesh::set_angle_z( const float in_angle_radians ) { m_f3_rotation.z = in_angle_radians; }
void Mesh::rotate_x( const float in_angle_radians ) { m_f3_rotation.x += in_angle_radians; }
void Mesh::rotate_y( const float in_angle_radians ) { m_f3_rotation.y += in_angle_radians; }
void Mesh::rotate_z( const float in_angle_z ) 
{	
	m_f3_rotation.z += in_angle_z;
	matrix_rotation = XMMatrixRotationZ( m_f3_rotation.z ); 
}
void Mesh::rotate_origin_z( const float in_radians )
{
	//rotation_axis_world = XMVectorSet( in_axis.x , in_axis.y , in_axis.z , 1.0f );

	//rotation_origin_angles.z += in_radians;
	//matrix_rotation_origin = XMMatrixRotationZ( rotation_origin_angles.z );

	matrix_world = matrix_scale * matrix_rotation * matrix_translation * matrix_rotation_origin;
	matrix_world = XMMatrixTranspose( matrix_world );
}
/*
void Mesh::rotate_point( XMFLOAT3 point , const float in_angle_radians )
{
	m_f3_rotation_world.z += in_angle_radians;
	
	//matrix_world_translation = XMMatrixTranslation( point.x , point.y , point.z );
	//matrix_world_rotation = XMMatrixRotationRollPitchYaw( 0.0f , 0.0f , m_f3_rotation_world.z );
		
	//matrix_world = XMMatrixRotationRollPitchYaw( m_f3_rotation_world.x , m_f3_rotation_world.y , m_f3_rotation_world.z );

	//XMVECTOR axis = XMVectorSet( 100.0 , 100.0 , 1.0 , 1.0f );
	//matrix_world = XMMatrixRotationAxis( axis , 20 );
}
*/

//template< typename vertex_t >
//void Mesh< vertex_t >::scale( const XMFLOAT3 in_scale )
void Mesh::set_scale( const XMFLOAT3 in_scale )
{
	m_f3_scale = in_scale;
	matrix_scale = XMMatrixScaling( m_f3_scale.x , m_f3_scale.y , m_f3_scale.z );
	//m_matrix_world *= XMMatrixScaling( XMVectorGetX( in_scale_x ) , XMVectorGetY( in_scale_y ) , XMVectorGetZ( in_scale_z ) )
	//m_matrix_world = XMMatrixTranspose( m_matrix_world );
}
void Mesh::delta_scale( const XMFLOAT3 in_scale )
{
	m_f3_scale.x += in_scale.x;
	m_f3_scale.y += in_scale.y;
	m_f3_scale.z += in_scale.z;

	matrix_scale = XMMatrixScaling( m_f3_scale.x , m_f3_scale.y , m_f3_scale.z );
}

//template< typename vertex_t >
//void Mesh< vertex_t >::create_diffuse_map( const wchar_t * const in_filename_diffuse_map )
void Mesh::create_diffuse_map( const char * const in_filename_diffuse_map )
// diffuse_map( filename )
{	
	// mesh.textures.load( maps::diffuse , wchar_t filename );
	// mesh.textures.create( = maps::diffuse, const D3D11_USAGE in_usage = D3D11_USAGE_DEFAULT , vector data = 0 );

	// mesh.textures.plot( maps::diffuse, const float norm_x, const float norm_y, colour );

	//m_p_image_diffuse = make_unique< Bitmap >( in_filename_diffuse_map );	
	//OR m_p_image_diffuse( new Image( in_filename_diffuse_map ) );
	
	//m_p_image_diffuse = make_unique< WICImage >( in_filename_diffuse_map );
	//m_image_diffuse.load_image( in_filename_diffuse_map );
	m_image_diffuse.load( in_filename_diffuse_map );

	//D3D11_BUFFER_DESC texture_description {};

	// render target texture
	m_texture2d_description.Width				= m_image_diffuse.width();
	m_texture2d_description.Height				= m_image_diffuse.height();
	m_texture2d_description.MipLevels			= 1;
	m_texture2d_description.ArraySize			= 1;
	m_texture2d_description.Format				= DXGI_FORMAT_R8G8B8A8_UNORM; // passing in an array of unsigned int's
	m_texture2d_description.SampleDesc.Count	= 1;
	m_texture2d_description.SampleDesc.Quality	= 0;
	m_texture2d_description.Usage				= D3D11_USAGE_IMMUTABLE; //D3D11_USAGE_DYNAMIC
	m_texture2d_description.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	m_texture2d_description.CPUAccessFlags		= 0; //D3D11_CPU_ACCESS_WRITE
	m_texture2d_description.MiscFlags			= 0;

	// 2D : You set SysMemPitch to the distance between any two adjacent pixels on different lines. 
	//		You set SysMemSlicePitch to the size of the entire 2D surface in bytes. 
	//		To access a specific pixel, you use: (const char*)pSysMem + SysMemPitch*y + (x * BytesPerPixel)

	m_subresource_data.pSysMem			= m_image_diffuse.vector_rgba()->data();
	m_subresource_data.SysMemPitch		= m_image_diffuse.width() * 4u; // map.bpp()
	//m_struct_subresource_data.SysMemSlicePitch	= static_cast< uint >( mp_image_diffuse->get_vector_image_data_uniform()->size() );
		
	//
	mh_result = mp_video_device->CreateTexture2D( & m_texture2d_description ,
												  & m_subresource_data ,	// initial data
												    mp_texture2D_diffuse.ReleaseAndGetAddressOf() );

	if( FAILED( mh_result ) ) ErrorExit( L"Quad_textured() error; CreateTexture2D" );

	
	m_view_description.Format						= DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_R8G8B8A8_UINT
	m_view_description.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	m_view_description.Texture2D.MostDetailedMip	= 0u;	// number of mips - 1;
	m_view_description.Texture2D.MipLevels			= 1u;

	//
	mh_result = mp_video_device->CreateShaderResourceView( mp_texture2D_diffuse.Get() ,
														   & m_view_description ,
														   mp_view_diffuse_map.ReleaseAndGetAddressOf() );

	if( FAILED( mh_result ) ) ErrorExit( L"Quad_textured() error; CreateShaderResourceView" );

	//catch D3D11 errors
	//if (DX11 error) throw ()

/*
#if defined(_DEBUG) || defined(DEBUG)
	m_p_texture2D_diffuse->SetPrivateData( WKPDID_D3DDebugObjectName ,
										   sizeof( "Mesh difuse map" ) - 1 ,
										   "Mesh difuse map" );
#endif
*/
}

//template< typename vertex_t >
//void Mesh< vertex_t >::render()
void Mesh::render()
{		
	// void set_topology( D3D11_PRIMITIVE_TOPOLOGY vertex_topology );
	//-------------set primitive topology-------------//
	mp_video_device_context->IASetPrimitiveTopology( m_primitive_topology );

	//unsigned int strides[ 1 ] { sizeof( vertex_t ) };
	//unsigned int offsets[ 1 ] { 0u };

	UINT stride = sizeof( vert_col_diff );
	UINT offset = 0u;
	
	// void set_vertex_buffer();
	mp_video_device_context->IASetVertexBuffers( 0 ,									// The first input slot for binding.
												  1 ,									// The number of vertex buffers in the array.
												  mp_buffer_vertex.GetAddressOf() ,	// A pointer to an array of vertex buffers 
												  & stride ,							// Pointer to an array of stride values
												  & offset );							// Pointer to an array of offset values

	// void set_index_buffer();
	mp_video_device_context->IASetIndexBuffer( mp_buffer_index.Get() ,// A pointer to an ID3D11Buffer object   
											    DXGI_FORMAT_R16_UINT ,	// 16-bit or 32-bit unsigned integers 
											    0 );					// Offset (in bytes) from the start of the index buffer to the first index to use.
	// short , unsigned short = 2 bytes = 8 * 2 = 16 bits
	// long  , unsigned long  = 4 bytes = 8 * 4 = 32 bits		



	// void set_diffuse_map( Microsoft::WRL::ComPtr< ID3D11Texture2D > diffuse_map );
	mp_video_device_context->PSSetShaderResources( 0 ,					// PS resource slot
													1 ,					// count of resources
												   mp_view_diffuse_map.GetAddressOf() );	// shader resource view

	// void draw_point_list()
	//m_p_video_device_context->Draw( m_ul_total_vertices , 0 );	

	// void draw_indexed()
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