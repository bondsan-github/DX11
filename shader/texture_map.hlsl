/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    Texture Mapping Shader
*/


Texture2D colour_map : register( t0 );
SamplerState colour_map_sampler : register( s0 );


struct VS_Input
{
    float4 position : POSITION;
    float2 texture0 : TEXCOORD0;
};

struct PS_Input
{
    float4 position : SV_POSITION;
    float2 texture0 : TEXCOORD0;
};


PS_Input VS_Main( VS_Input vertex )
{
    PS_Input vsOut = ( PS_Input )0;
    vsOut.position = vertex.position;
    vsOut.texture0 = vertex.texture0;

    return vsOut;
}


float4 PS_Main( PS_Input frag ) : SV_TARGET
{
    return colour_map.Sample( colour_map_sampler, frag.texture0 );
}