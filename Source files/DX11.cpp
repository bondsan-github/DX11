#include "DX11.h"

using Microsoft::WRL::ComPtr;

DX11 * g_dx11 = nullptr;

DX11::DX11( const HINSTANCE h_win_instance , const uint window_width , const uint window_height ) // int window_display_options )
	:	instance( h_win_instance ) ,
		window_width( window_width ),
		window_height( window_height )
		//m_i_window_display_options( window_display_options )
{
	g_dx11 = this;
	//							**** receive as constants ****
	window = create_window( instance , window_width , window_height, & client_area ); // , window_display_options );
	create_dx11_device( );
	window_size_update();
		
	//message_loop();	 // not all objects finished initialising?
}

/*DX11::~DX11()
{
	//m_p_DX_debug->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
	//m_p_DX_debug.Reset();
}*/

//void DX11::load_content() {	OutputDebugString( L"\nbase load content\n" ); }

LRESULT CALLBACK global_window_messaging( HWND hwnd , uint message , WPARAM wParam , LPARAM lParam )
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return g_dx11->window_messaging( hwnd , message , wParam , lParam );
}

LRESULT DX11::window_messaging( HWND hwnd , uint message , WPARAM wParam , LPARAM lParam )
{
	//PAINTSTRUCT paint_struct;
	//HDC hDC;
	// auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch( message )
	{
		//case WM_PAINT:
		//	hDC = BeginPaint( hwnd , &paint_struct );
		//	EndPaint( hwnd , &paint_struct );
		//	break;

		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		} 

		case WM_ACTIVATEAPP:
		{
			Keyboard::ProcessMessage( message , wParam , lParam );
			// Mouse::ProcessMessage(message, wParam, lParam);
			break;
		}

		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
		{
			//Mouse::ProcessMessage( message , wParam , lParam );
			//input_mouse();
			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{			
			Keyboard::ProcessMessage( message , wParam , lParam );
			//input_keyboard();
			break;
		} 

		default:
			return DefWindowProc( hwnd , message , wParam , lParam );
	}

	return 0;
}

HWND DX11::create_window( const HINSTANCE in_h_instance , const UINT in_client_width , const UINT in_client_height , RECT * out_client_size ) // int window_display_options )
{
	//debug_out( "\n***create window debug out ***\n" ); 
	//CBrush brush_background( 0x00ff0000 );  //0x00bbggrr

	// static Window::window_messaging

	// SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game.get()) );
	// VOID WINAPI GetStartupInfo( _Out_ LPSTARTUPINFO lpStartupInfo );

	WNDCLASSEX window_class {};

	window_class.cbSize			= sizeof( WNDCLASSEX );
	window_class.style			= CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc	= global_window_messaging;
	window_class.cbClsExtra		= 0;										// extra bytes to allocate following the window-class structure
	window_class.cbWndExtra		= 0;										// extra bytes to allocate following the window instance
	window_class.hInstance		= in_h_instance;
	window_class.hIcon			= LoadIcon( in_h_instance , L"IDI_ICON" );	// a handle to an icon resource
	window_class.hCursor		= LoadCursor( nullptr , IDC_ARROW );
	window_class.hbrBackground	= ( HBRUSH ) ( COLOR_GRAYTEXT + 1 );		// brush_background;
	window_class.lpszMenuName	= nullptr;									// name of the class menu, as the name appears in the resource file
	window_class.lpszClassName	= L"DX11 version1";		// * Must match CreateWindowEx 2nd arg = lpClassName
	window_class.hIconSm		= LoadIcon( in_h_instance , L"IDI_ICON" );	// small icon

	if( ! RegisterClassEx( & window_class ) ) { ErrorExit( L"RegisterClassEX error" ); }

	RECT desktop_size {};
	GetWindowRect( GetDesktopWindow() , & desktop_size );

	int center_x = ( desktop_size.right - in_client_width ) / 2;
	int center_y = ( desktop_size.bottom - in_client_height ) / 2;

	RECT rectangle_window { 0l, 0l, static_cast< long >( in_client_width ), static_cast< long >( in_client_height ) };

	AdjustWindowRect( & rectangle_window , WS_OVERLAPPEDWINDOW , FALSE );

	unsigned long window_styles = WS_OVERLAPPEDWINDOW;//WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;

	HWND h_window = CreateWindowEx( 0 ,//WS_EX_OVERLAPPEDWINDOW ,	// Extended Window Style
									L"DX11 version1" ,			// must be in the low-order word of lpClassName
									L"" ,						// window title displayed in the title bar
									window_styles ,				// window style
									center_x ,					// initial horizontal position 
									center_y ,					// initial vertical position of the window.
									rectangle_window.right - rectangle_window.left ,	// width, in device units, of the window
									rectangle_window.bottom - rectangle_window.top ,	// height, in device units, of the window
									nullptr ,					// handle to the parent or owner window 
									nullptr ,					// handle to a menu - name of menu resource e.g. "MainMenu"
									in_h_instance ,				// handle to the instance of the module to be associated with the window
									nullptr );					// Pointer to a value to be passed to the window through the CREATESTRUCT structure 

	if( !h_window ) { ErrorExit( L"CreateWindowEx failed" ); }

	ShowWindow( h_window , SW_SHOWNORMAL );// window_display_options );	

										   //RECT client_area {};
	GetClientRect( h_window , out_client_size );

	return h_window;
}

