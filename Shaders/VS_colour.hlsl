/*
cbuffer cm_matrix_WVP : register( b0 )
{
	matrix m_mesh;
	matrix m_view;
	matrix m_projection;
};
*/
cbuffer cb_world			: register( b0 )// VS_BUFFER_MESH
{
	matrix matrix_world;
};

cbuffer cb_view			: register( b1 )// VS_BUFFER_VIEW
{
	matrix matrix_view;
};

cbuffer cb_projection    : register( b2 ) // VS_BUFFER_PROJECTION
{
    matrix matrix_projection;
};

struct VS_input //VS_XYZ_RGBA_UV
{
	float4 position				: POSITION;
    float4 colour               : COLOR;
    float2 texture_uv_0         : TEXCOORD;
};

// struct VS_XYZ_UV

struct VS_output // Pixel shader input
{
	float4 position				: SV_POSITION;
    float4 colour               : COLOR;
    float2 texture_uv_0         : TEXCOORD;

};

// Vertex shader outputs data to Pixel shader using VS_output structure
VS_output VS_Main( VS_input input )
{
	VS_output output = ( VS_output ) 0;

	//output.position = vertex.position;
	//output.position = mul( vertex.position , matrix_WVP );

	output.position = mul( input.position , matrix_world );		// float4( vertex.position , 1.0f );

	output.position = mul( output.position , matrix_view );
	output.position = mul( output.position , matrix_projection );

    output.texture_uv_0 = input.texture_uv_0;

    output.colour = input.colour;

	return output;
}
