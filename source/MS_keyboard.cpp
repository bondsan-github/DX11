#include "MS_keyboard.h"
//#include "PlatformHelpers.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

static_assert( sizeof( Keyboard::State ) == ( 256 / 8 ) , "Size mismatch for State" ); // 256 / 8 = 32

Keyboard::Keyboard()
{
	memset( & state , 0 , sizeof( State ) );
}

void Keyboard::key_down( int key , Keyboard::State & state )
{
	if( key < 0 || key > 0xfe ) return;

	auto ptr = reinterpret_cast< uint32_t * >( & state );

    unsigned int bf = 1u << (key & 0x1f);  // key & 11111 // bit field
    ptr[(key >> 5)] |= bf;

	// A:  0x41 = 1000001
	//     0x1f =   11111
	//        & =       1
	//    1u << =      10 ( 2 decimal )
	//       bf =	   10

	// A:0x41 >> 5 = 0x2  ( 10 binary )
}

void Keyboard::key_up( int key , Keyboard::State & state )
{
	if( key < 0 || key > 0xfe ) return;

	auto ptr = reinterpret_cast< uint32_t * >( & state );

	unsigned int bf = 1u << ( key & 0x1f );
	ptr[ ( key >> 5 ) ] &= ~bf;
}

//State __cdecl Keyboard::GetState( State& state ) const
//{
//	memcpy( &state , &mState , sizeof( State ) );
//}

void Keyboard::reset()
{
	memset( & state , 0 , sizeof( State ) );
}

void Keyboard::process_message(UINT message, WPARAM wParam, LPARAM lParam)
{
    bool down = false;

	switch( message )
    {
		case WM_ACTIVATEAPP:
			reset();
			return;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			down = true;
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			break;

		default:
			return;
    }

	int vk = static_cast< int >( wParam );

	switch( vk )
    {
		case VK_SHIFT:

			vk = MapVirtualKey( ( lParam & 0x00ff0000 ) >> 16 , MAPVK_VSC_TO_VK_EX );

			if ( !down )
			{
				// Workaround to ensure left vs. right shift get cleared when both were pressed at same time
				key_up( VK_LSHIFT , state );
				key_up( VK_RSHIFT , state );
			}
			break;

		case VK_CONTROL:
			vk = (lParam & 0x01000000) ? VK_RCONTROL : VK_LCONTROL;
			break;

		case VK_MENU:
			vk = (lParam & 0x01000000) ? VK_RMENU : VK_LMENU;
			break;
    }

	if( down )
    {
		key_down( vk , state );
    }
    else
    {
		key_up( vk , state );
    }
	// return vk
}

//#pragma warning( disable : 4355 )

//Keyboard::State Keyboard::GetState() const
//{
//    State state;
//    pImpl->GetState(state);
//    return state;
//}

//======================================================================================
// KeyboardStateTracker
//======================================================================================

void Keyboard::KeyboardStateTracker::update( const State & state )
{
    auto currPtr		= reinterpret_cast< const uint32_t * >( & state );
    auto prevPtr		= reinterpret_cast< const uint32_t * >( & last_state );
    auto releasedPtr	= reinterpret_cast< uint32_t * >( & released) ;
    auto pressedPtr		= reinterpret_cast< uint32_t * >( & pressed );

	for( size_t j = 0; j < ( 256 / 32 ); ++j )
    {
        * pressedPtr = * currPtr & ~( * prevPtr );
        * releasedPtr = ~( * currPtr ) & * prevPtr;

        ++currPtr;
        ++prevPtr;
        ++releasedPtr;
        ++pressedPtr;
    }

    last_state = state;
}

void Keyboard::KeyboardStateTracker::reset()
{
    memset( this, 0, sizeof( KeyboardStateTracker ) );
}
