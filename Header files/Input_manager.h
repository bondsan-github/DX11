#pragma once

#include <memory>

class Command
{
	public:
	virtual ~Command() {};
	virtual void exectute( shared_ptr< void > active_tetri ) = 0;
};

class RotateCommand : public Command
{
	public:
	virtual void execute( shared_ptr< Tetrimino > active_tetri )
	{
		active_tetri->rotate( direction::clock_wise );
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
	shared_ptr< Command > m_button_X;
};