WPARAM DX11::message_loop()
{
	MSG win_msg { };
	
	m_timer.reset();

	while ( win_msg.message != WM_QUIT )
	{
		if ( PeekMessage( &win_msg , nullptr , 0 , 0 , PM_REMOVE ) )
		{
			TranslateMessage( &win_msg );
			DispatchMessage( &win_msg );
		}
		else
		{
			m_timer.tick();

			update( m_timer.delta() );
			render();
		}
	}

	return win_msg.wParam;
}

void DX11::create_dx11_device()
{
	HRESULT		 result { E_FAIL };

	/*
	//------------------------------------------------------------------------------
	IDXGIFactory * pFactory = NULL;
	result = CreateDXGIFactory( __uuidof( IDXGIFactory ) , ( void** ) &pFactory );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; CreateDXGIFactory" );

	IDXGIAdapter * pAdapter;
	std::vector <IDXGIAdapter*> vAdapters;

	for( UINT i = 0; pFactory->EnumAdapters( i , &pAdapter ) != DXGI_ERROR_NOT_FOUND; ++i )
	{
		vAdapters.push_back( pAdapter );
	}

	//-------

	IDXGIOutput* pOutput = nullptr;

	result = vAdapters.at(0)->EnumOutputs( 0 , &pOutput );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; EnumOutputs" );

	UINT numModes = 0;
	DXGI_MODE_DESC * displayModes = nullptr;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;// DXGI_FORMAT_UNKNOWN;// ;

													// Get the number of elements
	result = pOutput->GetDisplayModeList( format , 0 , &numModes , nullptr );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; GetDisplayModeList" );

	displayModes = new DXGI_MODE_DESC[ numModes ];

	// Get the list
	result = pOutput->GetDisplayModeList( format , 0 , &numModes , displayModes );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; GetDisplayModeList" );

	vAdapters.clear();

	pOutput->Release();
	pOutput = nullptr;

	pFactory->Release();
	pFactory = nullptr;
	*/
	//------------------------------------------------------------------------------------------

	// RECT dimensions;
	// GetClientRect( m_h_window , &dimensions );

	// long width	= dimensions.right - dimensions.left;
	// long height	= dimensions.bottom - dimensions.top;

	unsigned int creation_flags{ 0 };

#ifdef _DEBUG
	creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

//	HRESULT		 result{ E_FAIL };
	unsigned int driver{ 0 };

	static const D3D_FEATURE_LEVEL feature_levels[ ]
	{
		//D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// to obtain a raw pointer from a ComPtr, use .Get()

	// when creating a new ComPtr; then using a COM creation function/factory, 
	// use .GetAddressOf() as the ComPtr is initially null.

	// when using a ComPtr as a class member;
	// use .ReleaseAndGetAddressOf() to ensure any existing object reference is properly released.

	// operator overload for & is equivelent to .ReleaseAndGetAddressOf()

	//int numfeatures = _countof( feature_levels );
	//int numfeatures2 = sizeof( * feature_levels );

	result = D3D11CreateDevice( nullptr ,						// A pointer to the video adapter to use when creating a device
								  D3D_DRIVER_TYPE_HARDWARE ,	// The D3D_DRIVER_TYPE, which represents the driver type to create
								  nullptr ,						// A handle to a DLL that implements a software rasterizer
								  creation_flags ,				// The runtime layers to enable
								  feature_levels ,				// pointer to D3D_FEATURE_LEVELs array
								  _countof( feature_levels ) ,	// The number of elements in D3D_FEATURE_LEVEL []
								  D3D11_SDK_VERSION ,			// The SDK version; use D3D11_SDK_VERSION
								  & video_device ,				// Returns the address of a pointer to an ID3D11Device
								  & feature_level ,				// Returns a pointer to a D3D_FEATURE_LEVEL
								  & video_device_context );		// Returns the address of a pointer to an ID3D11DeviceContext

	if( FAILED( result ) ) ErrorExit( L"D3D11CreateDevice error" );

	Drawable::set_video_device( video_device );


#ifndef GPU_DEBUG // <- *********************
	if( SUCCEEDED( video_device.As( & DX_debug ) ) )
	{
		ComPtr<ID3D11InfoQueue> d3d_info_queue;
		if( SUCCEEDED( DX_debug.As( & d3d_info_queue ) ) )
		{
#ifdef _DEBUG
			d3d_info_queue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_CORRUPTION , true );
			d3d_info_queue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_ERROR , true );
#endif
			D3D11_MESSAGE_ID hide[ ]
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};

			D3D11_INFO_QUEUE_FILTER filter = { };
			filter.DenyList.NumIDs = _countof( hide );
			filter.DenyList.pIDList = hide;
			d3d_info_queue->AddStorageFilterEntries( &filter );
		}
	}
