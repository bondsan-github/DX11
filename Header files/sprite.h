#pragma once

#include "Texture.h"

//#include <list>
#include <vector>
#include <map>

//using std::list;
using std::wstring;
using std::vector;
using std::map;

class Sprite
{
	private:

		// class composition 
	/*
		class frames
		{
			private:
				
				Texture image;

				const uint delay_ms;
		};
		
		map< wstring , frames > animation;
		*/				
	public:

	Sprite( const wstring filename )
	{

	}

			void add_animation( wstring name, const wstring filename )
			{
				// if(frames.last) or linked list 
				//vector< frames > anim_frames;

				Texture anim( filename );

			}

			void update( const double time_delta );
		
			void draw();
};


