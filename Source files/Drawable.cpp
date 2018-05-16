#include "..\Header files\Drawable.h"

Microsoft::WRL::ComPtr< ID3D11Device > Drawable::m_video_device = nullptr;


void Drawable::set_video_device( const Microsoft::WRL::ComPtr< ID3D11Device > p_video_device )
{
	m_video_device = p_video_device;
}

const Microsoft::WRL::ComPtr< ID3D11Device > Drawable::get_video_device( )
{
	return m_video_device;
}