#endif
}
		
void DX11::create_swap_chain()
{
	//------------ get dxgi factory 1 interface pointer ------------//
	ComPtr< IDXGIDevice1 > dxgi_device1;
	result = video_device.As( & dxgi_device1 ); // Retrieve the underlying DXGI Device from the D3D Device.

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; cannot retrived GXGI device from video device" );


	ComPtr< IDXGIAdapter > dxgi_adapter;
	result = dxgi_device1->GetAdapter( dxgi_adapter.GetAddressOf() ); // Identify the physical adapter (GPU or card) this device is running on.

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; cannot retrived device adaptor" );


	ComPtr< IDXGIFactory1 > dxgi_factory1;
	result = dxgi_adapter->GetParent( IID_PPV_ARGS( dxgi_factory1.GetAddressOf() ) ); // Obtain the factory object that created it.

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; cannot obtain a factory object" );
	
	/*UINT m4xMsaaQuality;
	HR( md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM , 4 , &m4xMsaaQuality ) );
	assert( m4xMsaaQuality > 0 );*/

	//uint format_support {}
	//ID3D11Device::CheckFormatSupport( DXGI_FORMAT , & format_support );
	// if( format_support & D3D11_FORMAT_SUPPORT_DISPLAY )

	//------------ create swap chain ------------//
	
	swap_chain_description.BufferCount						= swap_chain_count;

	swap_chain_description.BufferDesc.Width					= window_width;	// Width and height of the back buffer
	swap_chain_description.BufferDesc.Height				= window_height;
	swap_chain_description.BufferDesc.Format				= swap_chain_format; //DXGI_FORMAT_B8G8R8A8_UNORM; //DXGI_FORMAT_R8G8B8A8_UINT;

	//enumerate display modes -> if( v_sync ) Numerator = monitor refresh rate;
	swap_chain_description.BufferDesc.RefreshRate.Numerator	= 60;	// 60hz refresh rate
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;

	//swap_chain_description.BufferDesc.Scaling				= DXGI_MODE_SCALING_STRETCHED;
	//swap_chain_description.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;

	swap_chain_description.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.OutputWindow						= window;
	swap_chain_description.Windowed							= true;

	swap_chain_description.SampleDesc.Count					= 1;
	swap_chain_description.SampleDesc.Quality				= 0; // No multisampling
	//swap_chain_description.SwapEffect						= DXGI_SWAP_EFFECT_SEQUENTIAL /_DISCARD;

	//On DirectX 11.1 or later systems you can use IDXGIFactory2::CreateSwapChainForHwnd.
	result = dxgi_factory1->CreateSwapChain( video_device.Get() , & swap_chain_description , & swap_chain );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; cannot create swap chain" );

	// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
	result = dxgi_factory1->MakeWindowAssociation( window , DXGI_MWA_NO_ALT_ENTER );
	// DXGI_MWA_NO_ALT_ENTER - Prevent DXGI from responding to an alt-enter sequence. 
	// ensures that DXGI will not interfere with application's handling of window mode changes or Alt+Enter.

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; MakeWindowAssociation()" );
}

void DX11::create_render_target_view()
{
	HRESULT result{ E_FAIL };

	//------------ GetBuffer swap_chain 0 ------------//

	// Obtain the backbuffer for this window which will be the final 3D render target.
	// Textures cannot be bound directly to the pipeline; instead, a view must be created and bound.

	// ComPtr< ID3D11Texture2D > swap_chain_texture;
	result = swap_chain->GetBuffer( 0 , IID_PPV_ARGS( & render_target_texture ) );

	// IID_PPV_ARGS macro
	// Used to retrieve an interface pointer, 
	// supplying the IID value of the requested interface automatically 
	// based on the type of the interface pointer used. 
	// This avoids a common coding error by checking the type of the value passed at compile time.

	if( FAILED( result ) ) ErrorExit( L"Failed to get a swap chain buffer" );


	//------------ Create Render Target View ---------//
	// Create the render target view with the back buffer pointer.

	// resources must use a view to access resource data

	// A render-target is a resource that can be written by the output-merger stage at the end of a render pass. 
	// A render-target-view interface identifies the render-target subresources that can be accessed during rendering.
	// Each render-target should also have a corresponding depth-stencil view.

	result = video_device->CreateRenderTargetView( render_target_texture.Get(),	// ID3D11Resource * that represents a render target
												   nullptr,						// D3D11_RENDER_TARGET_VIEW_DESC *
												   & render_target_view );		// ID3D11RenderTargetView &* 

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; CreateRenderTargetView()" );
}

