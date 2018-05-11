#include "Mesh.h"

template class Mesh< vertex >;
template class Mesh< vertex_texture >;

template< typename vertex_t >
Mesh< vertex_t >::Mesh( ID3D11Device * const p_video_device ) : m_p_video_device( p_video_device )
{
    m_matrix_world = XMMatrixIdentity();

    //initialise();
}

template< typename vertex_t >
Mesh< vertex_t >::~Mesh()
{
	// replace with safe_release;

    if( m_p_buffer_vertex )         m_p_buffer_vertex       ->Release();
    if( m_p_buffer_index )          m_p_buffer_index        ->Release();
    if( m_p_diffuse_map_sampler )   m_p_diffuse_map_sampler ->Release();
    if( m_p_diffuse_map )           m_p_diffuse_map         ->Release();
    
	m_p_buffer_vertex       = nullptr;
	m_p_buffer_index        = nullptr;
	m_p_diffuse_map_sampler = nullptr;
	m_p_diffuse_map         = nullptr;
	m_p_video_device        = nullptr;

    //OutputDebugString( L"~Mesh destructor finished\n");
}

template< typename vertex_t >
HRESULT Mesh< vertex_t >::initialise()
{
    HRESULT h_result = S_OK;

    //if( p_video_device == nullptr ) { MessageBox( nullptr, L"Mesh - video device null pointer\n", L"Error", MB_OK ); return E_INVALIDARG; }
    /*
    // texture (colour/diffuse_map)
    h_result = D3DX11CreateShaderResourceViewFromFile( p_video_device,  // A pointer to the device (see ID3D11Device) that will use the resource
                                                       L"decal.dds",    // Name of the file that contains the shader-resource view
                                                       NULL,            // Optional. Identifies the characteristics of a texture when the data processor is created
                                                       nullptr,         // Pointer to a thread-pump interface 
                                                       &diffuse_map,    // Address of a pointer to the shader-resource view
                                                       nullptr );       // A pointer to the return value. May be NULL
    

    if( FAILED( h_result ) ) { OutputDebugString( L"Failed to load texture image" ); return false; }

    D3D11_SAMPLER_DESC diffuse_map_desc;
    ZeroMemory( &diffuse_map_desc, sizeof( diffuse_map_desc ) );
    diffuse_map_desc.AddressU         = D3D11_TEXTURE_ADDRESS_WRAP;
    diffuse_map_desc.AddressV         = D3D11_TEXTURE_ADDRESS_WRAP;
    diffuse_map_desc.AddressW         = D3D11_TEXTURE_ADDRESS_WRAP;
    diffuse_map_desc.ComparisonFunc   = D3D11_COMPARISON_NEVER;
    diffuse_map_desc.Filter           = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    diffuse_map_desc.MaxLOD           = D3D11_FLOAT32_MAX;

    h_result = p_video_device->CreateSamplerState( &diffuse_map_desc, &diffuse_map_sampler );

    if( FAILED( h_result ) ) { OutputDebugString( L"Failed to create diffuse map sampler state" ); return h_result; }
    */
    

    return h_result; // S_OK;
}

template< typename vertex_t >
ID3D11DeviceContext * const Mesh< vertex_t >::get_context_video()
{ 
    ID3D11DeviceContext * m_p_video_device_context;

	m_p_video_device->GetImmediateContext( & m_p_video_device_context );

    return m_p_video_device_context;
}

// Adds a vertex if unique
// if( exists in vertex vector ) return existing index
// else { return new vertex index }
template< typename vertex_t >
const unsigned int Mesh< vertex_t >::add_vertex_unique( std::vector< vertex_t > in_vertex, const vertex_t new_vertex ) // remove pointer
{
    static unsigned long new_index = 0; //static_cast< unsigned int >( p_buffer->size() );

    // search for existing vertex ( position )
    std::vector< vertex_t >::const_iterator iterator_find = std::find( m_vector_vertices.begin(), m_vector_vertices.end(), new_vertex );
    
    if( iterator_find != m_vector_vertices.end() )
    {
        unsigned long found_index = iterator_find - m_vector_vertices.begin();

        //debug_out( "\n duplicate vertex at %u, new index %d", found_index, new_index );

        return found_index; // return existing vertex index (into buffer_vertices)
    }
    else
    {
		m_vector_vertices.push_back( new_vertex );
                
        return new_index++; // return current index position then increment
    }
}

