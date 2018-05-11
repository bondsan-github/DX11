#pragma once

#include "DX11_base.h"

#include "Camera.h"
#include "Icosahedron.h"
//#include "Cube.h"

class DX11_derived : public DX11
{
    ID3D11VertexShader *    vertex_shader_solid_colour	= nullptr;
    ID3D11PixelShader *     pixel_shader_solid_colour	= nullptr;

    ID3D11InputLayout *     input_layout				= nullptr;
    ID3D11Buffer *          vertex_buffer				= nullptr;

    ID3D11SamplerState *    diffuse_map_sampler			= nullptr;

    //Cube * cube = nullptr;  //std::unique_ptr
    Icosahedron *	icosphere	= nullptr; // add_mesh() // add_icosphere( location, radius, tesselation_amount, COLOUR | material() );

    Camera *		camera		= nullptr; // add_camera( location, look_at); // TARGET_POINT|TARGET_MESH|

    public:

		DX11_derived( HINSTANCE h_instance , HWND h_window );
        virtual ~DX11_derived();

        bool load_content();
        void unload_content();

        void update( float delta_time );
        void render();
};