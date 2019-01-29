#pragma once

#include <string>
#include <wrl/client.h>
#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;  // for DirectXMath.h

#include "debugging.h"
#include "Drawable.h"
#include "types.h"

using std::wstring;
using Microsoft::WRL::ComPtr;

enum class Projection { perspective , orthographic };

class Camera : public Drawable
{
	public:
		//Camera() = delete;

		Camera( wstring name	= L"main",
				XMVECTOR position	= XMVectorSet( 0.0f , 0.0f , -1.0f , 0.0f ) ,
				XMVECTOR target		= XMVectorSet( 0.0f , 0.0f ,  0.0f , 0.0f ) );

		~Camera();	

		void projection( const Projection in_projection = Projection::perspective )
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
			//m_matrix_projection = projection_orthographic;
		}
		
		void position( const XMVECTOR position );
		void target( const XMVECTOR target );

		void z( const float new_z );
		void delta_z( const float delta_z );

		// set_projection_matrix( XMMATRIX )

		//void roll  
		//void pitch
		// void target( Mesh * )

		// update(event);
		void render();		

	private:

		ComPtr< ID3D11Device >			video_device;
		ComPtr< ID3D11DeviceContext >	device_context_video;

		wstring m_string_name = L"camera_default";
		
		XMVECTOR	m_v_position	= XMVectorSet(  0.0f ,  0.0f ,  -1.0f ,  0.0f );
		XMVECTOR	m_v_target		= XMVectorSet(  0.0f ,  0.0f ,   0.0f ,  0.0f );
		XMVECTOR	m_v_up			= XMVectorSet(  0.0f ,  1.0f ,   0.0f ,  0.0f );

		float	m_f_distance		= 0.0f;
		float	m_f_distance_min	= 0.0f;
		float	m_f_distance_max	= 0.0f;

		float	m_f_rotation_x		= 0.0f;
		float	m_f_rotation_y		= 0.0f;

		float	m_f_y_min			= 0.0f;
		float	m_f_x_min			= 0.0f;

		XMMATRIX			view_matrix {};
		XMMATRIX			projection_matrix {};

		Projection			projection_method = Projection::perspective;
		
		D3D11_BUFFER_DESC						m_buffer_description {};

		ComPtr< ID3D11Buffer >	buffer_matrix_view;
		ComPtr< ID3D11Buffer >	buffer_matrix_projection;

};

/*
XMFLOAT3	m_f3_position	= XMFLOAT3( 0.0 , 0.0 , -10.0  );
XMFLOAT3	m_f3_target		= XMFLOAT3( 0.0 , 0.0 , 0.0 );
XMFLOAT3	m_f3_up			= XMFLOAT3( 0.0 , 1.0 , 0.0 );
*/