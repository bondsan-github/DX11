#include "Input.h"

Input::Input()
{
    caps_mouse.dwSize       = sizeof(DIDEVCAPS);
    caps_joystick.dwSize    = sizeof(DIDEVCAPS);

    initialise();
}

void Input::initialise()
{
    HRESULT h_result;

    h_result = DirectInput8Create( GetModuleHandle( nullptr ), 
                                   DIRECTINPUT_VERSION,
                                   IID_IDirectInput8,
                                   reinterpret_cast< void ** >( &p_direct_input8 ),
                                   nullptr );



}