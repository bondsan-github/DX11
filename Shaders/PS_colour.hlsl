

struct PS_Input
{
	float4 position			: SV_POSITION;
    float4 vertex_colour    : COLOR;
	float2 texture_uv_0    	: TEXCOORD;
};

Texture2D		in_texture		: register( t0 );

/*
 SamplerType [in] The sampler type, which is one of the following: 
 sampler, sampler1D, sampler2D, sampler3D, samplerCUBE, sampler_state, SamplerState.
 Direct3D 10 and later supports: SamplerComparisonState. 
*/

SamplerState	sampler0 : register( s0 );
/*{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressV = WRAP; }
*/

float4 PS_Main( PS_Input input ) : SV_TARGET
{
    float4 texture0 = in_texture.Sample( sampler0, input.texture_uv_0 );

    //texture0 += input.vertex_colour;
    //texture0.a = 0.3;
    
    return texture0;
}

/*
 Pixel shader must always have float4 pos : SV_POSITION 
 semantics for input position of pixel( It is a one of the System Value semantics ).

 The output value that will be stored in a render target.
 The index indicates which of the 8 possibly bound render targets to write to.
 The value is available to all shaders. 
*/
