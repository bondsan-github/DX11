#pragma once

#include <wrl/client.h>
#include <d3d11.h>

class Drawable
{
	public:

		static void set_video_device( const Microsoft::WRL::ComPtr< ID3D11Device > p_video_device );
		static const Microsoft::WRL::ComPtr< ID3D11Device > get_video_device();

	private:

		static Microsoft::WRL::ComPtr< ID3D11Device > m_video_device;
};