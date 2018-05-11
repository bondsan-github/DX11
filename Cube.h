#pragma once

#include "Mesh.h"

//template< typename vertex_t >
class Cube_colour : public Mesh//< vertex_colour_texture >
{
    public:
        //Cube( ID3D11Device * p_video_device, XMFLOAT3 size = XMFLOAT3( 1,1,1 ), XMFLOAT3 position = XMFLOAT3( 0, 0, 0 ) );

		Cube_colour( ) //input layout, XMFLOAT3 size, XMFLOAT3 position );
			//: Mesh( p_video_device )
		{
			XMFLOAT2 texture_coord( 0.0f , 0.0f );

			// set vertex buffer
			std::vector< vertex_colour_diffuse > cube_vertices
			{
				//				position	 x		y	   z,		colour    r		g	  b		a
				vertex_colour_diffuse( XMFLOAT3( -0.5f,  0.5f, -0.5f ) , XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) , texture_coord ),	// ftl // 0
				vertex_colour_diffuse( XMFLOAT3(  0.5f,  0.5f, -0.5f ) , XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) , texture_coord ),	// ftr // 1
				vertex_colour_diffuse( XMFLOAT3( -0.5f, -0.5f, -0.5f ) , XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) , texture_coord ),	// fbl // 2
				vertex_colour_diffuse( XMFLOAT3(  0.5f, -0.5f, -0.5f ) , XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) , texture_coord ),	// fbr // 3

				vertex_colour_diffuse( XMFLOAT3( -0.5f,  0.5f,  0.5f ) , XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) , texture_coord ),	// btl // 4
				vertex_colour_diffuse( XMFLOAT3(  0.5f,  0.5f,  0.5f ) , XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) , texture_coord ),	// btr // 5
				vertex_colour_diffuse( XMFLOAT3( -0.5f, -0.5f,  0.5f ) , XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) , texture_coord ),	// bbl // 6
				vertex_colour_diffuse( XMFLOAT3(  0.5f, -0.5f,  0.5f ) , XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) , texture_coord )	// bbr // 7
			};

			// set index buffer
			std::vector< unsigned short > cube_indices // short = 16-bit
			{
				0,1,2, 2,1,3,   // front
				1,5,3, 3,5,7,   // right
				0,4,5, 0,5,1,   // top
				7,6,2, 3,7,2,   // bottom
				2,4,0, 6,4,2,   // left
				6,5,4, 7,5,6    // back
			};

			/*
			std::vector< vertex_t > cube_vertices
			{
			vertex_t( XMFLOAT3( -1.0f , 1.0f , -1.0f ),	XMFLOAT4( 0.0f , 0.0f , 1.0f , 1.0f ) ) ,
			vertex_t( XMFLOAT3(  1.0f, 1.0f, -1.0f ),	XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) ) ,
			vertex_t( XMFLOAT3(  1.0f, 1.0f, 1.0f ),	XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) ) ,
			vertex_t( XMFLOAT3( -1.0f, 1.0f, 1.0f ),	XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) ) ,
			vertex_t( XMFLOAT3( -1.0f, -1.0f, -1.0f ),	XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) ) ,
			vertex_t( XMFLOAT3(  1.0f, -1.0f, -1.0f ),	XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) ) ,
			vertex_t( XMFLOAT3(  1.0f, -1.0f, 1.0f ),	XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ) ,
			vertex_t( XMFLOAT3( -1.0f, -1.0f, 1.0f ),	XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) )
			};

			std::vector< unsigned short > cube_indices // short = 16-bit
			{
			3,1,0,
			2,1,3,

			0,5,4,
			1,5,0,

			3,4,7,
			0,4,3,

			1,6,5,
			2,6,1,

			2,7,6,
			3,7,2,

			6,4,5,
			7,4,6,
			};
			*/
			topology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

			vertices( cube_vertices );
			indices( cube_indices );

			// setup diffuse map
		}
        //~Cube();
        
        //XMMATRIX XMMatrixRotationAxis( XMVECTOR Axis, FLOAT Angle )

        //void rotate( float radians, AXIS axis ); // switch( axis ) case: X
        //void rotate( float x, float y, float z ); //XMMatrixRotationRollPitchYaw // XMVECTOR 
        
        //void scale( float amount, AXIS axis );

};