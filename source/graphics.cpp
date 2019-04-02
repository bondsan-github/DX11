#include "graphics.h"

void Graphics::initialise( const HWND in_window )
{
	window = in_window;

	create_dx11_device();
	create_swap_chain();
	window_size_update();
}

Graphics::~Graphics()
{
	//depth_stencil_view.Reset();
	//depth_stencil_texture.Reset();

	//sampler_state.Reset();
	//input_layout.Reset();
	//pixel_shader.Reset();
	//vertex_shader.Reset();
	//depth_stencil_view.Reset();
	//depth_stencil_texture.Reset();
	//swap_chain_view.Reset();
	//swap_chain_target.Reset();
	//swap_chain.Reset();

	////DX_debug.Reset();

	//video_device_context.Reset();
	//video_device.Reset();
}

void Graphics::create_dx11_device()
{
	#ifdef _DEBUG
		creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D11CreateDevice( nullptr ,					// A pointer to the video adapter to use when creating a device *****
					   D3D_DRIVER_TYPE_HARDWARE ,	// The D3D_DRIVER_TYPE, which represents the driver type to create
					   nullptr ,					// A handle to a DLL that implements a software rasterizer
					   creation_flags ,				// The runtime layers to enable
					   feature_levels ,				// pointer to D3D_FEATURE_LEVELs array
					   _countof( feature_levels ) ,	// The number of elements in D3D_FEATURE_LEVEL []
					   D3D11_SDK_VERSION ,			// The SDK version; use D3D11_SDK_VERSION
					   & video_device ,				// Returns the address of a pointer to an ID3D11Device
					   & feature_level ,			// Returns a pointer to a D3D_FEATURE_LEVEL
					   & video_device_context );	// Returns the address of a pointer to an ID3D11DeviceContext
}

void Graphics::create_swap_chain()
{
	ComPtr< IDXGIDevice1 > dxgi_device1;
	video_device.As( &dxgi_device1 );  // Retrieve the underlying DXGI Device from the D3D Device.

	ComPtr< IDXGIAdapter > dxgi_adapter;
	dxgi_device1->GetAdapter( dxgi_adapter.GetAddressOf() ); // Identify the physical adapter (GPU or card) this device is running on.

	ComPtr< IDXGIFactory1 > dxgi_factory1;
	dxgi_adapter->GetParent( IID_PPV_ARGS( dxgi_factory1.GetAddressOf() ) ); // Obtain the factory object that created it.



	GetClientRect( window , & client_size ); // upper-left corner = (0,0).

	swap_chain_description.BufferCount			= swap_chain_count;
	swap_chain_description.BufferDesc.Width		= client_size.right;
	swap_chain_description.BufferDesc.Height	= client_size.bottom;
	swap_chain_description.BufferDesc.Format	= swap_chain_format;

	//enumerate display modes -> if( v_sync ) Numerator = monitor refresh rate;
	swap_chain_description.BufferDesc.RefreshRate.Numerator		= 60;	// 60hz refresh rate
	swap_chain_description.BufferDesc.RefreshRate.Denominator	= 1;

	//swap_chain_description.BufferDesc.Scaling				= DXGI_MODE_SCALING_STRETCHED;
	//swap_chain_description.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;

	swap_chain_description.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow			= window;
	swap_chain_description.Windowed				= true;

	swap_chain_description.SampleDesc.Count		= 1;
	swap_chain_description.SampleDesc.Quality	= 0; // No multisampling
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // _SEQUENTIAL 

	dxgi_factory1->CreateSwapChain( video_device.Get() , &swap_chain_description , & swap_chain );

	// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT + ENTER shortcut.
	// DXGI_MWA_NO_ALT_ENTER - Prevent DXGI from responding to an alt-enter sequence. 
	// ensures that DXGI will not interfere with application's handling of window mode changes or Alt+Enter.

	dxgi_factory1->MakeWindowAssociation( window , DXGI_MWA_NO_ALT_ENTER );
	
}

