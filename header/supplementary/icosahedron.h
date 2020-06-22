#pragma once

#include "debugging.h"
#include "Mesh.h"

#include <math.h>
#include <algorithm>
#include <random>

//template< typename TYPE >
//class Icosahedron : public Mesh< TYPE > 

template< typename vertex_t >
class Icosahedron : public Mesh//< vertex_t > //, public drawable()
{
    public:
        Icosahedron( const unsigned char tessalation );
        ~Icosahedron() {};     
};

#include "icosahedron.cpp"