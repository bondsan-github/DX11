#pragma once

// type alias
using uchar		= unsigned char;
using uint		= unsigned int;
using ushort	= unsigned short;
using ulong		= unsigned long;

//#include <d3dcommon.h>
//D3D_PRIMITIVE_TOPOLOGY triangle_list = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

typedef struct rectangle
{
	float top {};
	float left{};
	float right{};
	float bottom{};
} rectangle;

enum class VS_buffer { MESH, VIEW, PROJECTION };
//enum { VS_BUFFER_MESH = 0 , VS_BUFFER_CAMERA_VIEW , VS_BUFFER_CAMERA_PROJECTION };

#include <wincodec.h>	// WICImage

class pixel_format
{
	public:
		pixel_format( DXGI_FORMAT _dxgi_id , WICPixelFormatGUID _wic_guid , uint _bytes_per_pixel ) : dxgi_id( _dxgi_id ) , wic_guid( _wic_guid ), bytes_per_pixel(_bytes_per_pixel) { }

		DXGI_FORMAT dxgi_id{};
		WICPixelFormatGUID wic_guid{};
		uint bytes_per_pixel{};
		//colour mask
		//
};

//pixel_format unorm_32bpp{ DXGI_FORMAT_R8G8B8A8_UNORM, GUID_WICPixelFormat32bppRGBA, 1 };
//pixel_format