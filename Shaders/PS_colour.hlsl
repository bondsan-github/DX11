struct input_PS
{
	float4 position	: SV_POSITION;
    float4 colour	: COLOR;
	float2 uv		: TEXCOORD;
};

Texture2D in_texture : register( t0 );

/*
 SamplerType [in] The sampler type, which is one of the following: 
 sampler, sampler1D, sampler2D, sampler3D, samplerCUBE, sampler_state, SamplerState.
 Direct3D 10 and later supports: SamplerComparisonState. 
*/

//SamplerState sampler0 : register( s0 );
/*{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressV = WRAP; }
*/

float4 main_PS( input_PS input ) : SV_TARGET
{
    //float4 texture0 = in_texture.Sample( sampler0, input.uv );

    //float4 texture0 = input.colour;
    //texture0.a = 0.3;
    
	//return input.colour;
    //return texture0;
	return float4( 1.0f, 1.0f, 1.0f, 0.0f );
}

/*
 Pixel shader must always have float4 pos : SV_POSITION 
 semantics for input position of pixel( It is a one of the System Value semantics ).

 The output value that will be stored in a render target.
 The index indicates which of the 8 possibly bound render targets to write to.
 The value is available to all shaders. 
*/
