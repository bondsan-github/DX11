#include "DX11_math.h"

namespace DirectX
{
	XMFLOAT3 operator * ( XMFLOAT3 in_xmf3 , float in_multiplier )  // make arguments references when optimising
	{
		XMFLOAT3 mulitple;

		mulitple.x = mulitple.y = mulitple.z = 0.0f;

		mulitple.x = in_xmf3.x * in_multiplier;
		mulitple.y = in_xmf3.y * in_multiplier;
		mulitple.z = in_xmf3.z * in_multiplier;

		return mulitple;
	}

	XMFLOAT3 operator + ( XMFLOAT3 in_lhs , XMFLOAT3 in_rhs )
	{
		// var1 + var2 + var3

		// operator + (var1 , var2 )

		XMFLOAT3 addition;

		addition.x = addition.y = addition.z = 0.0f;

		addition.x = in_lhs.x + in_rhs.x;
		addition.y = in_lhs.y + in_rhs.y;
		addition.z = in_lhs.z + in_rhs.z;

		return addition;
	}

	XMFLOAT3 operator += ( XMFLOAT3 in_lhs , XMFLOAT3 in_rhs )
	{
		// var1 += 1;
		// operator += ( var1 , var 2 )

		XMFLOAT3 add_and_assign = in_lhs; // compound_assignment

		add_and_assign.x += in_rhs.x;
		add_and_assign.y += in_rhs.y;
		add_and_assign.z += in_rhs.z;

		return add_and_assign;
	}

	XMFLOAT3 operator /= ( XMFLOAT3 in_lhs , float divisor )
	{
		XMFLOAT3 divide_and_assign = in_lhs;

		divide_and_assign.x /= divisor;
		divide_and_assign.y /= divisor;
		divide_and_assign.z /= divisor;

		return divide_and_assign;
	}
}