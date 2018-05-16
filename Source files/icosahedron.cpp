//#include "..\Header files\icosahedron.h"

//Icosahedron< Tvertex >::Icosahedron( ID3D11Device * const p_video_device, unsigned char tessalation ) : Mesh< Tvertex >( p_video_device )

//template class Icosahedron< vertex >;

template< typename vertex_t >
Icosahedron< vertex_t >::Icosahedron( Microsoft::WRL::ComPtr< ID3D11Device > p_video_device, unsigned char tessalation )
	: Mesh< vertex_t >( p_video_device )
{
	vertex_t new_vertex;

    // set vertex buffer
    // 12 Vertices 

    float φ = ( 1.0f + sqrt(5.0f) ) / 2.0f; // φ - phi - golden ratio ~ 1.62 

    // (  0, ±1, ±φ )
    // ( ±1, ±φ,  0 )
    // ( ±φ,  0, ±1 )
    
	// remove colour

    //                                     position                      colour
    new_vertex.position = XMVectorSet(  0.0f,  1.0f,  φ,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 1.0f, 1.0f, 0.0f ); add_vertex( new_vertex );// 0
    new_vertex.position = XMVectorSet(  0.0f,  1.0f, -φ,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 0.0f, 1.0f, 0.0f ); add_vertex( new_vertex );// 1
    new_vertex.position = XMVectorSet(  0.0f, -1.0f,  φ,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 1.0f, 0.0f, 0.0f ); add_vertex( new_vertex );// 2
    new_vertex.position = XMVectorSet(  0.0f, -1.0f, -φ,  0.0f ); new_vertex.colour = XMVectorSet( 0.0f, 1.0f, 1.0f, 0.0f ); add_vertex( new_vertex );// 3 
       
    new_vertex.position = XMVectorSet(  1.0f,  φ,  0.0f,  0.0f ); new_vertex.colour = XMVectorSet( 0.0f, 1.0f, 1.0f, 0.0f ); add_vertex( new_vertex );// 4
    new_vertex.position = XMVectorSet(  1.0f, -φ,  0.0f,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 0.0f, 1.0f, 0.0f ); add_vertex( new_vertex );// 5
    new_vertex.position = XMVectorSet( -1.0f,  φ,  0.0f,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 1.0f, 0.0f, 0.0f ); add_vertex( new_vertex );// 6
    new_vertex.position = XMVectorSet( -1.0f, -φ,  0.0f,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 1.0f, 0.0f, 0.0f ); add_vertex( new_vertex );// 7

    new_vertex.position = XMVectorSet(  φ,  0.0f,  1.0f,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 0.0f, 1.0f, 0.0f ); add_vertex( new_vertex );// 8
    new_vertex.position = XMVectorSet(  φ,  0.0f, -1.0f,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 0.0f, 1.0f, 0.0f ); add_vertex( new_vertex );// 9
    new_vertex.position = XMVectorSet( -φ,  0.0f,  1.0f,  0.0f ); new_vertex.colour = XMVectorSet( 1.0f, 1.0f, 0.0f, 0.0f ); add_vertex( new_vertex );// 10
    new_vertex.position = XMVectorSet( -φ,  0.0f, -1.0f,  0.0f ); new_vertex.colour = XMVectorSet( 0.0f, 1.0f, 1.0f, 0.0f ); add_vertex( new_vertex );// 11

    // set index buffer
    // 20 Faces
    // at each vertex, 5 edges meet

    //   6 6 6 6 6 
    //   /\/\/\/ \/\ 
    //  1 4 0 10 11 1
    //  /\/\/\/\/\/
    // 3 9 8 2 7 3
    //  \/\/\/\/\/s
    //  5 5 5 5 5 

    std::vector< unsigned long > indices 
    { 
        1,6,4, 4,6,0, 0,6,10, 10,6,11, 11,6,1,
        3,1,9, 9,1,4, 9,4,8,  8,4,0,   8,0,2,  2,0,10, 2,10,7, 7,10,11, 7,11,3, 3,11,1,
        5,3,9, 5,9,8, 5,8,2,  5,2,7,   5,7,3
    };

	set_indices( indices );	

	indices.clear();

    //vertex_col_norm type_vertex;

	/*
    tessalate(1);
    tessalate(1);
    tessalate(1);
    tessalate(1);
	*/

    //calc_normals();

    //create_buffer_vertices();
    
    //create_buffer_indices();
}



