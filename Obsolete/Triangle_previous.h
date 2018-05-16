#pragma once

#include "Mesh.h"

struct triangle_indices
{
	//public:

	triangle_indices()
		: index0( 0 ) , index1( 0 ) , index2( 0 )
	{ }

	triangle_indices( const unsigned int in_index0 , const unsigned int in_index1 , const unsigned int in_index2 )
		: index0( in_index0 ) , index1( in_index1 ) , index2( in_index2 )
	{ }

	//private:
	unsigned long index0 , index1 , index2; // short = 16-bit signed - unsigned = MAX 32767 // unsigned int = 32-bit  MAX =
};

template< typename vertex_t >
struct triangle_vertices
{
	vertex_t vertex1 , vertex2 , vertex3;

	triangle_vertices()
	{
		//vertex1 = vertex2 = vertex3 = {};
	}

	/*
	triangle_vertices( const TYPE in_vertex0, const TYPE in_vertex1, const TYPE in_vertex2 )
	: vertex0( in_vertex0 ),
	vertex1( in_vertex1 ),
	vertex2( in_vertex2 )
	{ }
	*/
};
