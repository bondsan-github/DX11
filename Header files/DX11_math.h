#pragma once

//#include <Windows.h>
//#include <wrl/client.h>
//#include <d3d11.h>
#include <DirectXMath.h>

namespace DirectX
{
	XMFLOAT3 operator *  ( XMFLOAT3 in_lhs , float in_multiplier );  // make arguments references when optimising	
	XMFLOAT3 operator +  ( XMFLOAT3 in_lhs , XMFLOAT3 in_rhs );
	XMFLOAT3 operator += ( XMFLOAT3 in_lhs , XMFLOAT3 in_rhs );
	XMFLOAT3 operator /= ( XMFLOAT3 in_lhs , float divisor ); //dividend is divided by the divisor to get a quotient
}