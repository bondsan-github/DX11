#pragma once

#include <Windows.h>

class Keyboard
{
	public:
		
		Keyboard() {}

		void process_message( unsigned int message , WPARAM wParam , LPARAM lParam )
		{
		}

	private:

		typedef struct keys
		{
		};

		enum class Keys { a , b , c , d , e , f };
		float repeat_delay;
};