template< typename vertex_t >
void Mesh< vertex_t >::add_vertex( const vertex_t new_vertex )
{
	m_vector_vertices.push_back( new_vertex );
}

template< typename vertex_t >
void Mesh< vertex_t >::set_indices( const std::vector< unsigned long > new_vector_index )
{
	m_vector_indices = new_vector_index;
}

template< typename vertex_t >
HRESULT Mesh< vertex_t >::create_buffer_vertices( )
{
    HRESULT h_result;

    D3D11_BUFFER_DESC buffer_description {};
    buffer_description.ByteWidth         = sizeof( vertex_t ) * static_cast< unsigned int >( m_vector_vertices.size() ); // buffer size in bytes
    buffer_description.Usage             = D3D11_USAGE_DEFAULT;
    buffer_description.BindFlags         = D3D11_BIND_VERTEX_BUFFER;    

    D3D11_SUBRESOURCE_DATA resource_data {};
    resource_data.pSysMem = m_vector_vertices.data();  // Pointer to the initialisation data.

	m_ulong_total_indices = static_cast< unsigned long >( m_vector_vertices.size() );

    h_result = m_p_video_device->CreateBuffer( & buffer_description, & resource_data, & m_p_buffer_vertex );

    if( FAILED( h_result ) )
	{
		debug_out("Failed to create vertices buffer\n");
		return h_result;
	}

    return h_result;
}

template< typename vertex_t >
HRESULT Mesh< vertex_t >::create_buffer_indices( )
{
	// see How to: Create an Index Buffer

    HRESULT h_result;

    D3D11_BUFFER_DESC buffer_description {};
    buffer_description.ByteWidth         = sizeof( unsigned int ) * static_cast< unsigned int >( m_vector_indices.size() ); // buffer size in bytes
    buffer_description.Usage             = D3D11_USAGE_DEFAULT;
    buffer_description.BindFlags         = D3D11_BIND_INDEX_BUFFER;    

    D3D11_SUBRESOURCE_DATA resource_data {};
    resource_data.pSysMem = m_vector_indices.data();  // Pointer to the initialisation data.

    //set_total_indices( static_cast< unsigned long >( m_vector_indices.size() ) );

	m_ulong_total_indices = static_cast< unsigned long >( m_vector_indices.size() );
    h_result = m_p_video_device->CreateBuffer( & buffer_description, & resource_data, & m_p_buffer_index );
    
    if( FAILED( h_result ) ) { OutputDebugString( L"Failed to create indices buffer\n" ); return h_result; }

    return h_result;
}

template< typename vertex_t >
void Mesh< vertex_t >::translate( float x, float y, float z )
{
	m_matrix_translate = XMMatrixTranslation( x, y, z );

	m_matrix_world *= m_matrix_translate;
}

template< typename vertex_t >
XMMATRIX Mesh< vertex_t >::get_matrix_world() //const
{
    //matrix_world = XMMatrixTranspose( matrix_world );  // [a b] = [a]   make matrix column/row major (convert from DX to GC format)
                                                         //         [b] 
    return m_matrix_world;
}

template< typename vertex_t >
void Mesh< vertex_t >::rotate( float x, float y, float z )
{
    m_matrix_rotate = XMMatrixRotationRollPitchYaw( x, y, z );
	m_matrix_world *= m_matrix_rotate;
}

template< typename vertex_t >
void Mesh< vertex_t >::draw_normals()
{
    // buffer normal_lines

    ID3D11DeviceContext * p_video_device_context;
    m_p_video_device->GetImmediateContext( & p_video_device_context );

    //create buffer p_buffer_centroids

    // p_video_device_context->IASetVertexBuffers( 0, 1, & p_buffer_normal_lines, 0, 0 );
    // p_video_device_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST ); 
    // p_video_device_context->Draw( list_centroids.total() );
}