void DX11::create_depth_stencil()
{
	HRESULT result { E_FAIL };
	//------------ Create depth/stencil texture ---------//

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.

	depth_texture_description.Width				= window_width;
	depth_texture_description.Height			= window_height;
	depth_texture_description.MipLevels			= 1;
	depth_texture_description.ArraySize			= 1;
	depth_texture_description.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_texture_description.SampleDesc.Count	= 1;
	depth_texture_description.SampleDesc.Quality= 0;
	depth_texture_description.Usage				= D3D11_USAGE_DEFAULT;
	depth_texture_description.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depth_texture_description.CPUAccessFlags	= 0;
	depth_texture_description.MiscFlags			= 0;

	result = video_device->CreateTexture2D( & depth_texture_description ,
											nullptr ,
											& depth_stencil_texture );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; creating depth texture" );
}

void DX11::create_depth_stencil_view()
{
	HRESULT result { E_FAIL };
	
	//------------ create the depth stencil view ------------//
	depth_stencil_view_description.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;//depth_texture_description.Format;
	depth_stencil_view_description.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_description.Texture2D.MipSlice	= 0;

	result = video_device->CreateDepthStencilView( depth_stencil_texture.Get() ,
												   & depth_stencil_view_description ,
												   & depth_stencil_view );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; CreateDepthStencilView()" );

	//D3D11_DSV_DIMENSION_TEXTURE2DMS		The resource will be accessed as a 2D texture with multisampling.	
}

