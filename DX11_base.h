#pragma once
// TAB width is 4
#include <map>
#include <memory>

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment( lib , "d3d11.lib" )
//#pragma comment( lib , "dxgi.lib" )
//#pragma comment( lib , "dinput8.lib" )
#include <d3dcompiler.h>
#pragma comment( lib , "d3dcompiler.lib" )

#include "debugging.h"
#include "input_layouts.h"
//#include "window.h"
#include "Mesh.h"
//#include "Vertex.h"
#include "Timer.h"

#include "Keyboard.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;
/*
class Game_DX11
{
	private:
		
		class graphics_manager
			DX11_Device;
			DX11_setup;
			
		class sound_manager

		class windows_message_handler
			create window
			message_loop;
};
*/

//template< typename vertex_t >
class DX11 //abstract
{
	public:
		//DX11( ); // override default constructor

		DX11( const HINSTANCE h_win_instance , const UINT window_width = 800u , const UINT window_height = 600u ); // int window_display_options );
		//~DX11();  // virutal = this and derived classes, member data destruction
		
		WPARAM	message_loop();
		LRESULT window_messaging( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam );
		HWND	create_window( const HINSTANCE in_h_instance , const UINT in_client_width , const UINT in_client_height , RECT * out_client_size ); // int window_display_options )

		void	create_dx11_device();

		void	window_size_update();
		void	gpu_device_lost();

		//virtual void on_key_down( const WPARAM wParam , const LPARAM lParam ) {}
		//void input_keyboard();

		// void load_shader(LCTSTR filename);
		// LPCSTR Type can be either unicode or ANSI
		//HRESULT compile_shader( LPCTSTR file_path , LPCSTR entry , LPCSTR shader_model , ID3DBlob** buffer );

		//void load_vertex_shader( const LPWSTR filename );
		//void load_pixel_shader( const LPWSTR filename );

		//virtual bool load_content( );
		//virtual void unload_content( );

		//ID3D11Device * const get_p_video_device( void ) { return m_p_video_device; }; 

		//Microsoft::WRL::ComPtr<ID3D11Device> & get_p_video_device()	{ return m_p_video_device; };

		//Microsoft::WRL::ComPtr<ID3D11DeviceContext> & get_p_video_device_context() { return m_p_video_device_context; }

		void clear();
		void clear( const XMFLOAT4 in_colour );

		void present();

		//virtual void load_content();
		virtual void update( const double time_delta ) = 0;	// pure virtual must be implemented // float delta_time
		virtual void render() = 0;

		const RECT get_client_area() const { return m_client_area; }

		std::unique_ptr< Keyboard > m_keyboard = std::make_unique< Keyboard >();

		//m_mouse = std::make_unique<Mouse>();
		//m_mouse->SetWindow( window );

	private:
	
		void create_swap_chain();
		void create_render_target_view();

		void create_depth_stencil();
		void create_depth_stencil_view();

		HINSTANCE					m_instance { };
		HWND						m_window { };
		int							m_window_display_options { };
		unsigned long				m_window_styles {};
		unsigned int				m_window_width { 800 };
		unsigned int				m_window_height { 600 };		

		RECT						m_client_area {};

		D3D_DRIVER_TYPE				m_driver_type { D3D_DRIVER_TYPE_HARDWARE };
		D3D_FEATURE_LEVEL			m_feature_level { D3D_FEATURE_LEVEL_9_1 };

		unsigned int				m_swap_chain_count { 1 };
		DXGI_FORMAT					m_swap_chain_format { DXGI_FORMAT_R8G8B8A8_UNORM };
		DXGI_SWAP_CHAIN_DESC		m_swap_chain_description { };
		// 32 bit unsigned normalized integer format that supports 8 bits per channel including alpha.

		DXGI_FORMAT					m_depth_buffer_format { DXGI_FORMAT_D24_UNORM_S8_UINT };
		D3D11_TEXTURE2D_DESC		m_depth_texture_description { };

		// directxtk.codeplex.com/wikipage?title=ComPtr

		// "ComPtr is initially null."
		ComPtr< ID3D11Device >				m_video_device;
		ComPtr< ID3D11DeviceContext >		m_video_device_context;

		ComPtr< ID3D11Debug >				m_DX_debug;

		//DXGI_SWAP_CHAIN_DESC				m_struct_swap_chain_description{};
		ComPtr< IDXGISwapChain >			m_swap_chain;

		ComPtr< ID3D11Texture2D >			m_render_target_texture;
		ComPtr< ID3D11RenderTargetView >	m_render_target_view;
		float								m_back_clear_colour[ 4 ] { 0.0f , 0.0f , 0.5f , 1.0f };

		D3D11_DEPTH_STENCIL_VIEW_DESC		m_depth_stencil_view_description { };
		//D3D11_TEXTURE2D_DESC				m_struct_depth_texture_description {};
		ComPtr< ID3D11Texture2D >			m_depth_stencil_texture;
		ComPtr< ID3D11DepthStencilView >	m_depth_stencil_view;

		D3D11_RASTERIZER_DESC				m_rasteriser_description {};
		ComPtr< ID3D11RasterizerState >		m_rasteriser_state;

		ComPtr< ID3D11VertexShader >		m_vertex_shader;
		ComPtr< ID3D11PixelShader >			m_pixel_shader;

		ComPtr< ID3D11InputLayout >			m_input_layout;
		ComPtr< ID3D11Buffer >				m_vertex_buffer;

		D3D11_SAMPLER_DESC					m_struct_sampler_description {};
		ComPtr< ID3D11SamplerState >		m_sampler_state;

		D3D11_BLEND_DESC					m_blend_descripton { };
		ComPtr< ID3D11BlendState >			m_blend_state;

		float								m_blend_factor[ 4 ] { 1.0f , 1.0f , 1.0f , 1.0f };
		unsigned int						m_sample_mask = 0xffffffff;

		Timer m_timer;


};

/*
D3D_DRIVER_TYPE driver_types[ ] =
{
	D3D_DRIVER_TYPE_HARDWARE,
	D3D_DRIVER_TYPE_WARP,
	D3D_DRIVER_TYPE_REFERENCE,
	D3D_DRIVER_TYPE_SOFTWARE
};

D3D_FEATURE_LEVEL feature_levels[ ] =
{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1
};
*/
