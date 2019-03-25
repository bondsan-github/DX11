// DDC9Input.h
#ifndef DDC9INPUT_H
#define DDC9INPUT_H

#include <stdexcept>
#include <string>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
// * #include <dinputd.h>
//pragma (lib, "dinput8.lib")
#include "DDC9Utils.h"
#include "DDC9Log.h"
#include "DDC9Game.h"
class DDC9Game;

class DDC9Input
{
private:
	const DDC9Game *m_pGame;	
	LPDIRECTINPUT8			m_lpDirectInput8;
	LPDIRECTINPUTDEVICE8	m_lpDIKeyboard;
	LPDIRECTINPUTDEVICE8	m_lpDIMouse;
	LPDIRECTINPUTDEVICE8	m_lpDIJoystick1;
	//LPDIRECTINPUTDEVICE8	m_lpdiJoystick2;
	//list< DDC9Joysticks > joysticks
	//list< DDC9Gamepads > gamepads
	//list< LPDIRECTINPUTDEVICE8 > joysticks;
	unsigned char			m_keyboardState[256]; 
	DIMOUSESTATE			m_mouseState;
	unsigned int			m_mouseStateSize;
	DIDEVCAPS				m_mouseCaps;	
	bool					m_bMouseAttached;
	bool					m_bMouseHasWheel;
	unsigned char			m_mouseButtonCount;
	unsigned char			m_mouseAxisCount;
	//unsigned char			m_ucMouseButtons[4];	
	DIDEVCAPS				m_joystickCaps;
	DIJOYSTATE				m_joystickState;
	unsigned int			m_joystickStateSize;
  /*LONG lX;
    LONG lY;
    LONG lZ;
    LONG lRx;
    LONG lRy;
    LONG lRz;
    LONG rglSlider[2];
    DWORD rgdwPOV[4];
    BYTE rgbButtons[32];*/
	GUID m_joystickGUID; 
	//bool m_bJoystick1Attached;
	unsigned char m_ucJoystickButtonCount;
	ofstream m_log;
	//std::string decToBin(const unsigned int decimal) const;

public:
	DDC9Input(const DDC9Game *pGame);//, const HINSTANCE hInstance);
	~DDC9Input();
	//DDC9Input(const DDC9Input& rhs) {}; 
	//DDC9Input& operator= ( const DDC9Input& rhs ) {};

	static BOOL CALLBACK enumJoysticksCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef); //Enumerate the devices 
	static BOOL CALLBACK setAxisRange(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	void getDeviceCaps();

	void update();

	bool key(const unsigned char key) const { return ( m_keyboardState[key] & 0x80 ) != 0; }

	int mouseDx() const { return m_mouseState.lX; } 
	int mouseDy() const { return m_mouseState.lY; } 
	int mouseDz() const { return m_mouseState.lZ; } 

	//unsigned char rgbButtons[4];	
	bool mouseButtonPressed(const unsigned char button) const;
	
	//joystick int dx, dy, bool button1..32
	//int joystickDx(const unsigned char Joystick1..4)
	int joystickDx() const { return m_joystickState.lX; }
	int joystickDy() const { return m_joystickState.lY; }
	int joystickDz() const { return m_joystickState.lZ; }

	//bool joystickButton(const unsigned char button) const;
	bool joystickButton(const BYTE button) const;
};

#endif