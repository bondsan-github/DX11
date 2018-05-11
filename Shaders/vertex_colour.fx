/*
  Solid vertex colour 
*/

//-----------------------------------------------------
// Constant buffer variables
//-----------------------------------------------------

// ? seperate buffers based on update frequency
cbuffer constant_buffer_model : register( b0 )
{
    matrix cb_world;
}

cbuffer buffer_update_static : register( b1 ) 
{ 
    matrix cb_view; 
}

cbuffer buffer_update_OnResize : register( b2 ) 
{ 
    matrix cb_projection; 
}

//-----------------------------------------------------
// stage communication structures
//-----------------------------------------------------
struct VS_OUTPUT
{
   float4 position : SV_POSITION;  // SV = ? Stage Vertex
   float4 colour   : COLOR;
};

//-----------------------------------------------------
// Vertex Shader
//-----------------------------------------------------
VS_OUTPUT vertex_shader( float4 position : POSITION,
                         float4 colour   : COLOR ) 
{
   VS_OUTPUT VS_output = (VS_OUTPUT)0;

   VS_output.position = mul( position,           cb_world ); // World transform

   VS_output.position = mul( VS_output.position, cb_view );  // View transoform - ? clipping
   VS_output.position = mul( VS_output.position, cb_projection ); // Projection transform - projection or orthographic


   VS_output.colour = colour;

   return VS_output;
}


//-----------------------------------------------------
// Pixel Shader
//-----------------------------------------------------
float4 pixel_shader( VS_OUTPUT PS_input ) : SV_Target
{
   return PS_input.colour;
}