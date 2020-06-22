#pragma once

#include <memory>

#include <d3d11.h>
#pragma comment( lib , "d3d11.lib" )

#include <d3dcompiler.h>
#pragma comment( lib , "d3dcompiler.lib" )

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#include <DirectXColors.h>

#include "debugging.h"
#include "input_layouts.h"

//using input_layout = input_layout_xyz_uv;

class Graphics
{
	public:

		Graphics() {};// = default;
		~Graphics();

		void initialise(const HWND in_window);
		void window_size_update();

		void clear_render_target( const DirectX::XMVECTORF32 in_colour = DirectX::Colors::DarkBlue);
		void present();

		ComPtr< ID3D11Device > get_video_device() const { return video_device; }

	private:

		void create_dx11_device();
		void create_swap_chain();
		void create_render_target_view();
		void create_depth_stencil();
		void create_depth_stencil_view();
		void gpu_device_lost();

	private:

		HRESULT							result { E_FAIL };
		HWND							window {};
		RECT							client_size {};

		ComPtr< ID3D11Device >			video_device;
		ComPtr< ID3D11DeviceContext >	video_device_context;
		D3D_FEATURE_LEVEL				feature_level;

		unsigned int					creation_flags {};

		const D3D_FEATURE_LEVEL			feature_levels[6]
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};
		
		unsigned int						swap_chain_count { 1 };
		DXGI_FORMAT							swap_chain_format { DXGI_FORMAT_R8G8B8A8_UNORM }; // _UINT;
		DXGI_SWAP_CHAIN_DESC				swap_chain_description {};
		ComPtr< IDXGISwapChain >			swap_chain;
		ComPtr< ID3D11Texture2D >			swap_chain_target;
		ComPtr< ID3D11RenderTargetView >	swap_chain_view;

		D3D11_TEXTURE2D_DESC				depth_texture_description {};
		DXGI_FORMAT							depth_buffer_format { DXGI_FORMAT_D24_UNORM_S8_UINT };
		ComPtr< ID3D11Texture2D >			depth_stencil_texture;
		
		D3D11_DEPTH_STENCIL_VIEW_DESC		depth_stencil_view_description {};
		ComPtr< ID3D11DepthStencilView >	depth_stencil_view;

		D3D11_RASTERIZER_DESC				rasteriser_description {};
		bool								multi_sample_enable	{ false };
		ComPtr< ID3D11RasterizerState >		rasteriser_state;

		D3D11_VIEWPORT						viewport {};

		ComPtr< ID3DBlob >					d3d_blob;

		ComPtr< ID3D11VertexShader >		vertex_shader;
		ComPtr< ID3D11PixelShader >			pixel_shader;

		ComPtr< ID3D11InputLayout >			input_layout;

		D3D11_SAMPLER_DESC					sampler_description {};
		ComPtr< ID3D11SamplerState >		sampler_state;

		D3D11_BLEND_DESC					blend_descripton{};
		ComPtr< ID3D11BlendState >			blend_state;

		float								blend_factor[ 4 ] { 1.0f , 1.0f , 1.0f , 1.0f };
		unsigned int						sample_mask { 0xffffffff };
};

