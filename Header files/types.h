#pragma once

typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned short	ushort;
typedef unsigned long	ulong;

//#include <d3dcommon.h>
//D3D_PRIMITIVE_TOPOLOGY triangle_list = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

enum class buffer_VS { VS_BUFFER_MESH = 0, VS_BUFFER_VIEW, VS_BUFFER_PROJECTION };
enum { VS_BUFFER_MESH = 0 , VS_BUFFER_CAMERA_VIEW , VS_BUFFER_CAMERA_PROJECTION };