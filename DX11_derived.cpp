include "DX11_derived.h"

DX11_derived::DX11_derived( HINSTANCE h_instance , HWND h_window ) : DX11( h_instance, h_window )
{
    //MessageBox( nullptr, L"Derived constructor", L"Error", MB_OK );   
}

DX11_derived::~DX11_derived() 
{
    //delete cube;
    delete icosphere;

    unload_content();
}

bool DX11_derived::load_content( ) 
{
    // std::string directory_shaders = "..\\shaders\\";

    ID3DBlob * vertex_shader_buffer = nullptr;

    // C:\Program Files (x86)\Windows Kits\8.1\bin\x86\fxc.exe /T vs_4_0 /E VS_Main D:\Programming\Projects\DX11GameLib\Version1\texture_map.fx

    // Compile the vertex shader
    bool compile_result = compile_shader( L"..\\shaders\\vertex_colour.fx", "vertex_shader", "vs_4_0", &vertex_shader_buffer );

    if( compile_result == false  ) { OutputDebugString( L"compile_shader error\n" ); return false; }

    HRESULT h_result;

    // Create the vertex shader
    h_result = m_p_video_device->CreateVertexShader( vertex_shader_buffer->GetBufferPointer( ),   // A pointer to the compiled shader
                                                   vertex_shader_buffer->GetBufferSize( ),      // Size of the compiled vertex shader
                                                   nullptr,                                     // A pointer to a class linkage interface, the value can be NULL
                                                   &vertex_shader_solid_colour );               // Address of a pointer to a ID3D11VertexShader interface

    if( FAILED( h_result ) ) { OutputDebugString( L"CreateVertexShader error\n" ); if( vertex_shader_buffer ) vertex_shader_buffer->Release( ); return false; }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC description_input_layout[] =
    {
        { 
            "POSITION",                   // The *HLSL semantic* (keyword) associated with this element in a shader input-signature
            0,                            // The semantic index for the element
            DXGI_FORMAT_R32G32B32_FLOAT,  // The data type of the element data
            0,                            // An integer value that identifies the input-assembler. Valid values are between 0 and 15
            0,                            // 32bits / 8  Optional. Offset (in bytes) between each element
            D3D11_INPUT_PER_VERTEX_DATA,  // Identifies the input data class for a single input slot
            0                             // The number of instances to draw using the same per-instance data 
        },
        { 
            "COLOR", 
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            12,                          // Since an XMFLOAT3 is 4bytes x 3=12 bytes in size , colour coordinates won’t appear in vertex until after 12th byte                                         
            D3D11_INPUT_PER_VERTEX_DATA,
            0 
        },
        {
            "NORMAL",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            12 + 16,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    unsigned int total_layout_elements = ARRAYSIZE( description_input_layout );

    // Create the input layout
    h_result = m_p_video_device->CreateInputLayout( description_input_layout,                     // input-assembler stage input data types array
                                                  total_layout_elements,                        // Total input-data types in array of input-elements
                                                  vertex_shader_buffer->GetBufferPointer( ),    // compiled shader pointer
                                                  vertex_shader_buffer->GetBufferSize( ),       // size of compiled shader
                                                  &input_layout );                              // output pointer to created input-layout object

    if( FAILED( h_result ) ) { OutputDebugString( L"CreateInputLayout error" ); return false; }

    // Set the input layout
    // Bind an input-layout object to the input-assembler stage
	m_p_video_device_context->IASetInputLayout( input_layout );

    // Compile the pixel shader
    ID3DBlob * pixel_shader_buffer = nullptr;

    h_result = compile_shader( L"..\\shaders\\vertex_colour.fx", "pixel_shader", "ps_4_0", &pixel_shader_buffer );

    if( ! h_result ) { OutputDebugString( L"compile_shader error\n" ); return false; }

    // Create the pixel shader
    h_result = m_p_video_device->CreatePixelShader( pixel_shader_buffer->GetBufferPointer( ), // compiled shader pointer
                                                  pixel_shader_buffer->GetBufferSize( ),    // size of compiled pixel shader
                                                  nullptr,                                  // class linkage interface pointer, value can be NULL
                                                  &pixel_shader_solid_colour );             // ID3D11PixelShader interface pointer address

    pixel_shader_buffer->Release( );

    if( FAILED( h_result ) ) { OutputDebugString( L"CreatePixelShader error\n" ); return false; }
        

    //cube = new Cube( p_video_device );
    icosphere = new Icosahedron( m_p_video_device, 2 );
    //icosphere->tessalte();

    camera = new Camera();

    XMVECTORF32 pos = { 0.0f,  2.0f, -2.5f, 1.0f };

    camera->set_position( pos );

    
    //cube->translate( 1.0f,  0.0f, 0.0f );

    return true;
}

void DX11_derived::unload_content( ) 
{
	// RELEASE( ) macro
    if( diffuse_map_sampler ) diffuse_map_sampler->Release(); diffuse_map_sampler = nullptr;

    if( vertex_shader_solid_colour )    vertex_shader_solid_colour->Release( );
    if( pixel_shader_solid_colour )     pixel_shader_solid_colour->Release( );

    if( input_layout )                  input_layout->Release( );
    if( vertex_buffer )                 vertex_buffer->Release( );    

    vertex_shader_solid_colour  = nullptr;
    pixel_shader_solid_colour   = nullptr;

    input_layout                = nullptr;
    vertex_buffer               = nullptr;

    //OutputDebugString( L"unload_content finished\n" );
}

void DX11_derived::update( float delta_time )
{
    //static float position = 0.0f;
    //static float rotation = 0.0f;

    //XMVECTORF32 position = { position,  1.0f, -1.0f, 0.0f };
    //XMVECTORF32 target = { position,  0.0f, 0.0f, 0.0f };

    //camera->set_position( position );
    //camera->set_target( target );

    //cube->rotate( 0.0f, rotation, 0.0f );

    icosphere->rotate( 0, 0.0005, 0 ); // ( 0, 1 rps, 0 )

    //position += 0.0001f;
    //rotation += ;
}

void DX11_derived::render( ) 
{
    if( m_p_video_device_context == nullptr ) return;

    // clear the back buffer 
    float clear_colour[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	m_p_video_device_context->ClearRenderTargetView( m_p_back_buffer_target, // render target pointer to the 
                                                   clear_colour );       // 4-component colour array color to fill // dx9 D3DCOLOR_RGBA() );

    // clear depth stencil
	m_p_video_device_context->ClearDepthStencilView( m_p_depth_stencil_view,
                                                   D3D11_CLEAR_DEPTH,
                                                   1.0f,
                                                   0 );    

    XMMATRIX matrix_world = icosphere->get_matrix_world();
    matrix_world = XMMatrixTranspose( matrix_world );


    XMMATRIX matrix_view = camera->get_matrix_view();

    matrix_view = XMMatrixTranspose( matrix_view );

    // --- update constant buffers from matriceis ---
	m_p_video_device_context->UpdateSubresource( m_p_constant_buffer_world, // destination resource pointer
                                               0,                       // destination subresource, zero-based
                                               nullptr,                 // box pointer - portion of destination subresource to copy data into
                                               & matrix_world,          // source data pointer in memory
                                               0,                       // size of one row of source data
                                               0 );                     // size of one depth slice of source data

	m_p_video_device_context->UpdateSubresource( m_p_constant_buffer_view, 0, nullptr, & matrix_view, 0, 0 );

	m_p_video_device_context->UpdateSubresource( m_p_constant_buffer_projection, 0, nullptr, & m_matrix_projection, 0, 0 );

    // --- attach constant buffers to FX buffers (vertex shader stage) ---
	m_p_video_device_context->VSSetConstantBuffers( 0,                            // begining of devices' constant buffers index array (zero-based)
                                                  1,                            // total buffers 
                                                  & m_p_constant_buffer_world );  // constant buffers array to video device

	m_p_video_device_context->VSSetConstantBuffers( 1, 1, & m_p_constant_buffer_view );

	m_p_video_device_context->VSSetConstantBuffers( 2, 1, & m_p_constant_buffer_projection );
    
    // --- set FX VS and PS shader stages ---
	m_p_video_device_context->VSSetShader( vertex_shader_solid_colour, nullptr, 0 );

	m_p_video_device_context->PSSetShader( pixel_shader_solid_colour, nullptr, 0 );

    // --- set FX pixel shader ---
    //p_video_device_context->PSSetShaderResources( 0, 1, &diffuse_map );
    //p_video_device_context->PSSetSamplers( 0, 1, &diffuse_map_sampler );

    //cube->draw();
    icosphere->draw();

	m_p_swap_chain->Present( 0, 0 );
}