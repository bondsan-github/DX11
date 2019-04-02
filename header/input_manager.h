#pragma once

#include <memory>

class Tetrimino;

class Command
{
	public:
	virtual ~Command() {};
	virtual void exectute( void active_tetri ) = 0;
};

class RotateCommand : public Command
{
	public:
	virtual void execute( Tetrimino & active_tetri )
	{
		active_tetri.rotate( direction::clock_wise );
	}
};

class InputHandler
{
	public:
	void handle_input()
	{
		//auto kb = m_keyboard->GetState();

		//if( kb.Z )	{		}
	}

	private:
		//shared_ptr< Command > m_button_X;
		Command * button_x;
};