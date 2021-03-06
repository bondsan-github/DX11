#pragma once
// TAB width is 4
#include <map>
#include <memory>

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>

// check for processor support by calling XMVerifyCPUSupport() before using the DirectXMath Library.

// When you call XMVerifyCPUSupport, include <windows.h> before you include <DirectXMath.h>. 
// This is the only function in the library that requires any content from <windows.h> 
// so you aren't required to include <windows.h> in every module that uses <DirectXMath.h>.

// Windows 10 October 2018 Update SDK DirectXMath 3.13
#include <DirectXMath.h> // DIRECTX_MATH_VERSION 307

#pragma comment( lib , "d3d11.lib" )
//#pragma comment( lib , "dxgi.lib" )
//#pragma comment( lib , "dinput8.lib" )
#include <d3dcompiler.h>
#pragma comment( lib , "d3dcompiler.lib" )

#include "debugging.h"
#include "input_layouts.h"
//#include "window.h"
#include "mesh.h"
//#include "Vertex.h"
#include "timer.h"

#include "MS_keyboard.h"

using Microsoft::WRL::ComPtr;
using DirectX::Keyboard;

class DX11 abstract
{
	public:
		//DX11( ); // over-ride default constructor

		DX11( const HINSTANCE h_win_instance , const UINT window_width = 800u , const UINT window_height = 600u ); // int window_display_options );
																											   //~DX11();  // virutal = this and derived classes, member data destruction

		WPARAM	message_loop();
		LRESULT window_messaging( HWND hwnd , unsigned int msg , WPARAM wParam , LPARAM lParam );
		HWND	create_window( const HINSTANCE in_h_instance , const unsigned int in_client_width , const unsigned int in_client_height , RECT * out_client_size ); // int window_display_options )

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
		//ComPtr<ID3D11Device> & get_p_video_device()	{ return m_p_video_device; };
		//ComPtr<ID3D11DeviceContext> & get_p_video_device_context() { return m_p_video_device_context; }

		void clear();
		void clear( const XMFLOAT4 in_colour );

		void present();

		//virtual void load_content();
		virtual void update( const long double time_delta ) = 0;	// pure virtual must be implemented // float delta_time
		virtual void render() = 0;

		const RECT get_client_area() const { return client_area; }

		std::unique_ptr< Keyboard > keyboard = std::make_unique< Keyboard >();

		//m_mouse = make_unique<Mouse>();
		//m_mouse->SetWindow( window );

		const HWND get_window_handle() const
		{
			return window;
		}

	private:

		HRESULT result { E_FAIL };

		void create_swap_chain();
		void create_render_target_view();

		void create_depth_stencil();
		void create_depth_stencil_view();

		HINSTANCE					instance {};
		HWND						window {};
		int							window_display_options {};
		unsigned long				window_styles {};
		unsigned int				window_width { 800 };
		unsigned int				window_height { 600 };

		RECT						client_area {};

		D3D_DRIVER_TYPE				driver_type { D3D_DRIVER_TYPE_HARDWARE };
		D3D_FEATURE_LEVEL			feature_level { D3D_FEATURE_LEVEL_9_1 };

		unsigned int				swap_chain_count { 1 };
		DXGI_FORMAT					swap_chain_format { DXGI_FORMAT_R8G8B8A8_UNORM };
		DXGI_SWAP_CHAIN_DESC		swap_chain_description {};
		// 32 bit unsigned normalized integer format that supports 8 bits per channel including alpha.

		DXGI_FORMAT					depth_buffer_format { DXGI_FORMAT_D24_UNORM_S8_UINT };
		D3D11_TEXTURE2D_DESC		depth_texture_description {};

		// directxtk.codeplex.com/wikipage?title=ComPtr

		// "ComPtr is initially null."
		ComPtr< ID3D11Device >				video_device;
		ComPtr< ID3D11DeviceContext >		video_device_context;

		ComPtr< ID3D11Debug >				DX_debug;

		//DXGI_SWAP_CHAIN_DESC				swap_chain_description {};
		ComPtr< IDXGISwapChain >			swap_chain;

		ComPtr< ID3D11Texture2D >			render_target_texture;
		ComPtr< ID3D11RenderTargetView >	render_target_view;
		float								back_clear_colour[ 4 ] { 0.0f , 0.0f , 0.5f , 1.0f };
		
		D3D11_DEPTH_STENCIL_VIEW_DESC		depth_stencil_view_description {};
		//D3D11_TEXTURE2D_DESC				depth_texture_description {};
		ComPtr< ID3D11Texture2D >			depth_stencil_texture;
		ComPtr< ID3D11DepthStencilView >	depth_stencil_view;

		D3D11_RASTERIZER_DESC				rasteriser_description {};
		ComPtr< ID3D11RasterizerState >		rasteriser_state;

		ComPtr< ID3D11VertexShader >		vertex_shader;
		ComPtr< ID3D11PixelShader >			pixel_shader;

		ComPtr< ID3D11InputLayout >			input_layout;

		D3D11_SAMPLER_DESC					sampler_description {};
		ComPtr< ID3D11SamplerState >		sampler_state;

		D3D11_BLEND_DESC					blend_descripton{};
		ComPtr< ID3D11BlendState >			blend_state;

		float								blend_factor[ 4 ] { 1.0f , 1.0f , 1.0f , 1.0f };
		unsigned int						sample_mask = 0xffffffff;
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
