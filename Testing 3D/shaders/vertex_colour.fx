/*
  Solid vertex colour 
*/

//---------------------------
// Constant buffer variables
//---------------------------


//--------------------------
struct VS_OUTPUT
{
   float4 position : SV_POSITION;
   float4 colour : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT vertex_shader( float4 position : POSITION,
                         float4 colour : COLOR   ) 
{
   VS_OUTPUT PS_input;

   PS_input.position = position;
   PS_input.colour = colour;//float4( 1.0f, 1.0f, 0.0f, 1.0f );

   return PS_input;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 pixel_shader( VS_OUTPUT PS_input ) : SV_Target
{
   return PS_input.colour;
}