void Graphics::create_render_target_view()
{
	// Obtain the backbuffer for this window which will be the final 3D render target.
	// Textures cannot be bound directly to the pipeline; instead, a view must be created and bound.

	// ComPtr< ID3D11Texture2D > swap_chain_texture;
	swap_chain->GetBuffer( 0 , IID_PPV_ARGS( & swap_chain_target ) );

	//------------ create_render_target_view ---------//
	// Create the render target view with the back buffer pointer.

	// resources must use a view to access resource data

	// A render-target is a resource that can be written by the output-merger stage at the end of a render pass. 
	// A render-target-view interface identifies the render-target subresources that can be accessed during rendering.
	// Each render-target should also have a corresponding depth-stencil view.

	video_device->CreateRenderTargetView( swap_chain_target.Get() ,	// ID3D11Resource * that represents a render target
										  nullptr ,					// D3D11_RENDER_TARGET_VIEW_DESC *
										  & swap_chain_view );		// ID3D11RenderTargetView &* 
}

void Graphics::create_depth_stencil()
{
	//------------ Create depth/stencil texture ---------//
	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.

	GetClientRect( window , & client_size );

	depth_texture_description.Width				= client_size.right;
	depth_texture_description.Height			= client_size.bottom;
	depth_texture_description.MipLevels			= 1;
	depth_texture_description.ArraySize			= 1;
	depth_texture_description.Format			= depth_buffer_format;
	depth_texture_description.SampleDesc.Count	= 1;
	depth_texture_description.SampleDesc.Quality = 0;
	depth_texture_description.Usage				= D3D11_USAGE_DEFAULT;
	depth_texture_description.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depth_texture_description.CPUAccessFlags	= 0;
	depth_texture_description.MiscFlags			= 0;

	video_device->CreateTexture2D( & depth_texture_description ,
									 nullptr ,
								   & depth_stencil_texture );
}

void Graphics::create_depth_stencil_view()
{
	
	depth_stencil_view_description.Format				= depth_buffer_format;
	depth_stencil_view_description.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_description.Texture2D.MipSlice	= 0;
	//D3D11_DSV_DIMENSION_TEXTURE2DMS = The resource will be accessed as a 2D texture with multisampling.

	video_device->CreateDepthStencilView( depth_stencil_texture.Get() ,
										  & depth_stencil_view_description ,
										  & depth_stencil_view );
}

