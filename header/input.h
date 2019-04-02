#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include "dinput.h"
#include <vector>

//#pragma comment( lib , "dinput8.lib" )
#pragma comment( lib , "dxguid.lib" )

class Input
{
    LPDIRECTINPUT8			p_direct_input8     = nullptr;
	LPDIRECTINPUTDEVICE8	p_DI_keyboard       = nullptr;
	LPDIRECTINPUTDEVICE8	p_DI_mouse          = nullptr;
	std::vector< LPDIRECTINPUTDEVICE8 >	p_DI_joysticks; // .clear()

    unsigned char			keyboardState[256]  {};

    DIMOUSESTATE			state_mouse         {};
	unsigned int			state_size_mouse    = sizeof( DIMOUSESTATE );
    DIDEVCAPS				caps_mouse          {};	
	bool					mouse_attached      = false;
	bool					mouse_has_wheel     = false;
	unsigned char			button_count_mouse  = 0;
	unsigned char			axis_count_mouse    = 0;

    DIDEVCAPS				caps_joystick       {};
    DIJOYSTATE				state_joystick      {};
	unsigned int			state_size_joystick = sizeof( DIJOYSTATE );

    GUID                    m_joystickGUID; // ? MAKEGUID

    unsigned char           total_buttons_joystick = 0;

    void initialise();

    public:
        Input();
        ~Input();
};