void DX11::window_size_update() // Allocate all memory resources that change on a window SizeChanged event.
{
	HRESULT result { E_FAIL };

	// Clear the previous window size specific context.
	ID3D11RenderTargetView * null_target_views[ ] { nullptr };
	video_device_context->OMSetRenderTargets( _countof( null_target_views ) , null_target_views , nullptr ); // ARRAYSIZE

	render_target_view.Reset();	// Release all pointer references to the associated interface
	depth_stencil_view.Reset();	// Release all pointer references to the associated interface

	video_device_context->Flush(); // send buffered commands GPU

	// If the swap chain already exists, resize it, otherwise create one.
	if( swap_chain ) // .get()?
	{
		result = swap_chain->ResizeBuffers( swap_chain_count , window_width , window_height , swap_chain_format , 0 );

		if( result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET )
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			gpu_device_lost();

			// Everything is set up now. Do not continue execution of this method. 
			// gpu_device_lost() will re-enter this method ( function ? ) and correctly set up the new device.
			return;
		}
		else ErrorExit( L"window_size_update() error; ResizeBuffers()" ); 
	}
	else
	{	
		create_swap_chain();		
	}

	// Create depth stencil texture
	// Create the depth stencil view
	// Setup the viewport

	create_render_target_view();	

	create_depth_stencil();	
	//ID3D11Device::CreateDepthStencilState 
	create_depth_stencil_view();	

	//------------ attach the render target view to the output merger state ------------//
	// Bind the render target view and depth/stencil texture view to the Output Merger pipeline.
	video_device_context->OMSetRenderTargets( 1 ,									// number of render targets to bind 
											  render_target_view.GetAddressOf() ,	// array of ID3D11RenderTargetView pointer , RTV[8]
											  depth_stencil_view.Get() );			// ID3D11DepthStencilView pointer

	rasteriser_description.FillMode					= D3D11_FILL_SOLID;	// D3D11_FILL_WIREFRAME
	rasteriser_description.CullMode					= D3D11_CULL_NONE; //D3D11_CULL_BACK /  _FRONT
	rasteriser_description.FrontCounterClockwise	= false;
	rasteriser_description.DepthBias				= 0;
	rasteriser_description.DepthBiasClamp			= 0.0f;
	rasteriser_description.SlopeScaledDepthBias		= 0.0f;
	rasteriser_description.DepthClipEnable			= true; //false; //
	rasteriser_description.ScissorEnable			= false; //true;//
	rasteriser_description.MultisampleEnable		= false; //true;
	rasteriser_description.AntialiasedLineEnable	= false; //true;	// Only applies if doing line drawing and MultisampleEnable is FALSE. 

	result = video_device->CreateRasterizerState( & rasteriser_description , & rasteriser_state );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; CreateRasterizerState()" );

	video_device_context->RSSetState( rasteriser_state.Get() );
	
	// A viewport is a way of translating pixel coordinates to normalized coordinates.
	// pixel coordinates start at 0, 0 in the upper-left corner, and increase one pixel at a time. 
	// Normalized coordinates start at -1, -1 and increase to 1, 1, no matter the size of the back buffer. 
	// The word normalized means that a value is adjusted until it equals 1.
	
	//------------ create a viewport same size as backbuffer ------------
	D3D11_VIEWPORT		  viewport { };
	viewport.TopLeftX	= 0.0f;
	viewport.TopLeftY	= 0.0f;
	viewport.Width		= static_cast< float >( window_width );
	viewport.Height		= static_cast< float >( window_height );
	viewport.MinDepth	= 0.0f; // Range between 0..1
	viewport.MaxDepth	= 1.0f; // Range between 0..1

	// Bind an array of viewports to the rasterizer stage of the pipeline.
	video_device_context->RSSetViewports( 1 ,           // number of viewports to bind
										  & viewport );	// D3D11_VIEWPORT structures array to bind to device
	
	// * Initialise windows-size dependent objects here. * 

	//------------ create VS ------------//	
	ID3DBlob * d3dBlob;
	result = D3DReadFileToBlob( L".\\shaders\\VS_colour.cso" , & d3dBlob );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; D3DReadFileToBlob()" );
	
	result = video_device->CreateVertexShader( d3dBlob->GetBufferPointer(),
											   d3dBlob->GetBufferSize(),
											   nullptr,
											   & vertex_shader );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; CreateVertexShader()" );

	// ****  MOVE TO MESH ***** -> need VS shader compiled cso blob
	//------------ create input layout ------------
	// * * * * * * * * * * * * * * * * * * * * *
	unsigned int total_layout_elements = ARRAYSIZE( input_layout_xyz_rgba_uv );
	// * * * * * * * * * * * * * * * * * * * * *

	result = video_device->CreateInputLayout( input_layout_xyz_rgba_uv ,	// input-assembler stage input data types array
											  total_layout_elements ,		// Total input-data types in array of input-elements
											  d3dBlob->GetBufferPointer() ,	// compiled shader pointer
											  d3dBlob->GetBufferSize() ,	// size of compiled shader
											  & input_layout );				// output pointer to created input-layout object

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; CreateInputLayout()" );

	
	// *** MOVE TO MESH::Render()
	// Set the input layout
	// Bind an input-layout object to the input-assembler stage
	video_device_context->IASetInputLayout( input_layout.Get() );

	//-------------------------------------------------------------------------//
	
	//------------ create PS  ------------//
	result = D3DReadFileToBlob( L".\\shaders\\PS_colour.cso" , & d3dBlob );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; D3DReadFileToBlob()" );

	result = video_device->CreatePixelShader( d3dBlob->GetBufferPointer() ,
											  d3dBlob->GetBufferSize() ,
											  nullptr ,
											  & pixel_shader );

	if( FAILED( result ) ) ErrorExit( L"window_size_update() error; CreatePixelShader()" );

	/*
	File pixel_shader( L"shaders/PS_diffuse_map.hlsl" );

	m_p_video_device->CreatePixelShader( pixel_shader.content()->data() ,
										 pixel_shader.content()->size() ,
										 nullptr ,
										 & m_p_pixel_shader );
	*/
		
	video_device_context->VSSetShader( vertex_shader.Get() , nullptr , 0 );
	video_device_context->PSSetShader( pixel_shader.Get() , nullptr , 0 );

	// move to Texture
	//------------ create the sample state ------------
	sampler_description.Filter				= D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// _ANISOTROPIC /_MIN_MAG_MIP_POINT
	sampler_description.AddressU			= D3D11_TEXTURE_ADDRESS_CLAMP;		// _WRAP / _BORDER
	sampler_description.AddressV			= D3D11_TEXTURE_ADDRESS_BORDER;		// _CLAMP / _WRAP
	sampler_description.AddressW			= D3D11_TEXTURE_ADDRESS_BORDER;		// _CLAMP / _WRAP
	sampler_description.MipLODBias			= 0.0f;
	sampler_description.MaxAnisotropy		= 1u;
	sampler_description.ComparisonFunc		= D3D11_COMPARISON_LESS;	// _NEVER // pixels closest to the camera will overwrite the pixels behind them
	sampler_description.BorderColor[ 0 ]	= 1.0f;
	sampler_description.BorderColor[ 1 ]	= 0.0f;
	sampler_description.BorderColor[ 2 ]	= 0.0f;
	sampler_description.BorderColor[ 3 ]	= 1.0f;
	sampler_description.MinLOD				= 0.0f;
	sampler_description.MaxLOD				= D3D11_FLOAT32_MAX;// 0.0f;
	
	result = video_device->CreateSamplerState( & sampler_description , & sampler_state );
	
	if( FAILED( result ) ) ErrorExit( L"window_size_update() CreateSamplerState() error" );
		
	//------------ set pixel shader sampler/s ------------
	video_device_context->PSSetSamplers( 0,									// start sampler/s slot // enum class { SAMPLER_SLOT0 , ... }
										 1,									// count of smaplers
										 sampler_state.GetAddressOf() );	// sampler state

	/*
		If building an array of Direct3D interface pointers ,
		you should build one as a local variable as there's no direct way to convert an array of ComPtr<T> to an array of T*.

		ID3D11SamplerState* samplers[] = { sampler1.Get(), sampler2.Get() };
		context->PSSetSamplers( 0 , _countof( samplers ) , samplers );
	*/

	//------------ render target 0 blend state ------------
	blend_descripton.AlphaToCoverageEnable	= false;
	blend_descripton.IndependentBlendEnable	= false; //FALSE, only the RenderTarget[0] members are used; RenderTarget[1..7] are ignored. 

	// D3D11_BLEND_SRC_ALPHA		= blend factor (As, As, As, As) , alpha data (A) from a pixel shader. No pre-blend operation.
	// D3D11_BLEND_INV_SRC_ALPHA	= blend factor (1 - Rs, 1 - Gs, 1 - Bs, 1 - As), colour data (RGB) from a pixel shader. The pre-blend operation inverts the data, generating 1 - RGB.

	blend_descripton.RenderTarget[ 0 ].BlendEnable			= true;
	blend_descripton.RenderTarget[ 0 ].SrcBlend				= D3D11_BLEND_SRC_ALPHA; 
	blend_descripton.RenderTarget[ 0 ].DestBlend			= D3D11_BLEND_INV_SRC_ALPHA; 
	blend_descripton.RenderTarget[ 0 ].BlendOp				= D3D11_BLEND_OP_ADD;			//Add source 1 and source 2.
	blend_descripton.RenderTarget[ 0 ].SrcBlendAlpha		= D3D11_BLEND_SRC_ALPHA;
	blend_descripton.RenderTarget[ 0 ].DestBlendAlpha		= D3D11_BLEND_ZERO;// D3D11_BLEND_INV_SRC_ALPHA;
	blend_descripton.RenderTarget[ 0 ].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blend_descripton.RenderTarget[ 0 ].RenderTargetWriteMask= D3D11_COLOR_WRITE_ENABLE_ALL;

	result = video_device->CreateBlendState( & blend_descripton , & blend_state );
	
	if( FAILED( result ) ) ErrorExit( L"window_size_update() CreateBlendState() error" );

	float	blend_factor1[ 4 ]{ 1.0f , 1.0f , 1.0f , 1.0f };
	unsigned int sample_mask1 = 0xffffffff;

	video_device_context->OMSetBlendState( blend_state.Get() , blend_factor1 , sample_mask1 );
}