void Graphics::window_size_update()
{
	// Allocate all memory resources that change on a window SizeChanged event.
	// Clear the previous window size specific context.

	video_device_context->Flush(); // send buffered commands GPU

	ID3D11RenderTargetView * null_target_views[] { nullptr };
	video_device_context->OMSetRenderTargets( _countof( null_target_views ) , null_target_views , nullptr ); // ARRAYSIZE

	swap_chain_view.Reset();	// ComPtr::Reset = Release all pointer references to the associated interface
	depth_stencil_view.Reset();
	
	// If the swap chain already exists, resize it, otherwise create one.
	if( swap_chain ) // .Get()? nope
	{
		HRESULT result { E_FAIL };
		
		GetClientRect( window , & client_size );

		swap_chain->ResizeBuffers( swap_chain_count , client_size.right , client_size.bottom , swap_chain_format , 0 );

		if( result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET )
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			gpu_device_lost();

			// Everything is set up now. Do not continue execution of this method. 
			// gpu_device_lost() will re-enter this method and correctly set up the new device.
			return;
		}
	}
	else
	{
		create_swap_chain();
	}

	create_render_target_view();
	create_depth_stencil();
	create_depth_stencil_view();

	//------------ attach the render target view to the output merger state ------------// Output_merger_set_render_target()
	// Bind the render target view and depth/stencil texture view to the Output Merger pipeline.
	video_device_context->OMSetRenderTargets( 1 ,									// number of render targets to bind 
											  swap_chain_view.GetAddressOf() ,		// array of ID3D11RenderTargetView pointer , RTV[8]
											  depth_stencil_view.Get() );			// ID3D11DepthStencilView pointer

	rasteriser_description.FillMode					= D3D11_FILL_SOLID;	// _WIREFRAME
	rasteriser_description.CullMode					= D3D11_CULL_NONE; //_BACK / _FRONT
	rasteriser_description.FrontCounterClockwise	= false;
	rasteriser_description.DepthBias				= 0;
	rasteriser_description.DepthBiasClamp			= 0.0f;
	rasteriser_description.SlopeScaledDepthBias		= 0.0f;
	rasteriser_description.DepthClipEnable			= true;
	rasteriser_description.ScissorEnable			= false;
	rasteriser_description.MultisampleEnable		= multi_sample_enable; 
	rasteriser_description.AntialiasedLineEnable	= false;// Only applies if doing line drawing and MultisampleEnable is FALSE. 

	video_device->CreateRasterizerState( & rasteriser_description , & rasteriser_state );

	video_device_context->RSSetState( rasteriser_state.Get() );

	// A viewport is a way of translating pixel coordinates to normalised coordinates.
	// pixel coordinates start at 0, 0 in the upper-left corner, and increase one pixel at a time. 
	// Normalised coordinates start at -1, -1 and increase to 1, 1, no matter the size of the back buffer. 

	//------------ create a viewport same size as backbuffer ------------	
	viewport.TopLeftX	= 0.0f;
	viewport.TopLeftY	= 0.0f;
	viewport.Width		= static_cast< float >( client_size.right );
	viewport.Height		= static_cast< float >( client_size.bottom );
	viewport.MinDepth	= 0.0f; // Range between 0..1
	viewport.MaxDepth	= 1.0f; // Range between 0..1

	// Bind an array of viewports to the rasterizer stage of the pipeline.
	video_device_context->RSSetViewports( 1 ,           // number of viewports to bind
										  & viewport );	// D3D11_VIEWPORT structures array to bind to device

	// * Initialise windows-size dependent objects here. *

	//------------ create_vertex_shader( wstring filename *.cso ) ------------//	
	D3DReadFileToBlob( L".\\shaders\\VS_colour.cso" , & d3d_blob);
	
	video_device->CreateVertexShader( d3d_blob->GetBufferPointer() ,
									  d3d_blob->GetBufferSize() ,
									  nullptr ,
									  & vertex_shader );

	unsigned int total_layout_elements = _ARRAYSIZE( input_layout_xyz_rgba_uv );

	video_device->CreateInputLayout( input_layout_xyz_rgba_uv ,		// input-assembler stage input data types array
									 total_layout_elements ,		// Total input-data types in array of input-elements
									 d3d_blob->GetBufferPointer() ,	// compiled shader pointer
									 d3d_blob->GetBufferSize() ,	// size of compiled shader
									 & input_layout );				// output pointer to created input-layout object

	// Bind an input-layout object to the input-assembler stage
	video_device_context->IASetInputLayout( input_layout.Get() );

	//------------ create_pixel_shader( wstring filename *.cso )  ------------//
	D3DReadFileToBlob( L".\\shaders\\PS_colour.cso" , & d3d_blob );
	
	video_device->CreatePixelShader( d3d_blob->GetBufferPointer() ,
									 d3d_blob->GetBufferSize() ,
									 nullptr ,
									 & pixel_shader );

	video_device_context->VSSetShader( vertex_shader.Get() , nullptr , 0 );
	video_device_context->PSSetShader( pixel_shader.Get() , nullptr , 0 );

	//------------ create the sample state ------------
	sampler_description.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// _ANISOTROPIC /_MIN_MAG_MIP_POINT
	sampler_description.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;		// _WRAP / _BORDER
	sampler_description.AddressV		= D3D11_TEXTURE_ADDRESS_BORDER;		// _CLAMP / _WRAP
	sampler_description.AddressW		= D3D11_TEXTURE_ADDRESS_BORDER;		// _CLAMP / _WRAP
	sampler_description.MipLODBias		= 0.0f;
	sampler_description.MaxAnisotropy	= 1u;
	sampler_description.ComparisonFunc	= D3D11_COMPARISON_LESS; // _NEVER // pixels closest to the camera will overwrite the pixels behind them
	sampler_description.BorderColor[ 0 ] = 1.0f;
	sampler_description.BorderColor[ 1 ] = 0.0f;
	sampler_description.BorderColor[ 2 ] = 0.0f;
	sampler_description.BorderColor[ 3 ] = 1.0f;
	sampler_description.MinLOD			= 0.0f;
	sampler_description.MaxLOD			= D3D11_FLOAT32_MAX;// 0.0f;

	video_device->CreateSamplerState( & sampler_description , &sampler_state );

	//------------ set pixel shader sampler/s ------------
	video_device_context->PSSetSamplers( 0 ,								// start sampler/s slot // enum class { SAMPLER_SLOT0 , ... }
										 1 ,								// count of smaplers
										 sampler_state.GetAddressOf() );	// sampler state

	/* If building an array of Direct3D interface pointers ,
	   you should build one as a local variable as there's no direct way to convert an array of ComPtr<T> to an array of T*.

	   ID3D11SamplerState* samplers[] = { sampler1.Get(), sampler2.Get() };
	  context->PSSetSamplers( 0 , _countof( samplers ) , samplers ); */

	//------------ render target 0 blend state ------------
	blend_descripton.AlphaToCoverageEnable = false;
	blend_descripton.IndependentBlendEnable = false; //FALSE, only the RenderTarget[0] members are used; RenderTarget[1..7] are ignored. 
	
	// D3D11_BLEND_SRC_ALPHA		= blend factor (As, As, As, As) , alpha data (A) from a pixel shader. No pre-blend operation.
	// D3D11_BLEND_INV_SRC_ALPHA	= blend factor (1 - Rs, 1 - Gs, 1 - Bs, 1 - As), colour data (RGB) from a pixel shader. The pre-blend operation inverts the data, generating 1 - RGB.

	blend_descripton.RenderTarget[ 0 ].BlendEnable			= true;
	blend_descripton.RenderTarget[ 0 ].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blend_descripton.RenderTarget[ 0 ].DestBlend			= D3D11_BLEND_INV_SRC_ALPHA;
	blend_descripton.RenderTarget[ 0 ].BlendOp				= D3D11_BLEND_OP_ADD;			//Add source 1 and source 2.
	blend_descripton.RenderTarget[ 0 ].SrcBlendAlpha		= D3D11_BLEND_SRC_ALPHA;
	blend_descripton.RenderTarget[ 0 ].DestBlendAlpha		= D3D11_BLEND_ZERO;// D3D11_BLEND_INV_SRC_ALPHA;
	blend_descripton.RenderTarget[ 0 ].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blend_descripton.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	video_device->CreateBlendState( & blend_descripton , & blend_state );

	video_device_context->OMSetBlendState( blend_state.Get() , blend_factor , sample_mask );
}