template< typename vertex_t >
void Mesh< vertex_t >::draw()
{
    // D:\Programming\Source code\DirectXTK\Src\Model.cpp

    // d3dContext_->IASetInputLayout( inputLayout_ );

    ID3D11DeviceContext * p_video_device_context;
    m_p_video_device->GetImmediateContext( & p_video_device_context );
    
    // Set vertex buffer
    unsigned int strides[1] = { sizeof( vertex_t ) };
    unsigned int offsets[1] = { 0 };

    p_video_device_context->IASetVertexBuffers( 0,                  // The first input slot for binding.
                                                1,                  // The number of vertex buffers in the array.
                                                & m_p_buffer_vertex,   // A pointer to an array of vertex buffers 
                                                & strides[0],        // Pointer to an array of stride values
                                                & offsets[0] );      // Pointer to an array of offset values

    p_video_device_context->IASetIndexBuffer( m_p_buffer_index,       // A pointer to an ID3D11Buffer object   
                                              DXGI_FORMAT_R32_UINT, // A DXGI_FORMAT that specifies the format of the data in the index buffer - DXGI_FORMAT_R16_UINT or DXGI_FORMAT_R32_UINT
                                              0 );                  // Offset (in bytes) from the start of the index buffer to the first index to use.
                                            
    // Set primitive topology
    p_video_device_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ); 
                                                    //D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );// Points
                                                    //D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP );// Lines


    //p_video_device_context->VSSetShader( vertex_shader_solid_colour, 0, 0 );
    //p_video_device_context->PSSetShader( pixel_shader_solid_colour, 0, 0 );
    //p_video_device_context->PSSetShaderResources( 0, 1, &diffuse_map );
    //p_video_device_context->PSSetSamplers( 0, 1, &diffuse_map_sampler );


    p_video_device_context->DrawIndexed( m_ulong_total_vertices , // Number of index's to draw.
                                         0,                   // The location of the first index read by the GPU from the index buffer
                                         0);                  // A value added to each index before reading a vertex from the vertex buffer

    //p_video_device_context->Draw( get_total_vertices(), 0 );

    p_video_device_context->Release();
    p_video_device_context = nullptr;
}

template< typename vertex_t >
const XMVECTOR Mesh< vertex_t >::calc_normal( const vertex_t vector0, const vertex_t vector1, const vertex_t vector2 )
{
    XMVECTOR u = vector1.position - vector0.position;
    XMVECTOR v = vector2.position - vector0.position;

    return XMVector3Cross( u, v );
}

template< typename vertex_t >
void Mesh< vertex_t >::calc_normals()
{
    //for each 3 indices in list_indices

    std::vector< unsigned long >::iterator iterator_index = m_vector_indices.begin();
    
    //for( ; iterator_vertex != list_vertices.end(); iterator_vertex += 3 )

    while( iterator_index != m_vector_indices.end() )
    {
		vertex_t vertex_a = m_vector_vertices.at( *( iterator_index + 0 ) ); // 1 = index first loop 
		vertex_t vertex_b = m_vector_vertices.at( *( iterator_index + 1 ) ); // 6
		vertex_t vertex_c = m_vector_vertices.at( *( iterator_index + 2 ) ); // 4

        XMVECTOR normal = calc_normal( vertex_a, vertex_b, vertex_c );

		vertex_t centroid;

        centroid.position   = calc_centriod( vertex_a, vertex_b, vertex_c );
        centroid.normal     = normal;
        //centroid.normal_end_position = position * normal
        centroid.colour     = XMVectorSet( 1.0f, 1.0f, 1.0f, 0.0f );

		m_vector_centroids.push_back( centroid );
        // list_centroids.push_back( normal * 10 );

		m_vector_vertices.at( *( iterator_index + 0 ) ).normal += normal;
		m_vector_vertices.at( *( iterator_index + 1 ) ).normal += normal;
		m_vector_vertices.at( *( iterator_index + 2 ) ).normal += normal;

        // add_centroid( centriod );
        // line_end = face normal 

        iterator_index += 3;
    }

    std::vector< vertex_t >::iterator iterator_vertices = m_vector_vertices.begin();

    while( iterator_vertices != m_vector_vertices.end() )
    {
        iterator_vertices->normal = XMVector3Normalize( iterator_vertices->normal );

        ++iterator_vertices;
    }
}

template< typename vertex_t >
const XMVECTOR Mesh< vertex_t >::calc_centriod( const vertex_t vector0, const vertex_t vector1, const vertex_t vector2 )
{
    XMVECTOR centroid_position {};

    centroid_position = ( vector0.position + vector1.position + vector2.position ) / 3.0f;
        
    return centroid_position;
}

template< typename vertex_t >
void Mesh< vertex_t >::add_triangle_tessalated( const triangle_indices new_triangle )
//void Mesh< TVertex >::add_triangle( std::vector< unsigned int > * const p_vector, const TVertex new_triangle )
{
	m_vector_indices_tessalated.push_back( new_triangle.index0 );
	m_vector_indices_tessalated.push_back( new_triangle.index1 );
	m_vector_indices_tessalated.push_back( new_triangle.index2 );
}

