#pragma once

#include <Windows.h>
#include <vector>
#include <wrl/client.h>
#include <wincodec.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

#include "..\DX11\debugging.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using std::wstring;

class WICImage //: public Image
{
	public:

		WICImage() { }
		WICImage( const wstring in_filename );

		void load( const wstring in_filename );

		unsigned int width( void ) const { return m_width; }
		unsigned int height( void ) const { return m_height; }

		const unsigned char * pixels() const 
		{ 
			return m_pixels.data();
		}

	private:

		HRESULT	m_result { E_FAIL };

		ComPtr< IWICImagingFactory >		m_imaging_factory;
		ComPtr< IWICBitmapDecoder >			m_bitmap_decoder;
		ComPtr< IWICBitmapFrameDecode >		m_bitmap_frame_decode;

		WICPixelFormatGUID m_pixel_format { };

		ComPtr< IWICFormatConverter > m_format_converter;
		//GUID m_WIC_guid_target_format;

		unsigned int m_row_byte_pitch { };
		unsigned int m_size_bytes { };

		unsigned int m_width { };
		unsigned int m_height { };

		std::vector< unsigned char > m_pixels;
};