//void DX11::update( ){	//float delta_time}
//void DX11::render( ){clear();present();}

void DX11::clear()
{
	// Clear the views.
	float colour[ 4 ] { 1.0f, 1.0f, 1.0f, 0.0f };

	video_device_context->ClearRenderTargetView( render_target_view.Get() , 
												 colour );  // 4-component colour array color to fill // dx9 D3DCOLOR_RGBA() );

	video_device_context->ClearDepthStencilView( depth_stencil_view.Get() ,
												 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL ,
												 1.0f ,	// depth
												 0 );	// stencil
}

void DX11::clear( const XMFLOAT4 in_colour )
{
	// Clear the views.
	float clear_colour[ 4 ];

	clear_colour[ 0 ] = in_colour.x;
	clear_colour[ 1 ] = in_colour.y;
	clear_colour[ 2 ] = in_colour.z;
	clear_colour[ 3 ] = in_colour.w;

	video_device_context->ClearRenderTargetView( render_target_view.Get() ,
												 clear_colour );  // 4-component colour array color to fill // dx9 D3DCOLOR_RGBA() );

	video_device_context->ClearDepthStencilView( depth_stencil_view.Get() ,
												 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL ,
												 1.0f ,	// depth
												 0 );	// stencil
}

void DX11::present()
{ 
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.

	result = swap_chain->Present( 1u,	// sync interval
								  0u ); // flags

	// If the device was reset we must completely reinitialise the renderer.
	if( result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET )
	{
		window_size_update();
	}
	else
	{
		if( FAILED( result ) ) 
			ErrorExit( L"Present() error; video device removed or reset" );
	}
}


