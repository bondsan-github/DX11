#pragma once

typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned short	ushort;
typedef unsigned long	ulong;

//#include <d3dcommon.h>
//D3D_PRIMITIVE_TOPOLOGY triangle_list = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

enum class buffer_VS { VS_BUFFER_MESH = 0, VS_BUFFER_VIEW, VS_BUFFER_PROJECTION };
enum { VS_BUFFER_MESH = 0 , VS_BUFFER_CAMERA_VIEW , VS_BUFFER_CAMERA_PROJECTION };

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