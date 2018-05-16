#pragma once

#include <d3d11.h>
/*
D3D11_INPUT_ELEMENT_DESC

LPCSTR						Semantic name;			// The keyword associated with this element in a shader input-signature
UINT						Semantic index;			// The semantic index for the element
DXGI_FORMAT					Format;					// The data type of the element data
UINT						Input slot;				// An integer value that identifies the input-assembler. Valid values are between 0 and 15
UINT						Aligned byte offset;	// 32 bits / 8  Optional. Offset (in bytes) between each element
D3D11_INPUT_CLASSIFICATION	Input slot class;		// Identifies the input data class for a single input slot
UINT						Instance data step rate; // The number of instances to draw using the same per-instance data 

DXGI_FORMAT_R32G32B32A32_FLOAT = +12 = Since an XMFLOAT3 is 4 bytes x 3 = 12 bytes in size ,
DXGI_FORMAT_R8G8B8A8_UINT = +4
colour coordinates won’t appear in vertex until after 12th byte
DXGI_FORMAT_R32G32B32A32_FLOAT = +16 = XMFLOAT4 = 4 bytes (= 32bits) x 4 values = 16 bytes
*/

// Without const;
// "if you include the file in multiple compilation units, you are declaring a variable of that name in each unit"

const D3D11_INPUT_ELEMENT_DESC input_layout_xyz[ 1 ]
{
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,		D3D11_INPUT_PER_VERTEX_DATA,	0 }
};

const D3D11_INPUT_ELEMENT_DESC input_layout_xyz_rgba[ 2 ]
{
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,		D3D11_INPUT_PER_VERTEX_DATA,	0 },
	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,		D3D11_INPUT_PER_VERTEX_DATA,	0 }, // X change to 8bit per channel
};

const D3D11_INPUT_ELEMENT_DESC  input_layout_xyz_rgba_uv[ 3 ]
{
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,		D3D11_INPUT_PER_VERTEX_DATA,	0 },
	//{ "COLOR",		0,	DXGI_FORMAT_R8G8B8A8_UNORM,		0,	12,		D3D11_INPUT_PER_VERTEX_DATA,	0 },
	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,		D3D11_INPUT_PER_VERTEX_DATA,	0 }, // X change to 8bit per channel	
	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	12+16,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
};