//void DX11::load_vertex_shader(const LPWSTR filename)
//{
//	File shader( filename );
//
//	/*m_p_video_device->CreateVertexShader( shader.content(),
//										  shader.content()->size() ,
//										  nullptr ,
//										  m_p_vertex_shader.ReleaseAndGetAddressOf() );*/
//	/*
//	[in]					const void         *pShaderBytecode ,
//	[ in ]					SIZE_T             BytecodeLength ,
//	[ in , optional ]		ID3D11ClassLinkage *pClassLinkage ,
//	[ out , optional ]      ID3D11VertexShader **ppVertexShader
//	*/
//}

//void DX11::load_pixel_shader( const LPWSTR filename )
//{ 
//	File shader( filename );
//
//	/*m_p_video_device->CreatePixelShader( shader.content() ,
//										 shader.content()->size() ,
//										 nullptr ,
//										 m_p_pixel_shader.ReleaseAndGetAddressOf() );*/
//}

//HRESULT DX11::compile_shader( LPCTSTR file_path, LPCSTR entry, LPCSTR shader_model, ID3DBlob** buffer )
//{
//	/*
//	OutputDebugString( L"\n****** compile_shader ******\n" );
//	OutputDebugString( file_path );
//	OutputDebugString( L"\n" );
//
//	TCHAR szPath[ MAX_PATH ];
//
//	GetModuleFileNameW( 0,
//					   szPath ,
//					   MAX_PATH );
//	OutputDebugString( L"\nexecutable path : " );
//	OutputDebugString( szPath );
//	OutputDebugString( L"\n\n" );
//	*/
//
//    unsigned long shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
//
//    #if defined( _DEBUG )
//        shader_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
//    #endif
//        
//    ID3DBlob * error_buffer = nullptr;
//    long result;
//
//    result = D3DCompileFromFile( file_path,     // The name of the file that contains the shader code
//                                   nullptr,       // Optional. Pointer to an array of macro definitions 
//                                   nullptr,       // Optional. Pointer to an interface for handling include files
//                                   entry,         // Name of the shader-entry point function where shader execution begins.
//                                   shader_model,  // string that specifies the shader model 
//                                   shader_flags,  // Shader compile flags
//                                   0,             // Effect compile flags
//                                   buffer,        // pointer to memory which contains the compiled shader
//                                   &error_buffer ); // pointer to memory which contains a listing of errors and warnings that occurred during compilation
//
//    if( FAILED( result ) )
//    {
//        if( error_buffer ) // != 0
//        {
//			ErrorExit( static_cast< LPCTSTR >( error_buffer->GetBufferPointer( ) ) );
//        }
//        return result;
//    }
//    
//    if( error_buffer != 0 ) error_buffer->Release( );
//
//    return result;
//}

void DX11::gpu_device_lost()
{
	// ComPtr::Reset = Release all pointer references to the associated interface

	//m_map_cameras.clear();

	sampler_state.Reset();

	input_layout.Reset();

	pixel_shader.Reset();
	vertex_shader.Reset();
	
	//m_p_constant_buffer_projection.Reset();
	//m_p_constant_buffer_view.Reset();	
	//m_p_constant_buffer_world.Reset();
	
	depth_stencil_view.Reset();
	depth_stencil_texture.Reset();

	render_target_view.Reset();
	render_target_texture.Reset();
	
	swap_chain.Reset();

	DX_debug.Reset();

	video_device_context.Reset();
	video_device.Reset();

	create_dx11_device();

	window_size_update();	
}

//------------ Compile VS ------------

//ID3DBlob * vertex_shader_buffer = nullptr;
//result = compile_shader( L"shaders/texture_map.fx" , "VS_Main" , "vs_4_0" , & vertex_shader_buffer );
//if( FAILED( result ) ) { ErrorExit( L"compile_shader-vertex_shader error\n" ); }

///*
//ID3DBlob * vertex_shader_cso = nullptr;
//D3DReadFileToBlob( file_path , & vertex_shader_cso );
//*/

////------------ Create VS ------------
//result = m_p_video_device->CreateVertexShader( vertex_shader_buffer->GetBufferPointer() ,	// A pointer to the compiled shader
//												 vertex_shader_buffer->GetBufferSize() ,		// Size of the compiled vertex shader
//												 nullptr ,					 // A pointer to a class linkage interface, the value can be NULL
//												 m_p_vertex_shader_solid_colour.GetAddressOf() );	// Address of a pointer to a ID3D11VertexShader interface
//	

