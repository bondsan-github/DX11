#pragma once

#include <map>

#include "Mesh.h"
#include "Camera.h"

using std::map;
using std::string;
using Microsoft::WRL::ComPtr;

class Graphics_manager
{
	public:
	Graphics_manager( ComPtr< ID3D11Device > const p_video_device );

	//add_model( new Cube("cube1", uint width 1) 
	// load( 

	//add_model( std::string, 
	
	// add_icosphere( location, radius, tesselation_amount, COLOUR | material() );

	void add_camera( string camera_name );
	void add_camera( string camera_name , XMFLOAT3 location , XMFLOAT3 look_at );
	void set_active_camera( string camera_name );

	// graphics_manager->get_camera("camera1")->move_to( XMVECTOR position );
	// graphics_manager->get_active_camera()->move_to( float x, float y, float z );

	// add_event()
	// event_oject { mesh, camera 
	// enent_action { update, delete, add, translate
	// event_arguments 
	// event_priority { 

	private:

	// create_graphics_device( D3D11 );

	ComPtr< ID3D11Device >				m_p_video_device;

	map< string , Camera >					m_map_cameras;
	map< string , Camera >::const_iterator	m_itr_active_camera; 

	map< string , Mesh< vertex_colour_texture > >	m_map_meshes;
};
