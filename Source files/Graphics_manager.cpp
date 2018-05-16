#include "Graphics_manager.h"

Graphics_manager::Graphics_manager( Microsoft::WRL::ComPtr< ID3D11Device > const p_video_device )
	: m_p_video_device( p_video_device )
{

}


/*
void Graphics_manager::add_camera( )
{
Camera camera_default;
m_map_cameras[ "default" ] = camera_default;

// find "default"
m_itr_active_camera = m_map_cameras.begin( );// ->second;
}

void Graphics_manager::add_camera( std::wstring camera_name )
{

}

void Graphics_manager::add_camera( std::wstring camera_name , XMFLOAT3 location , XMFLOAT3 look_at )
{

}

void Graphics_manager::set_active_camera( std::wstring camera_name )
{

}
*/