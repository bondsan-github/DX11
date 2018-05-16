#include "..\Header files\Camera.h"

//Camera::Camera( ){}

Camera::Camera( std::wstring name , XMVECTOR position , XMVECTOR target ) : m_v_position( position ) , m_v_target( target ) , m_string_name( name )
{

	m_p_video_device = get_video_device();

	m_p_video_device->GetImmediateContext( m_p_video_device_context.ReleaseAndGetAddressOf() );

	HRESULT h_result { E_FAIL };

	m_matrix_view = XMMatrixLookAtLH( position , target , m_v_up );
	m_matrix_view = XMMatrixTranspose( m_matrix_view );

	projection( Projection::perspective );
	
	m_buffer_description.ByteWidth				= sizeof( XMMATRIX );
	m_buffer_description.Usage					= D3D11_USAGE_DEFAULT;	//D3D11_USAGE_DYNAMIC;
	m_buffer_description.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	//m_buffer_description.CPUAccessFlags			= 0;//D3D11_CPU_ACCESS_WRITE;
	//m_struct_buffer_description.MiscFlags				= 0;
	//m_struct_buffer_description.StructureByteStride	= sizeof( XMMATRIX );

	// " D3D11_USAGE_DYNAMIC is typically used on resources with vertex data and on constant buffers.
	//   Use the ID3D11DeviceContext::Map and ID3D11DeviceContext::Unmap methods to write data to these resources. "

	// Pay attention to the size of the constant buffers, 
	// if they are not multiples of 16 you need to pad extra space on to the end
	// of them or the CreateBuffer function will fail.
	// In this case the constant buffer is 28 bytes with 4 bytes padding to make it 32.

	//----------------create buffer world----------------//
	h_result = m_p_video_device->CreateBuffer( & m_buffer_description ,	// buffer description
											   nullptr ,						// subresource data description
											   m_p_buffer_matrix_view.ReleaseAndGetAddressOf() ); // ID3D11Buffer target

	if( FAILED( h_result ) ) ErrorExit( L"Camera; CreateBuffer world" );


	//----------------create buffer projection---------------//
	h_result = m_p_video_device->CreateBuffer( & m_buffer_description ,	// buffer description
											   nullptr ,						// subresource data description
											   m_p_buffer_matrix_projection.ReleaseAndGetAddressOf() ); // ID3D11Buffer target	

	if( FAILED( h_result ) ) ErrorExit( L"Camera; CreateBuffer projection" );

	//----------------update VS buffer view----------------
	m_p_video_device_context->VSSetConstantBuffers( 1 ,//VS_BUFFER_CAMERA_VIEW , // Index into the device's zero-based array to begin setting constant buffers to
													1 ,	// Number of buffers to set
													m_p_buffer_matrix_view.GetAddressOf() ); // Array of constant buffers	

																							 //----------------update VS buffer projection----------------
	m_p_video_device_context->VSSetConstantBuffers( 2 ,//VS_BUFFER_CAMERA_PROJECTION , // Index into the device's zero-based array to begin setting constant buffers to
													1 ,	// Number of buffers to set
													m_p_buffer_matrix_projection.GetAddressOf() ); // Array of constant buffers	
}

Camera::~Camera() { }

void Camera::render()
{
	

	//----------------------update subresource----------------------// ie a buffer ( inherits from D3DResource )
	m_p_video_device_context->UpdateSubresource( m_p_buffer_matrix_view.Get() , // ID3D11Resource destination
												 0 ,				// zero-based index of destination subresource
												 nullptr ,			// box that defines the portion of the destination subresource to copy the resource data into
												 & m_matrix_view ,	// source data
												 0 ,				// size of one row of the source data.
												 0 );				// size of one depth slice of source data.

	//----------------------update subresource----------------------// ie a buffer ( inherits from D3DResource )
	m_p_video_device_context->UpdateSubresource( m_p_buffer_matrix_projection.Get() ,	// ID3D11Resource destination
												 0 ,									// zero-based index of destination subresource
												 nullptr ,								// box portion of destination subresource to copy the resource data into
												 & m_matrix_projection ,				// source data
												 0 ,									// size of one row of the source data.
												 0 );									// size of one depth slice of source data.

	
}

//void Camera::update_matrix_buffer_view(){}

void Camera::position( XMVECTOR v_position_new )
{
	m_v_position = v_position_new;

	m_matrix_view = XMMatrixLookAtLH( v_position_new , m_v_target , m_v_up );

	m_matrix_view = XMMatrixTranspose( m_matrix_view );
}

void Camera::z( float new_z )
{
	// can a position vector be derived from a view/position matrix?
	// XMMatrixDecompose
	
	m_v_position = XMVectorSetZ( m_v_position , new_z );

	m_matrix_view = XMMatrixLookAtLH( m_v_position , m_v_target , m_v_up );

	m_matrix_view = XMMatrixTranspose( m_matrix_view );
}

void Camera::delta_z( float delta_z )
{

}

void Camera::target( XMVECTOR target_new )
{
	m_v_target = target_new;

	m_matrix_view = XMMatrixLookAtLH( m_v_position , target_new , m_v_up );

	m_matrix_view = XMMatrixTranspose( m_matrix_view );
}

//XMMATRIX Camera::get_matrix_view() const { return m_matrix_view; }