/*
template< typename vertex_t >
void Mesh< vertex_t >::tessalate(const unsigned char iterations)
{
    std::vector< unsigned int >::const_iterator const_iterator_indices = list_indices.begin();

    // for all triangles in 'buffer_indices'
    while( const_iterator_indices != list_indices.end() ) // 0..60 indices
    {
        //      b
        //     / \
        //    d   e
        //   /     \
        //  a---f---c
        //
        // find midpoint d,e,f of each edge a-b, b-c, c-a

        // original vertex
		vertex_t vertex_a = m_vector_vertices.at( *( const_iterator_indices + 0 ) ); // 1 = index first loop 
		vertex_t vertex_b = m_vector_vertices.at( *( const_iterator_indices + 1 ) ); // 6
		vertex_t vertex_c = m_vector_vertices.at( *( const_iterator_indices + 2 ) ); // 4
        
        // new vertex
		vertex_t midpoint_d = get_midpoint( vertex_a, vertex_b );
		vertex_t midpoint_e = get_midpoint( vertex_b, vertex_c );
		vertex_t midpoint_f = get_midpoint( vertex_c, vertex_a );
               
        //      b        b---e---c
        //     / \        \ / \ /
        //    d---e        d---f
        //   / \ / \        \ /
        //  a---f---c        a
        //
        // = 6 vertices
               
        // change all vertices length, from origin to 1.0 
        vertex_a.position = XMVector3Normalize( vertex_a.position );
        vertex_b.position = XMVector3Normalize( vertex_b.position );
        vertex_c.position = XMVector3Normalize( vertex_c.position );

        midpoint_d.position = XMVector3Normalize( midpoint_d.position );
        midpoint_e.position = XMVector3Normalize( midpoint_e.position );
        midpoint_f.position = XMVector3Normalize( midpoint_f.position );

        // add new triangles : a,d,f  d,e,f  f,e,c  d,b,e
        triangle_indices indices_tessalated();
        
        indices_tessalated.index0 = add_vertex_unique( & list_vertices_tessalated, vertex_a   );
        indices_tessalated.index1 = add_vertex_unique( & list_vertices_tessalated, midpoint_d );
        indices_tessalated.index2 = add_vertex_unique( & list_vertices_tessalated, midpoint_f );

        // add indices to list_indices_tessalated
        add_triangle_tessalated( indices_tessalated );


        indices_tessalated.index0 = add_vertex_unique( & list_vertices_tessalated, midpoint_d );
        indices_tessalated.index1 = add_vertex_unique( & list_vertices_tessalated, midpoint_e );
        indices_tessalated.index2 = add_vertex_unique( & list_vertices_tessalated, midpoint_f );

        add_triangle_tessalated( indices_tessalated );


        indices_tessalated.index0 = add_vertex_unique( & list_vertices_tessalated, midpoint_f );
        indices_tessalated.index1 = add_vertex_unique( & list_vertices_tessalated, midpoint_e );
        indices_tessalated.index2 = add_vertex_unique( & list_vertices_tessalated, vertex_c   );

        add_triangle_tessalated( indices_tessalated );


        indices_tessalated.index0 = add_vertex_unique( & list_vertices_tessalated, midpoint_d );
        indices_tessalated.index1 = add_vertex_unique( & list_vertices_tessalated, vertex_b   );
        indices_tessalated.index2 = add_vertex_unique( & list_vertices_tessalated, midpoint_e );
               
        add_triangle_tessalated( indices_tessalated );

        const_iterator_indices += 3;
        
        // this.set_draw_type( D3D11_PRIMITIVE_TOPOLOGY = TRIANGLE_STRIPS )
        // update to Geometry shader HLSL
    }

    list_vertices = list_vertices_tessalated;
    list_indices  = list_indices_tessalated;
}
*/

template< typename vertex_t >
const vertex Mesh< vertex_t >::get_midpoint( const vertex_t first, const vertex_t second )
{
    vertex midpoint;

    midpoint.position = ( first.position + second.position ) / 2.0f;
    //midpoint.position = ( first.position + second.position ) / 2.0f;
    //midpoint.position = ( first.position + second.position ) / 2.0f;
    

    //XMVector3Length( temp );
   
    std::random_device rd;
    std::mt19937 generator( rd() );
    std::uniform_real_distribution< float > distribution( 0.0f, 1.0f );

    //midpoint.colour = { 1.0f, 1.0f, 1.0f, 0.0f };
    //midpoint.colour = XMVectorSet( distribution( generator ), distribution( generator ), distribution( generator ), 0.0f );

    return midpoint;
}

