#pragma once

#include <DirectXMath.h>

#include "debugging.h"

// type alias'
using uchar		= unsigned char;
using uint		= unsigned int;
using ushort	= unsigned short;
using ulong		= unsigned long;

//enum class screen_mode { window , fullscreen };

// user defined literal
//unsigned char operator "" _uc( char in_real ) { return static_cast< unsigned char >( in_real ); }

//#include <d3dcommon.h>
//D3D_PRIMITIVE_TOPOLOGY triangle_list = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

namespace DX11
{
	typedef struct Rectangle
	{
		Rectangle() {}

		Rectangle( float in_top , float in_left , float in_right , float in_bottom )
			: top( in_top ) , left( in_left ) , right( in_right ) , bottom( in_bottom )
		{}

		float top = 0.0f;
		float left = 0.0f;
		float right = 0.0f;
		float bottom = 0.0f;

	} Rectangle;
}
// minwindef.h
#undef min
#undef max

typedef struct Bounding_box
{
	Bounding_box( DirectX::XMFLOAT3 in_min = { 0.0f, 0.0f, 0.0f } , DirectX::XMFLOAT3 in_max = { 0.0f, 0.0f, 0.0f } ) : min( in_min ) , max( in_max ) {}

	DirectX::XMFLOAT3 min { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 max { 0.0f, 0.0f, 0.0f };

	DirectX::XMFLOAT3 centre { 0.0f, 0.0f, 0.0f };

	DirectX::XMFLOAT3 get_centre()
	{
		float x = ( min.x + max.x ) / 2.0f;
		float y = ( min.y + max.y ) / 2.0f;
		float z = ( min.z + max.z ) / 2.0f;

		return DirectX::XMFLOAT3( x , y , z );
	}

	//Bounding_box & operator = ( Bounding_box rhs )
	//{
	//	min = rhs.min;
	//	max = rhs.max;
	// centre = rhs.centre;

	//	return * this;
	//}

	Bounding_box & operator += ( DirectX::XMFLOAT3 & rhs ) // const XMFLOAT3 & rhs // receive argument as a const address to an XMFLOAT3
	{
		min.x += rhs.x;
		min.y += rhs.y;
		min.z += rhs.z;

		max.x += rhs.x;
		max.y += rhs.y;
		max.z += rhs.z;

		return * this;  // dereference this ( get address )
	}

	Bounding_box & operator += ( DirectX::XMFLOAT2 & rhs ) // const XMFLOAT3 & rhs // receive argument as a const address to an XMFLOAT3
	{
		min.x += rhs.x;
		min.y += rhs.y;

		max.x += rhs.x;
		max.y += rhs.y;

		return * this;  // dereference this ( get address )
	}

	/*
	void swap( float & lhs , float & rhs )
	{
		float temp = 0.0f;

		temp = lhs;
		lhs = rhs;
		rhs = temp;
	}

	void rotation_z( XMFLOAT3 & in_point , float in_radians )
	{
		in_point.x = in_point.x * cos( in_radians ) - in_point.y * sin( in_radians );
		in_point.y = in_point.y * cos( in_radians ) + in_point.y * sin( in_radians );
	}
		
	Bounding_box rotate_z( double radians )
	{
		DirectX::XMFLOAT3 centre = get_centre();
		Bounding_box rotated;

		rotated.min = min;
		rotated.max = max;

		rotated.min -= centre;
		rotated.max -= centre;
		//rotated.min.x = rotated.min.x - centre.x;
		//rotated.max.x = rotated.max.x - centre.x;

		//rotated.min.y = rotated.min.y - centre.y;
		//rotated.max.y = rotated.max.y - centre.y;

		// XMFLOAT2 rotation_z( XMFLOAT2 in_point , float in_radians )

		//x′ = x * cos θ − y * sin θ
		//y′ = y * cos θ + x * sin θ

		rotation_z( rotated.min , radians );
		rotation_z( rotated.max , radians );

		//rotated.min.x = rotated.min.x * cos( radians ) - rotated.min.y * sin( radians );
		//rotated.min.y = rotated.min.y * cos( radians ) + rotated.min.y * sin( radians );

		//rotated.max.x = rotated.max.x * cos( radians ) - rotated.max.y * sin( radians );
		//rotated.max.y = rotated.max.y * cos( radians ) + rotated.max.y * sin( radians );
		
		rotated.min += centre;
		rotated.max += centre;

		//rotated.min.x = rotated.min.x + centre.x;
		//rotated.max.x = rotated.max.x + centre.x;
		//rotated.min.y = rotated.min.y + centre.y;
		//rotated.max.y = rotated.max.y + centre.y;

		// re-calculate 
		//if( rotated.min.x > rotated.max.x ) swap( rotated.min.x , rotated.max.x );
		//if( rotated.min.y > rotated.max.y ) swap( rotated.min.y , rotated.max.y );

		debug_out( "\n rotated min.x = %.2f" , rotated.min.x );
		debug_out( "\n rotated min.y = %.2f\n" , rotated.min.y );

		return rotated;

		//XMVECTOR * XMATRIX;
	}
	*/
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

/*
class Colour_rgba128bpp_float
{
public:
Colour_rgba128bpp_float() {}

Colour_rgba128bpp_float( const float in_red,  // ? change to unsigned float (PS input) + normalise ?
const float in_green,
const float in_blue,
const float in_alpha )
: m_red( in_red ) , m_green( in_green ) , m_blue( in_blue ) , m_alpha( in_alpha ) { }

//operator *
// return int = m_red << 24 &&
//get ? ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
//unsigned int as_int() {	//int pixel = ( m_red << 24u ) | ( m_green << 16u ) | ( m_blue << 8u ) | m_alpha ) ; }

private:
float m_red { };	// 4 byte
float m_green { };	// 4 byte
float m_blue { };	// 4 byte
float m_alpha { };	// 4 byte
};
*/
//static Colour_8bit_rgba white( 1.0f , 1.0f , 1.0f , 1.0f ); #define white
