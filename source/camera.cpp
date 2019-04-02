#include "Camera.h"

using namespace DirectX;


Camera::Camera( std::wstring in_name , XMVECTOR in_position , XMVECTOR in_target ) 
	: position( in_position ) , target( in_target ) , name( in_name )
{

	//video_device = get_video_device();

	Graphics_component::video_device->GetImmediateContext( & device_context_video );
	
	view_matrix = XMMatrixLookAtLH( position , target , up );
	view_matrix = XMMatrixTranspose( view_matrix );

	set_projection( Projection::perspective );
	
	buffer_description.ByteWidth						= sizeof( XMMATRIX );
	buffer_description.Usage							= D3D11_USAGE_DEFAULT;	//D3D11_USAGE_DYNAMIC; ***
	buffer_description.BindFlags						= D3D11_BIND_CONSTANT_BUFFER;
	//m_buffer_description.CPUAccessFlags				= 0;//D3D11_CPU_ACCESS_WRITE;
	//m_struct_buffer_description.MiscFlags				= 0;
	//m_struct_buffer_description.StructureByteStride	= sizeof( XMMATRIX );

	// ************************************************************************************************
	// " D3D11_USAGE_DYNAMIC is typically used on resources with vertex data and on constant buffers.
	//   Use the ID3D11DeviceContext::Map and ID3D11DeviceContext::Unmap methods to write data to these resources. "
	// ************************************************************************************************

	// Pay attention to the size of the constant buffers, 
	// if they are not multiples of 16 you need to pad extra space on to the end
	// of them or the CreateBuffer function will fail.
	// In this case the constant buffer is 28 bytes with 4 bytes padding to make it 32.

	//----------------create buffer world----------------//
	result = video_device->CreateBuffer( & buffer_description ,		// buffer description
										   nullptr ,				// subresource data description
										   & view_matrix_buffer );	// ID3D11Buffer target

	


	//----------------create buffer projection---------------//
	result = video_device->CreateBuffer( & buffer_description ,			// buffer description
										 nullptr ,						// subresource data description
										  & projection_matrix_buffer );	// ID3D11Buffer target	

	

	//----------------update VS buffer view----------------
	device_context_video->VSSetConstantBuffers( 1 , // VS_BUFFER_CAMERA_VIEW // Index into the device's zero-based array to begin setting constant buffers to
												1 ,	// Number of buffers to set
												view_matrix_buffer.GetAddressOf() ); // Array of constant buffers	

	//----------------update VS buffer projection----------------
	device_context_video->VSSetConstantBuffers( 2 ,//VS_BUFFER_CAMERA_PROJECTION , // Index into the device's zero-based array to begin setting constant buffers to
												1 ,	// Number of buffers to set
												projection_matrix_buffer.GetAddressOf() ); // Array of constant buffers	
}

Camera::~Camera() { }

void Camera::update()
{
	//----------------------update view matrix buffer()----------------------// ie a buffer ( inherits from D3DResource )
	device_context_video->UpdateSubresource( view_matrix_buffer.Get() , // ID3D11Resource destination
											 0 ,						// zero-based index of destination subresource
											 nullptr ,					// box that defines the portion of the destination subresource to copy the resource data into
											 & view_matrix ,			// source data
											 0 ,						// size of one row of the source data.
											 0 );						// size of one depth slice of source data.

	//----------------------update projection_matrix_buffer----------------------// ie a buffer ( inherits from D3DResource )
	device_context_video->UpdateSubresource( projection_matrix_buffer.Get() ,	// ID3D11Resource destination
											 0 ,								// zero-based index of destination subresource
											 nullptr ,							// box portion of destination subresource to copy the resource data into
											 & projection_matrix ,				// source data
											 0 ,								// size of one row of the source data.
											 0 );								// size of one depth slice of source data.
}

//void Camera::update_matrix_buffer_view(){}

void Camera::set_position( XMVECTOR in_position )
{
	position = in_position;

	view_matrix = XMMatrixLookAtLH( in_position , target , up );

	view_matrix = XMMatrixTranspose( view_matrix );
}

void Camera::set_z( float new_z )
{
	// can a position vector be derived from a view/position matrix?
	// XMMatrixDecompose
	
	position = XMVectorSetZ( position , new_z );

	view_matrix = XMMatrixLookAtLH( position , target , up );

	view_matrix = XMMatrixTranspose( view_matrix );
}

void Camera::set_delta_z( float delta_z )
{
}

void Camera::set_target( XMVECTOR target_new )
{
	target = target_new;

	view_matrix = XMMatrixLookAtLH( position , target_new , up );

	view_matrix = XMMatrixTranspose( view_matrix );
}

//XMMATRIX Camera::get_matrix_view() const { return m_matrix_view; }

/*
if( FAILED( h_result ) ) ErrorExit( L"Camera; CreateBuffer world" );
if( FAILED( h_result ) ) ErrorExit( L"Camera; CreateBuffer projection" );
*/