#pragma once

#include <DirectXMath.h>

using DirectX::XMFLOAT3;

// type alias
using uchar		= unsigned char;
using uint		= unsigned int;
using ushort	= unsigned short;
using ulong		= unsigned long;

// user defined literal
//unsigned char operator "" _uc( char in_uc ) { return static_cast< unsigned char >( in_uc ); }

//#include <d3dcommon.h>
//D3D_PRIMITIVE_TOPOLOGY triangle_list = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

typedef struct rectangle
{
	rectangle() {}

	rectangle( float in_top , float in_left , float in_right , float in_bottom )
		: top( in_top ) , left( in_left ) , right( in_right ) , bottom( in_bottom )
	{}

	float top		= 0.0f;
	float left		= 0.0f;
	float right		= 0.0f;
	float bottom	= 0.0f;

} rectangle;

typedef struct Bounding_box
{
	XMFLOAT3 min = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 max = { 0.0f, 0.0f, 0.0f };

} Bounding_box;

enum class VS_buffer { MESH, VIEW, PROJECTION };
//enum { VS_BUFFER_MESH = 0 , VS_BUFFER_CAMERA_VIEW , VS_BUFFER_CAMERA_PROJECTION };

//#include <wincodec.h>	// WICImage

//class pixel_format
//{
//	public:
//		pixel_format( DXGI_FORMAT _dxgi_id , WICPixelFormatGUID _wic_guid , uint _bytes_per_pixel ) 
//		: dxgi_id( _dxgi_id ) , wic_guid( _wic_guid ), bytes_per_pixel(_bytes_per_pixel) { }
//
//		DXGI_FORMAT dxgi_id{};
//		WICPixelFormatGUID wic_guid{};
//		uint bytes_per_pixel{};
//		//colour mask
//		//
//};

//pixel_format unorm_32bpp{ DXGI_FORMAT_R8G8B8A8_UNORM, GUID_WICPixelFormat32bppRGBA, 1 };
//pixel_format