template< typename vertex_t >
const float Mesh< vertex_t >::square( const float number )
{
    return number * number;
}

template< typename vertex_t >
const float Mesh< vertex_t >::magnitude( const vertex_t vertex )
{
    // find magnitute(length) from origin to point
    //XMVector3LengthSq

    //float length = square( vertex.position.x ) + square( vertex.position.y ) + square( vertex.position.z );
    //length = sqrtf( length );

    return 0.0f;//length;
}


/*
template< typename TVertex >
const TVertex Mesh< TVertex >::normalise( const TVertex vertex )
{
    // normalise point = vector / ||vector||

    TVertex vertex_normalised;

    float length = magnitude( vertex );

    vertex_normalised.position.x = vertex.position.x / length;
    vertex_normalised.position.y = vertex.position.y / length;
    vertex_normalised.position.z = vertex.position.z / length;

    vertex_normalised.colour = vertex.colour;

    return vertex_normalised;
}
*/

/*template< typename TVertex >
HRESULT Mesh< TVertex >::create_and_fill_buffer( const D3D11_BIND_FLAG flags_bind, const std::vector< TVertex > * const p_vector_data )
{ 
    HRESULT h_result;

    D3D11_BUFFER_DESC buffer_description {};
    buffer_description.ByteWidth         = sizeof( TVertex ) * p_vector_data->size(); // buffer size in bytes
    buffer_description.Usage             = D3D11_USAGE_DEFAULT;
    buffer_description.BindFlags         = flags_bind;    
    // buffer_description_vertex.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ; 
    // buffer_description_vertex.MiscFlags           = 0; // D3D11_RESOURCE_MISC_GENERATE_MIPS;
    // buffer_description_vertex.StructureByteStride = 0; // = sizeof( TYPE ); // StructuredBuffer<MyStruct> mySB;

    D3D11_SUBRESOURCE_DATA resource_data {};
    resource_data.pSysMem = p_vector_data->data();  // Pointer to the initialisation data.
    //SysMemPitch       // UINT - Not used for vertex buffers.
    //SysMemSlicePitch  // UINT - Not used for vertex buffers.

    switch( flags_bind )
    {
        case D3D11_BIND_VERTEX_BUFFER:
            set_total_vertices( static_cast< unsigned int >( p_vector_data->size() ) );
            h_result = p_video_device->CreateBuffer( & buffer_description, & resource_data, & p_buffer_vertex );
            break;

        case D3D11_BIND_INDEX_BUFFER:
            set_total_indices( static_cast< unsigned int >( p_vector_data->size() ) );
            h_result = p_video_device->CreateBuffer( & buffer_description, & resource_data, & p_buffer_index );
            break;
    }
    
    if( FAILED( h_result ) ) { OutputDebugString( L"Failed to create indices buffer\n" ); return h_result; }

    return h_result;
}
*/

/*
 void Mesh::tessalte() // must be traiangle list
 {
    //HRESULT h_result;    

    std::vector< vertex_colour > mesh_tessalated;
        
    ID3D11DeviceContext * p_video_device_context;
    p_video_device->GetImmediateContext( &p_video_device_context);

    D3D11_MAPPED_SUBRESOURCE subresource_mapped {}; // void *pData; UINT RowPitch; UINT DepthPitch;

    p_video_device_context->Map( p_buffer_vertex, 0, D3D11_MAP_READ, D3D11_MAP_FLAG_DO_NOT_WAIT, & subresource_mapped );

    // copy vertex data // ? memcopy
    //mesh_tessalated = subresource_mapped.pData;

    p_video_device_context->Unmap( p_buffer_vertex, 0 );  
 }
 */

/*get_context_video()->UpdateSubresource( get_buffer_inidces(),// A pointer to the destination resource 
                                            0,// A zero-based index, that identifies the destination subresource
                                            nullptr,// A pointer to a box that defines the portion of the destination subresource to copy the resource data into
                                            indices,// A pointer to the source data in memory
                                            0,// The size of one row of the source data
                                            0 );// The size of one depth slice of source data*/