//if ( FAILED( result ) ) { ErrorExit( L"CreateVertexShader error" ); }
////if ( vertex_shader_buffer ) vertex_shader_buffer->Release( ); return false;

////------------ Compile PS ------------
//ID3DBlob * pixel_shader_buffer = nullptr;

//// LPCSTR profile = ( device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) ? "cs_5_0" : "cs_4_0";
//// Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11-class hardware.

//result = compile_shader( L"shaders/texture_map.fx" , "PS_Main" , "ps_4_0" , & pixel_shader_buffer );
//if ( FAILED( result ) ) { ErrorExit( L"compile_shader-pixel_shader error\n" ); }

////------------ Create PS  ------------
//result = m_p_video_device->CreatePixelShader( pixel_shader_buffer->GetBufferPointer( ) ,	// compiled shader pointer
//												pixel_shader_buffer->GetBufferSize( ) ,		// size of compiled pixel shader
//												nullptr ,									// class linkage interface pointer, value can be NULL
//												m_p_pixel_shader_solid_colour.GetAddressOf() );	// ID3D11PixelShader interface pointer address

////pixel_shader_buffer->Release( );

//if ( FAILED( result ) ) {	ErrorExit( L"CreatePixelShader error\n" ); }

////------------ Create input layout ------------
//// * * * * * * * * * * * * * * * * * * * * *
//unsigned int total_layout_elements = ARRAYSIZE( input_layout_pos_texture );
//// * * * * * * * * * * * * * * * * * * * * *

//result = m_p_video_device->CreateInputLayout( input_layout_pos_texture ,	// input-assembler stage input data types array
//												total_layout_elements ,		// Total input-data types in array of input-elements
//												vertex_shader_buffer->GetBufferPointer() ,	// compiled shader pointer
//												vertex_shader_buffer->GetBufferSize() ,		// size of compiled shader
//												m_p_input_layout.GetAddressOf() );		// output pointer to created input-layout object

//if( FAILED( result ) )
//{
//	ErrorExit( L"CreateInputLayout error" );
//}

//// Set the input layout
//// Bind an input-layout object to the input-assembler stage
//m_p_video_device_context->IASetInputLayout( m_p_input_layout.Get() );

//// load_content( );  // cascaded function call - returns result of load_content();

////m_p_video_device_context->VSSetShader( m_p_vertex_shader_solid_colour , NULL , 0 );
////m_p_video_device_context->PSSetShader( m_p_pixel_shader_solid_colour , NULL , 0 );

/*
//-------- clear depth stencil --------
m_p_video_device_context->ClearDepthStencilView( m_p_depth_stencil_view ,
D3D11_CLEAR_DEPTH ,
1.0f ,
0 );

XMMATRIX matrix_world = icosphere->get_matrix_world( );
//matrix_world = XMMatrixTranspose( matrix_world );

XMMATRIX matrix_view = m_itr_active_camera->second.get_matrix_view();
//matrix_view = XMMatrixTranspose( matrix_view );

//-------- update constant buffers from matrices --------
m_p_video_device_context->UpdateSubresource( m_p_constant_buffer_world ,	// destination resource pointer
0 ,							// destination subresource, zero-based
nullptr ,						// box pointer - portion of destination subresource to copy data into
& matrix_world ,				// source data pointer in memory
0 ,							// size of one row of source data
0 );							// size of one depth slice of source data

m_p_video_device_context->UpdateSubresource( m_p_constant_buffer_view , 0 , nullptr , & matrix_view , 0 , 0 );

m_p_video_device_context->UpdateSubresource( m_p_constant_buffer_projection , 0 , nullptr , & m_matrix_projection , 0 , 0 );



//-------- attach constant buffers to FX buffers (vertex shader stage) --------
m_p_video_device_context->VSSetConstantBuffers( 0 ,								// begining of devices' constant buffers index array (zero-based)
1 ,								// total buffers
& m_p_constant_buffer_world );  // constant buffers array to video device

m_p_video_device_context->VSSetConstantBuffers( 1 , 1 , &m_p_constant_buffer_view );

m_p_video_device_context->VSSetConstantBuffers( 2 , 1 , &m_p_constant_buffer_projection );

//-------- set FX VS and PS shader stages --------
m_p_video_device_context->VSSetShader( m_p_vertex_shader_solid_colour , nullptr , 0 );

m_p_video_device_context->PSSetShader( m_p_pixel_shader_solid_colour , nullptr , 0 );

//-------- set FX pixel shader --------
// p_video_device_context->PSSetShaderResources( 0, 1, &diffuse_map );
// p_video_device_context->PSSetSamplers( 0, 1, &diffuse_map_sampler );

//cube->draw();
icosphere->draw();
*/