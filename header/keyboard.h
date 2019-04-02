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

		enum class Keys { a , b , c , d , e , f };
};