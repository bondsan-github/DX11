#pragma once

#include <DirectXMath.h> 
using namespace DirectX;

//enum class vertex_types { vertex , vertex_colour , vertex_texture };

struct vertex//_xyz
{
	vertex( const XMFLOAT3 in_xyz = XMFLOAT3( 0.0f , 0.0f , 0.0f ) )
		
		: xyz( in_xyz ) { }

	bool operator == ( const vertex & in_vertex ) // address & is always const !?
	{
		if( xyz.x == in_vertex.xyz.x && xyz.y == in_vertex.xyz.y && xyz.z == in_vertex.xyz.z ) 
			return true;
		else 
			return false;
	}
	
	//operator = ( const vertex & in_vertex )
	//operator - ( const vertex & in_vertex ) 

	private:
		XMFLOAT3 xyz;
};


struct vertex_rgba : public vertex
{
	vertex_rgba( const XMFLOAT3 in_xyz,
				 const XMFLOAT4 in_rgba = XMFLOAT4( 0.0f , 0.0f , 0.0f , 0.0f ) )
		
		: vertex( in_xyz ) , rgba( in_rgba ) { }

	private:
		XMFLOAT4 rgba;
};


struct vertex_rgba_uv : public vertex_rgba
{
	vertex_rgba_uv( const XMFLOAT3 in_xyz ,
				   const XMFLOAT4 in_rgba ,
				   const XMFLOAT2 in_texture_uv = XMFLOAT2( 0.0f , 0.0f ) )
		
		: vertex_rgba( in_xyz, in_rgba ) , texture_uv( in_texture_uv ) { }

	private:
		XMFLOAT2 texture_uv;
};

