#pragma once

/*
//#include "../DX11/Image.h"
//#include "../DX11/Drawable.h"

//#include <d3d11.h>
#include <vector>

using std::vector;

class Font : public Mesh
{
	private:

		struct center_offset
		{
			uint x;
			uint y;
		};

		struct character
		{
			vector< center_offset > pixels;
		};

		vector< character > character_set;

		Bitmap tilemap_rgba;

		//uchar char_width {};
		//uchar char_height {};
		
		//D3D11_TEXTURE2D_DESC texture2d_description {};
		//D3D11_SUBRESOURCE_DATA subresource_data {};
		//Microsoft::WRL::ComPtr< ID3D11Texture2D > p_texture2D_diffuse;

	public:

		//Font( const float char_scale = 1.0f ){}
		void typeface( wstring in_typeface );

	private:

		void tilemap( const wstring in_filename, XMUINT2 tile_size , XMUINT2 grid_size )// const wstring filename) // const * const wstring filename
		{
			tilemap_rgba.load( in_filename );

			//Quad new_character;

			uint x = 0;
			uint y = 0;
			

			//for( const auto char_raw : * tilemap_rgba.vector_raw() )
			//{
			//	//character new_character;
			//}			
		};

		void draw_character( char in_char );	
};*/