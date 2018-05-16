
template< typename TYPE >
HRESULT Mesh::create_and_fill_buffer( const D3D11_BIND_FLAG flags_bind, const std::vector< TYPE > * const vector_data )
{ 
    HRESULT h_result;

    D3D11_BUFFER_DESC buffer_description {};
    buffer_description.ByteWidth         = sizeof( TYPE ) * get_total_indices(); // buffer size in bytes
    buffer_description.Usage             = D3D11_USAGE_DEFAULT;
    buffer_description.BindFlags         = flags_bind;    
    // index_description.CPUAccessFlags    = 0;

    std::vector< unsigned int >::const_pointer p_vector_data = & buffer_indices[0];

    D3D11_SUBRESOURCE_DATA resource_data {};
    resource_data.pSysMem  = p_vector_data; // Pointer to the initialisation data.

    switch( flags_bind )
    {
        case: D3D11_BIND_VERTEX_BUFFER
            h_result = p_video_device->CreateBuffer( & buffer_description, & resource_data, & p_buffer_vertex );
            break;

        case: D3D11_BIND_INDEX_BUFFER
            h_result = p_video_device->CreateBuffer( & buffer_description, & resource_data, & p_buffer_index );
            break;
    }
    
    if( FAILED( h_result ) ) { OutputDebugString( L"Failed to create indices buffer\n" ); return h_result; }

    return h_result;
}