// DDC9Input.cpp

#include "DDC9Input.h"

DDC9Input::DDC9Input(const DDC9Game *pGame) : m_pGame(pGame) //const HINSTANCE hInstance
{
	//if( !pGame ) throw std::invalid_argument("DDC9Input constructor: game pointer invalid");
	
	m_log.open( "Input log.txt", ios::out );	
	m_lpDirectInput8 =	nullptr;
	m_lpDIKeyboard =	nullptr;
	m_lpDIMouse =		nullptr;
	m_lpDIJoystick1 =	nullptr; // vector joysticks<>
	//m_joystickGUID =	typedef struct _GUID {unsigned long  Data1; unsigned short Data2; unsigned short Data3; unsigned char  Data4[ 8 ];}	
	m_mouseStateSize =	sizeof( DIMOUSESTATE );
	m_joystickStateSize =		sizeof( DIJOYSTATE );
	m_mouseCaps.dwSize =		sizeof(DIDEVCAPS);
	m_bMouseAttached =			false;
	m_mouseButtonCount =		0;
	m_bMouseHasWheel =			false;	
	m_mouseAxisCount =			0;
	m_joystickCaps.dwSize =		sizeof(DIDEVCAPS);
	m_ucJoystickButtonCount =	0;
	memset( m_keyboardState, 0, 256 );
	memset( &m_mouseState, 0, m_mouseStateSize );
	memset( &m_joystickState, 0, m_joystickStateSize );
	//memset( &m_mouseCaps, 0, sizeof( DIDEVCAPS ) );
	//memset( &m_joystickCaps, 0, sizeof( DIDEVCAPS ) );

	HINSTANCE hW = m_pGame->getHInstance();
	HMODULE hM = GetModuleHandle(0);

	//HR( DirectInput8Create( m_pGame->getHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast< void ** >( &m_lpDirectInput8 ), 0 ) ); 
	HR( DirectInput8Create( GetModuleHandle( 0 ), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast< void ** >( &m_lpDirectInput8 ), 0 ) );
	//HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter )

	/*
	struct DI_ENUM_CONTEXT   
    {   
        DIJOYCONFIG* pPreferredJoyCfg;   // dinputd.h
        bool bPreferredJoyCfgValid;   
    };  

	HRESULT hr;
	DIJOYCONFIG PreferredJoyCfg = {0};   
    DI_ENUM_CONTEXT enumContext;   
    enumContext.pPreferredJoyCfg = &PreferredJoyCfg;   
    enumContext.bPreferredJoyCfgValid = false;          
    IDirectInputJoyConfig8* pJoyConfig = 0;     

    hr = m_lpDirectInput8->QueryInterface( IID_IDirectInputJoyConfig8, (void **) &pJoyConfig );   
  
    PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);  
    if( SUCCEEDED( pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached   
        enumContext.bPreferredJoyCfgValid = true;  
    if( pJoyConfig )
	{
		pJoyConfig->Release();
		pJoyConfig = 0;
	}
	//PreferredJoyCfg.guidInstance	*/
		
	HR( 
		m_lpDirectInput8->EnumDevices( DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, reinterpret_cast< void * >( &m_joystickGUID ), DIEDFL_ATTACHEDONLY ); 
	);

	//m_log << "\n joystick guid: " << m_joystickGUID.Data1 << m_joystickGUID.Data2 << m_joystickGUID.Data3 << (char)m_joystickGUID.Data4 << std::flush;

	// int m = 1;
	// &m = 0x0
	// *&m = 0x0

	//DI_OK. 
	//DIERR_INVALIDPARAM == hr || DIERR_NOTINITIALIZED == hr
	// no joystick

	//c_dfDIJoystick — Three positional axes, three rotation axes, two sliders, a POV hat, and 32 buttons. Corresponds to the DIJOYSTATE structure.
	//c_dfDIKeyboard — An array of 256 characters, one for each key.
	//c_dfDIMouse — Three axes and four buttons. DIMOUSESTATE structure.
	//The window associated with the device must not be destroyed while it is still active in a DirectInput device. 

	//DISCL_FOREGROUND device is automatically unacquired when window moves to background. only be reacquired when the application moves to the foreground.

	if( DI_OK == m_lpDirectInput8->CreateDevice( GUID_SysKeyboard, &m_lpDIKeyboard, 0 ) )
	{
		HR( m_lpDIKeyboard->SetDataFormat( &c_dfDIKeyboard ) );
		HR( m_lpDIKeyboard->SetCooperativeLevel( m_pGame->getMainWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) );
		//HR( 
			m_lpDIKeyboard->Acquire();
		//		);
		//DIERR_OTHERAPPHASPRIO = E_ACCESSDENIED
	}

	if( DI_OK ==  m_lpDirectInput8->CreateDevice( GUID_SysMouseEm, &m_lpDIMouse, 0 ) )
	{
		HR( m_lpDIMouse->SetDataFormat( &c_dfDIMouse ) ); 
		HR( m_lpDIMouse->SetCooperativeLevel( m_pGame->getMainWindow(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) );
		/*HR*/ m_lpDIMouse->Acquire();
	}

	// joysticks.at(0) = ->CreateDevice	
	if( DI_OK == m_lpDirectInput8->CreateDevice( m_joystickGUID, &m_lpDIJoystick1, 0 ) )
	{
		HRESULT hr;

		HR( m_lpDIJoystick1->SetDataFormat( &c_dfDIJoystick ) ); 
		HR( m_lpDIJoystick1->SetCooperativeLevel( m_pGame->getMainWindow(), DISCL_EXCLUSIVE | DISCL_FOREGROUND ) );
		hr = m_lpDIJoystick1->SetEventNotification( 0 );
		m_log << "\n m_lpDIJoystick1->SetEventNotification() = " << DXGetErrorStringA( hr ) << " = " << DXGetErrorDescriptionA( hr );
		//HR( 
			hr = m_lpDIJoystick1->Acquire();
		//	);

		m_log << "\n m_lpDIJoystick1->Acquire() = " << DXGetErrorStringA( hr ) << " = " << DXGetErrorDescriptionA( hr );

		HR( m_lpDIJoystick1->EnumObjects( setAxisRange, (void*)m_lpDIJoystick1, DIDFT_AXIS ) );
	} 	

	// Enumerate the joystick objects. The callback function enabled user   
	// interface elements for objects that are found, and sets the min/max   
	// values property for discovered axes.   
	//if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback, (VOID*)hDlg, DIDFT_ALL ) ) ) 

	getDeviceCaps();

	DIPROPDWORD dipdw;  // DIPROPDWORD contains a DIPROPHEADER structure. 
	HRESULT hr; 
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwObj        = 0; // device property 
	dipdw.diph.dwHow        = DIPH_BYID;//DIPH_BYUSAGE; 

	if( m_lpDIJoystick1 )
	{
		hr = m_lpDIJoystick1->GetProperty(DIPROP_RANGE, &dipdw.diph); 
		if (SUCCEEDED(hr)) { 
		// The dipdw.dwData member contains the buffer size. 
		} 	
	}

	m_mouseState.lX = 0;
	m_mouseState.lY = 0;
	m_mouseState.lZ = 0;
	m_mouseState.rgbButtons[0] = 0;
	m_mouseState.rgbButtons[1] = 0;
	m_mouseState.rgbButtons[2] = 0;
	m_mouseState.rgbButtons[3] = 0;
}

DDC9Input::~DDC9Input()
{	
	m_log << "\n ~DDC9Input() start";

	if( m_lpDIJoystick1 ) 
	{
		HRESULT hr;

		//memset( &m_joystickState, 0, m_joystickStateSize );
		//try
		//{
			//IDirectInputDevice8_Unacquire(m_lpDIJoystick1);
			hr = m_lpDIJoystick1->Unacquire();
		//}
		//catch(...)
		//{
		//	m_pGame->log()->write(" unaquire exception ");
		//}
		m_log << "\n m_lpDIJoystick1->Unacquire() = " << DXGetErrorStringA( hr ) << " = " << DXGetErrorDescriptionA( hr );
		//hr = m_lpDIJoystick1->Unacquire();

		//releaseCom( m_lpDIJoystick1 );
		m_lpDIJoystick1->Release();
		//delete m_lpDIJoystick1;
		m_lpDIJoystick1 = 0;
	}	
	
	if( m_lpDIMouse ) 
	{
		m_lpDIMouse->Unacquire();
		releaseCom( m_lpDIMouse );
	}

	if( m_lpDIKeyboard ) 
	{
		m_lpDIKeyboard->Unacquire();
		releaseCom( m_lpDIKeyboard );
	}	
	
	releaseCom( m_lpDirectInput8 );	
	m_log << "\n ~DDC9Input() end";
	//m_pGame = 0;	
	m_log.close();
}


BOOL CALLBACK DDC9Input::enumJoysticksCallback(LPCDIDEVICEINSTANCE  lpddi, LPVOID pvRef)   
{
	//LPDIDEVICEINSTANCE lpddi, a ptr from DirectInput containing info about the device it just found on this iteration

	//DDC9Input* input = (DDC9Input*)pvRef;

	*static_cast< GUID * >( pvRef ) = lpddi->guidInstance;
	//*(GUID*)pvRef = lpddi->guidInstance;

	//lpddi->guidInstance.Data

	//HRESULT hr = StringCchCopy(szName, MAX_PATH, lpddoi->tszName);

	//m_log << "\n joystick instance name: " << lpddi->tszInstanceName;
	//m_pGame->log()->write(L"\n joystick product name: %s", lpddi->tszProductName);
	//input->m_pGame->log()->write(L"\n joystick guidInstance: %s", lpddi->guidInstance);
	//input->m_pGame->log()->write(L"\n joystick guidProduct: %s", lpddi->guidProduct);

	//input->m_joystickGUID = lpddi->guidInstance;
	
	return DIENUM_STOP;   	
	//Returns DIENUM_CONTINUE to continue the enumeration or DIENUM_STOP to stop the enumeration.
}

BOOL CALLBACK DDC9Input::setAxisRange(const LPCDIDEVICEOBJECTINSTANCE lpddoi, void* pvRef)
{
	if( lpddoi->dwType & DIDFT_AXIS )
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize       = sizeof(DIPROPRANGE);    
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);    
		diprg.diph.dwHow        = DIPH_BYID;    
		diprg.diph.dwObj        = lpddoi->dwType; // Specify the enumerated axis   
		diprg.lMin              = -1000;    
		diprg.lMax              = +1000;    
           
		/*struct joyAxis
		{
			LPDIRECTINPUTDEVICE8 diDevice;
			DIPROPRANGE axisRange;
		};*/

		// Set the range for the axis   
		if( FAILED( static_cast< LPDIRECTINPUTDEVICE8  >( pvRef )->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
			return DIENUM_STOP;    
	}  

	/*ofstream file( "joystick info.txt", ios::app );
	file << '\n' << reinterpret_cast< const char * >(lpddoi->tszName);
	file << "\t: " << lpddoi->dwType;
	file.close();*/

	//DIPROP_RANGE 
	//DIPROP_AXISMODE 

	//return DIENUM_STOP;
	return DIENUM_CONTINUE;
}

void DDC9Input::update()
{
	HRESULT hr;
	
	if( m_lpDIKeyboard )
	{
		hr = m_lpDIKeyboard->GetDeviceState(256, &m_keyboardState);
		//DIERR_INPUTLOST, DIERR_INVALIDPARAM, DIERR_NOTACQUIRED, DIERR_NOTINITIALIZED, E_PENDING.

		// if( DIERR_INPUTLOST == hr ||  )
		if( FAILED( hr ) )
		{
			memset( &m_keyboardState, 0, sizeof(m_keyboardState) );
			m_lpDIKeyboard->Acquire();
		}
	}

	if( m_lpDIMouse )
	{
		m_lpDIMouse->Poll();

		hr = m_lpDIMouse->GetDeviceState( m_mouseStateSize, &m_mouseState );

		if( FAILED( hr ) )
		{
			memset( &m_mouseState, 0, m_mouseStateSize );
			m_lpDIMouse->Acquire();
		}
	}

	// if( m_bJoystickAttached ) for( each joystick ) getDeviceState()
	if( m_lpDIJoystick1 )
	{
		//hr = m_lpDIJoystick1->Poll();
		// If the method succeeds, the return value is DI_OK, or DI_NOEFFECT if the device does not require polling. 
		// If the method fails, the return value can be one of the following error values: DIERR_INPUTLOST, DIERR_NOTACQUIRED, DIERR_NOTINITIALIZED.
		
		//m_pGame->log()->write( DXGetErrorStringA(hr) );
		//DIERR_INPUTLOST, DIERR_INVALIDPARAM, DIERR_NOTACQUIRED, DIERR_NOTINITIALIZED, E_PENDING.
		
		hr = m_lpDIJoystick1->Poll();

		//if( FAILED( hr ) )
		if( DIERR_NOTACQUIRED == hr || DIERR_INPUTLOST == hr)
		{
			m_log << "\n joystick DIERR_INPUTLOST";

			memset( &m_joystickState, 0, m_joystickStateSize );

			if( DI_OK == m_lpDIJoystick1->Acquire() )
				m_log << "\n joystick re-aquired";	
			//DIERR_OTHERAPPHASPRIO
		}		

		hr = m_lpDIJoystick1->GetDeviceState( sizeof( DIJOYSTATE ), &m_joystickState );
	}
}

//bool DDC9Input::key(const unsigned char key)
//{
//	return ( m_keyboardState[key] & 0x80 ) != 0;
	//char[256] 256 bytes - key on sets high bit 10000000
	//									  0x80 = 10000000	
//}

void DDC9Input::getDeviceCaps()
{	
	//m_mouseCaps.dwFlags = DIDC_ATTACHED | 

	if( m_lpDIMouse )
	{
		HR( m_lpDIMouse->GetCapabilities( &m_mouseCaps ) ); 
	
		m_bMouseAttached = m_mouseCaps.dwFlags & DIDC_ATTACHED;
		m_bMouseHasWheel = ( (m_mouseCaps.dwFlags & DIDC_ATTACHED) && (m_mouseCaps.dwAxes > 2) );	

		m_mouseButtonCount = m_mouseCaps.dwButtons;
		m_mouseAxisCount = m_mouseCaps.dwAxes;

		m_log << "\n mouse attached: " << m_bMouseAttached ? "yes" : "no";
		m_log << "\n mouse buttons: " << static_cast< int >(m_mouseButtonCount);
		//m_pGame->log()->write( "\n mouse axes: %d", m_mouseAxisCount );
		//m_pGame->log()->write( "\n mouse has wheel: %s", m_bMouseHasWheel ? "yes" : "no" );
	}

	/* deviceObjectEnum
	DIPROPDWORD dipdw;  // DIPROPDWORD contains a DIPROPHEADER structure. 
	HRESULT hr; 
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwObj        = 0; // device property 
	dipdw.diph.dwHow        = DIPH_DEVICE; //DIPROP_AXISMODE 
	//Retrieves the axis mode. The retrieved value can be DIPROPAXISMODE_ABS or DIPROPAXISMODE_REL. 
	*/
	
	//m_lpDIJoystick1->GetProperty(m_joystickGUID, &dipdw.diph);

	if( m_lpDIJoystick1 )
	{
		HR( m_lpDIJoystick1->GetCapabilities( &m_joystickCaps ) ); 

		m_ucJoystickButtonCount = m_joystickCaps.dwButtons;

		//m_bCallPoll = m_joystickCaps.dwFlags & DIDC_POLLEDDATAFORMAT ? true : flase;

		if( m_joystickCaps.dwFlags & DIDC_POLLEDDATAFORMAT )
			m_log << "\n joystick requires call to poll()";

		if( m_joystickCaps.dwFlags & DIDC_FORCEFEEDBACK )
			m_log << "\n joystick supports force feedback.";

		m_log << "\n joystick axes: " << m_joystickCaps.dwAxes;
		m_log << "\n joystick buttons: " << m_joystickCaps.dwButtons;
		m_log << "\n joystick POVs: " << m_joystickCaps.dwPOVs;
	}

	/*DIDEVCAPS {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwDevType;
    DWORD dwAxes;
    DWORD dwButtons;
    DWORD dwPOVs;
    DWORD dwFFSamplePeriod;
    DWORD dwFFMinTimeResolution;
    DWORD dwFirmwareRevision;
    DWORD dwHardwareRevision;
    DWORD dwFFDriverVersion;*/
}

/*DDC9Log* const DDC9Input::log(void) const
{ 
	return m_pGame->getLogPtr(); 
}*/

//bool DDC9Input::joystickButton(const unsigned char button) const
bool DDC9Input::joystickButton(const BYTE button) const
{ 

	//char[256] 256 bytes - key on sets high bit 10010010
	//									  0x80 = 10000000

	return ( m_joystickState.rgbButtons[button] & 0x80 ) ; 
}

bool DDC9Input::mouseButtonPressed(const unsigned char button) const
{ 
	return ( m_mouseState.rgbButtons[button] & 0x80 ); 
} 

