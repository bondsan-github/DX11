#pragma once

//#include <Windows.h>
#include <vector>
#include <wrl/client.h>
#include <wincodec.h>
//#include <d3d11.h>
//#include <DirectXMath.h>
#include <string>

#include "..\DX11\debugging.h"

/*
struct WIC_to_DXGI
{
	GUID		wic;
	DXGI_FORMAT	format;
};

static WIC_to_DXGI WICFormats[]
{
	{ GUID_WICPixelFormat32bppRGBA,	DXGI_FORMAT_R8G8B8A8_UNORM }
};
*/

class WICImage //: public Image
{
	public:

		WICImage() { }
		WICImage( const std::wstring in_filename );

		void load( const std::wstring in_filename );

		unsigned int width( void ) const { return m_width; }
		unsigned int height( void ) const { return m_height; }

		//const DXGI_FORMAT format() const { return m_pixel_format; }

		const unsigned char * pixels() const
		{ 
			return m_pixels.data();
		}

	private:

		std::vector< unsigned char > m_pixels;

		HRESULT	m_result { E_FAIL };

		Microsoft::WRL::ComPtr< IWICImagingFactory >		m_imaging_factory;
		Microsoft::WRL::ComPtr< IWICBitmapDecoder >			m_bitmap_decoder;
		Microsoft::WRL::ComPtr< IWICBitmapFrameDecode >		m_bitmap_frame_decode;

		WICPixelFormatGUID m_pixel_format { };

		Microsoft::WRL::ComPtr< IWICFormatConverter > m_format_converter;
		//GUID m_WIC_guid_target_format;

		unsigned int m_row_byte_pitch { };
		unsigned int m_size_bytes { };

		unsigned int m_frame_count { };

		unsigned int m_width { };
		unsigned int m_height { };		
};