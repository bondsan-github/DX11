#pragma once

#include <string>
#include <wrl/client.h>
#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;  // for DirectXMath.h

#include "debugging.h"
#include "Drawable.h"
#include "types.h"

enum class Projection { perspective , orthographic };

class Camera : public Drawable
{
	public:
		//Camera();
		Camera( std::wstring name	= L"camera_default",
				XMVECTOR position	= XMVectorSet( 0.0f , 0.0f , -1.0f , 0.0f ) ,
				XMVECTOR target		= XMVectorSet( 0.0f , 0.0f ,  0.0f , 0.0f ) );

		~Camera();	

		void projection( const Projection in_projection = Projection::perspective )
		{
			D3D11_VIEWPORT viewport;
			UINT           number_of_viewports = 1u;	

			m_p_video_device_context->RSGetViewports( & number_of_viewports , & viewport );

			XMFLOAT4X4 matrix_projection {};

			if( in_projection == Projection::perspective )
			{
				m_matrix_projection = XMMatrixPerspectiveFovLH( XM_PIDIV2 , viewport.Width / viewport.Height , 0.01f , 10000.0f );
			}
			else
			{
				m_matrix_projection = XMMatrixOrthographicLH( viewport.Width , viewport.Height , 0.001f , 1000.0f );

				//m_matrix_projection = XMMatrixOrthographicLH( 200 , 200 , 0.01f , 100.0f );
				//m_matrix_projection = m_matrix_view;							
			}

			XMMATRIX projection_orthographic(
				0 , 0 , 0 , 0 ,
				0 , 0 , 0 , 0 ,
				0 , 0 , 0 , 0 ,
				0 , 0 , 0 , 1 );

			m_matrix_projection = XMMatrixTranspose( m_matrix_projection );
			//m_matrix_projection = projection_orthographic;
		}
		
		void position( const XMVECTOR position );
		void target( const XMVECTOR target );

		void z( const float new_z );
		void delta_z( const float delta_z );

		// set_projection_matrix( XMMATRIX )

		//void roll  
		//void pitch 

		// update(event);
		void render();		

	private:

		Microsoft::WRL::ComPtr< ID3D11Device >			m_p_video_device;
		Microsoft::WRL::ComPtr< ID3D11DeviceContext >	m_p_video_device_context;

		std::wstring m_string_name = L"camera_default";
		
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

		XMMATRIX			m_matrix_view { };
		XMMATRIX			m_matrix_projection;// {};

		Projection			m_projection_method;// = camera_projection::PERSPECTIVE;
		
		D3D11_BUFFER_DESC						m_buffer_description{ };

		Microsoft::WRL::ComPtr< ID3D11Buffer >	m_p_buffer_matrix_view;
		Microsoft::WRL::ComPtr< ID3D11Buffer >	m_p_buffer_matrix_projection;

};

/*
XMFLOAT3	m_f3_position	= XMFLOAT3( 0.0 , 0.0 , -10.0  );
XMFLOAT3	m_f3_target		= XMFLOAT3( 0.0 , 0.0 , 0.0 );
XMFLOAT3	m_f3_up			= XMFLOAT3( 0.0 , 1.0 , 0.0 );
*/