void Graphics::gpu_device_lost()
{
	depth_stencil_view.Reset();
	depth_stencil_texture.Reset();

	sampler_state.Reset();
	input_layout.Reset();
	pixel_shader.Reset();
	vertex_shader.Reset();
	depth_stencil_view.Reset();
	depth_stencil_texture.Reset();
	swap_chain_view.Reset();
	swap_chain_target.Reset();
	swap_chain.Reset();

	//DX_debug.Reset();

	video_device_context.Reset();
	video_device.Reset();

	create_dx11_device();

	window_size_update();
}

void Graphics::clear_render_target( const DirectX::XMVECTORF32 in_colour ) // DirectX::Colors::
{

	video_device_context->ClearRenderTargetView( swap_chain_view.Get() ,
												 in_colour );  

	video_device_context->ClearDepthStencilView( depth_stencil_view.Get() ,
												 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL ,
												 1.0f ,	// depth
												 0 );	// stencil

	/*float clear_colour[ 4 ];

	clear_colour[ 0 ] = in_colour.x;
	clear_colour[ 1 ] = in_colour.y;
	clear_colour[ 2 ] = in_colour.z;
	clear_colour[ 3 ] = in_colour.w;*/
}

void Graphics::present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.

	HRESULT result = swap_chain->Present( 1u ,	// sync interval
										  0u ); // flags

	// If the device was reset we must completely re-initialise the renderer.
	if( result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET )
	{
		window_size_update();
	}
	//else if( FAILED( result ) ) ErrorExit( L"Present() error; video device removed or reset" );
}

