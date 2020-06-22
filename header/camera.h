#pragma once

#include <string>
#include <wrl/client.h>
#include <d3d11.h>

#include <DirectXMath.h>

#include "debugging.h"
#include "Graphics_component.h"
#include "types.h"


using std::wstring;
using Microsoft::WRL::ComPtr;

using DirectX::XMVECTOR;
using DirectX::XMFLOAT4;
using DirectX::XMVectorSet;
using DirectX::XMFLOAT4X4;
using DirectX::XMMATRIX;
using DirectX::XMMatrixPerspectiveFovLH;
using DirectX::XMMatrixOrthographicLH;
using DirectX::XM_PIDIV2;

enum class Projection { perspective , orthographic };

class Camera : public Graphics_component
{
	private:

		HRESULT result{ E_FAIL };

		//ComPtr< ID3D11Device >			video_device;
		ComPtr< ID3D11DeviceContext >	device_context_video;

		wstring name;

		XMVECTOR	position	= XMVectorSet( 0.0f , 0.0f , -1.0f , 0.0f );
		XMVECTOR	target		= XMVectorSet( 0.0f , 0.0f , 0.0f , 0.0f );
		XMVECTOR	up			= XMVectorSet( 0.0f , 1.0f , 0.0f , 0.0f );

		float	distance		= 0.0f;
		float	distance_min	= 0.0f;
		float	distance_max	= 0.0f;

		float	rotation_x		= 0.0f;
		float	rotation_y		= 0.0f;

		float	y_min			= 0.0f;
		float	x_min			= 0.0f;

		XMMATRIX				view_matrix {};
		XMMATRIX				projection_matrix {};

		Projection				projection_method = Projection::perspective;

		D3D11_BUFFER_DESC		buffer_description{};

		ComPtr< ID3D11Buffer >	view_matrix_buffer;
		ComPtr< ID3D11Buffer >	projection_matrix_buffer;

	public:
		
		//Camera() {}

		Camera( wstring name		= L"main",
				XMVECTOR position	= XMVectorSet( 0.0f , 0.0f , -1.0f , 0.0f ) ,
				XMVECTOR target		= XMVectorSet( 0.0f , 0.0f ,  0.0f , 0.0f ) );

		~Camera();	

		void set_projection( const Projection in_projection = Projection::perspective )
		{
			D3D11_VIEWPORT viewport;
			UINT           number_of_viewports = 1u;	

			device_context_video->RSGetViewports( & number_of_viewports , & viewport );

			XMFLOAT4X4 matrix_projection {};

			if( in_projection == Projection::perspective )
			{
				projection_matrix = XMMatrixPerspectiveFovLH( XM_PIDIV2 , // FovAngleY
																viewport.Width / viewport.Height , // ** Aspect_Height_by_Width
																0.01f ,		// near Z
																10000.0f ); // far Z
			}
			else
			{
				projection_matrix = XMMatrixOrthographicLH( viewport.Width , viewport.Height , 0.001f , 1000.0f );

				//m_matrix_projection = XMMatrixOrthographicLH( 200 , 200 , 0.01f , 100.0f );
				//m_matrix_projection = m_matrix_view;							
			}

			XMMATRIX projection_orthographic(
				0 , 0 , 0 , 0 ,
				0 , 0 , 0 , 0 ,
				0 , 0 , 0 , 0 ,
				0 , 0 , 0 , 1 );

			projection_matrix = XMMatrixTranspose( projection_matrix );
			//matrix_projection = projection_orthographic;
		}
		
		void set_position( const XMVECTOR position );
		void set_target( const XMVECTOR target );

		void set_z( const float new_z );
		void set_delta_z( const float delta_z );

		// set_projection_matrix( XMMATRIX )

		//void roll  
		//void pitch
		// void target( Mesh * )

		// update(event);
		void update();
};