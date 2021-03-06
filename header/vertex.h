#pragma once

#include <DirectXMath.h>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMUINT4; // for DXGI_FORMAT_R8G8B8A8_UNORM colour

//enum class vertex_types { vertex , vertex_colour , vertex_texture };

struct vertex//_xyz
{
	vertex( const XMFLOAT3 in_point = XMFLOAT3( 0.0f , 0.0f , 0.0f ) )
	: point( in_point )
	{ }

	bool operator == ( const vertex & in_vertex ) // address & is always const !?
	{
		if( point.x == in_vertex.point.x && point.y == in_vertex.point.y && point.z == in_vertex.point.z ) // swap variables for legibility
			return true;
		else 
			return false;
	}
	
	//operator = ( const vertex & in_vertex )
	//operator - ( const vertex & in_vertex )

	//private:
		XMFLOAT3 point;
};


struct vertex_rgba : public vertex
{
	vertex_rgba( const XMFLOAT3 in_point = XMFLOAT3( 0.0f , 0.0f , 0.0f ) ,
				// const uchar in_rgba[ 4 ] = { 0 } )
				 const XMFLOAT4 in_rgba = XMFLOAT4( 0.0f , 0.0f , 0.0f , 0.0f ) )
	: vertex( in_point ) , rgba( in_rgba )
	{
		/*rgba[ 0 ] = in_rgba[ 0 ];
		rgba[ 1 ] = in_rgba[ 1 ];
		rgba[ 2 ] = in_rgba[ 2 ];
		rgba[ 3 ] = in_rgba[ 3 ];*/

		//memcopy(rgba, in_rgba, 4);
	}

	private:
		XMFLOAT4 rgba {};
		//uchar rgba[4];

};

struct vertex_uv : public vertex
{
	vertex_uv( const XMFLOAT3 in_point = XMFLOAT3( 0.0f , 0.0f , 0.0f ) ,
			   const XMFLOAT2 in_uv = XMFLOAT2( 0.0f , 0.0f ) )
		: vertex( in_point ) , uv( in_uv )
	{}

	//private:
		XMFLOAT2 uv;
};


struct vertex_rgba_uv : public vertex_rgba
{
	vertex_rgba_uv( const XMFLOAT3 in_point = XMFLOAT3( 0.0f , 0.0f , 0.0f ) ,
					const XMFLOAT4 in_rgba = XMFLOAT4( 0 , 0 , 0 , 0 ) ,
					const XMFLOAT2 in_texture_uv = XMFLOAT2( 0.0f , 0.0f ) )
		
	: vertex_rgba( in_point , in_rgba ) , texture_uv( in_texture_uv ) 
	{ }

	private:
		XMFLOAT2 texture_uv;
};

