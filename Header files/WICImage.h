#pragma once

//#include <Windows.h>
#include <vector>
using std::vector;

#include <wrl/client.h>
#include <wincodec.h>
//#include <d3d11.h>
//#include <DirectXMath.h>

using Microsoft::WRL::ComPtr;

#include <string>
using std::wstring;


#include "debugging.h"
#include "types.h"

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
		WICImage( const wstring in_filename );

		void load( const wstring in_filename );

		uint width() const { return _width; }
		uint height() const { return _height; }

		//const DXGI_FORMAT pixel_format() const { return wic_pixel_format_guid; }

		uchar * pixels()
		{ 
			return _pixels.data();
		}

		//const Pixel_format pixel_format() const { return _pixel_format; }

	private:

		vector< uchar > _pixels;

		HRESULT	result { E_FAIL };

		ComPtr< IWICImagingFactory >		imaging_factory {};
		ComPtr< IWICBitmapDecoder >			bitmap_decoder {};
		ComPtr< IWICBitmapFrameDecode >		bitmap_frame_decode {};

		WICPixelFormatGUID					wic_pixel_format_guid {};

		ComPtr< IWICFormatConverter >		format_converter {};
		//GUID m_WIC_guid_target_format;
		BOOL								can_convert = false;

		IWICComponentInfo	* component_info {};
		IWICPixelFormatInfo * pixel_format_info {};
		GUID				format_guid {};

		wchar_t	format_name[ 128 ] {};
		uint	chars_read {};

		//Pixel_format pixel_format {};

		uint bits_per_pixel {};

		uint row_byte_pitch {};
		uint size_bytes {};

		uint channel_count {};

		uint frame_count {};

		uint _width {};
		uint _height {};		
};