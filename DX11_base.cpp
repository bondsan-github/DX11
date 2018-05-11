#include "DX11_base.h"

using Microsoft::WRL::ComPtr;

DX11 * g_dx11 = nullptr;

DX11::DX11( const HINSTANCE h_win_instance , const UINT window_width , const UINT window_height ) // int window_display_options )
	:	m_instance( h_win_instance ) ,
		m_window_width( window_width ),
		m_window_height( window_height )
		//m_i_window_display_options( window_display_options )
{
	g_dx11 = this;
	//							**** receive as constants ****
	m_window = create_window( m_instance , window_width , window_height, & m_client_area ); // , window_display_options );
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

LRESULT CALLBACK global_window_messaging( HWND hwnd , UINT message , WPARAM wParam , LPARAM lParam )
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return g_dx11->window_messaging( hwnd , message , wParam , lParam );
}

LRESULT DX11::window_messaging( HWND hwnd , UINT message , WPARAM wParam , LPARAM lParam )
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
									rectangle_window.right - rectangle_window.left ,				// width, in device units, of the window
									rectangle_window.bottom - rectangle_window.top ,				// height, in device units, of the window
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
	HRESULT		 h_result { E_FAIL };

	/*
	//------------------------------------------------------------------------------
	IDXGIFactory * pFactory = NULL;
	h_result = CreateDXGIFactory( __uuidof( IDXGIFactory ) , ( void** ) &pFactory );

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; CreateDXGIFactory" );

	IDXGIAdapter * pAdapter;
	std::vector <IDXGIAdapter*> vAdapters;

	for( UINT i = 0; pFactory->EnumAdapters( i , &pAdapter ) != DXGI_ERROR_NOT_FOUND; ++i )
	{
		vAdapters.push_back( pAdapter );
	}

	//-------

	IDXGIOutput* pOutput = nullptr;

	h_result = vAdapters.at(0)->EnumOutputs( 0 , &pOutput );

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; EnumOutputs" );

	UINT numModes = 0;
	DXGI_MODE_DESC * displayModes = nullptr;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;// DXGI_FORMAT_UNKNOWN;// ;

													// Get the number of elements
	h_result = pOutput->GetDisplayModeList( format , 0 , &numModes , nullptr );

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; GetDisplayModeList" );

	displayModes = new DXGI_MODE_DESC[ numModes ];

	// Get the list
	h_result = pOutput->GetDisplayModeList( format , 0 , &numModes , displayModes );

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; GetDisplayModeList" );

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

//	HRESULT		 h_result{ E_FAIL };
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

	h_result = D3D11CreateDevice( nullptr ,								// A pointer to the video adapter to use when creating a device
								  D3D_DRIVER_TYPE_HARDWARE ,			// The D3D_DRIVER_TYPE, which represents the driver type to create
								  nullptr ,								// A handle to a DLL that implements a software rasterizer
								  creation_flags ,						// The runtime layers to enable
								  feature_levels ,						// A pointer to an array of D3D_FEATURE_LEVELs
								  _countof( feature_levels ) ,			// The number of elements in D3D_FEATURE_LEVEL []
								  D3D11_SDK_VERSION ,					// The SDK version; use D3D11_SDK_VERSION
								  m_video_device.ReleaseAndGetAddressOf() , // Returns the address of a pointer to an ID3D11Device
								  & m_feature_level ,					// Returns a pointer to a D3D_FEATURE_LEVEL
								  m_video_device_context.ReleaseAndGetAddressOf() ); // Returns the address of a pointer to an ID3D11DeviceContext

	if( FAILED( h_result ) ) ErrorExit( L"D3D11CreateDevice error" );

	Drawable::set_video_device( m_video_device );


#ifndef GPU_DEBUG // <- *********************
	if( SUCCEEDED( m_video_device.As( &m_DX_debug ) ) )
	{
		ComPtr<ID3D11InfoQueue> d3d_info_queue;
		if( SUCCEEDED( m_DX_debug.As( &d3d_info_queue ) ) )
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
	HRESULT h_result{ E_FAIL };

	//------------ get dxgi factory 1 interface pointer ------------//
	ComPtr< IDXGIDevice1 > dxgi_device1;
	h_result = m_video_device.As( & dxgi_device1 ); // Retrieve the underlying DXGI Device from the D3D Device.

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; cannot retrived GXGI device from video device" );


	ComPtr< IDXGIAdapter > dxgi_adapter;
	h_result = dxgi_device1->GetAdapter( dxgi_adapter.GetAddressOf() ); // Identify the physical adapter (GPU or card) this device is running on.

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; cannot retrived device adaptor" );


	ComPtr< IDXGIFactory1 > dxgi_factory1;
	h_result = dxgi_adapter->GetParent( IID_PPV_ARGS( dxgi_factory1.GetAddressOf() ) ); // Obtain the factory object that created it.

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; cannot obtain a factory object" );
	
	/*UINT m4xMsaaQuality;
	HR( md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM , 4 , &m4xMsaaQuality ) );
	assert( m4xMsaaQuality > 0 );*/

	//uint format_support {}
	//ID3D11Device::CheckFormatSupport( DXGI_FORMAT , & format_support );
	// if( format_support & D3D11_FORMAT_SUPPORT_DISPLAY )

	//------------ create swap chain ------------//
	
	m_swap_chain_description.BufferCount						= m_swap_chain_count;

	m_swap_chain_description.BufferDesc.Width					= m_window_width;	// Width and height of the back buffer
	m_swap_chain_description.BufferDesc.Height					= m_window_height;
	m_swap_chain_description.BufferDesc.Format					= m_swap_chain_format; //DXGI_FORMAT_B8G8R8A8_UNORM; //DXGI_FORMAT_R8G8B8A8_UINT;

	//enumerate display modes -> if( v_sync ) Numerator = monitor refresh rate;
	m_swap_chain_description.BufferDesc.RefreshRate.Numerator	= 60;					// 60hz refresh rate
	m_swap_chain_description.BufferDesc.RefreshRate.Denominator	= 1;

	//m_swap_chain_description.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	//m_swap_chain_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;

	m_swap_chain_description.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_swap_chain_description.OutputWindow						= m_window;
	m_swap_chain_description.Windowed							= true;

	m_swap_chain_description.SampleDesc.Count					= 1;
	m_swap_chain_description.SampleDesc.Quality					= 0; // No multisampling
	//m_swap_chain_description.SwapEffect						= DXGI_SWAP_EFFECT_SEQUENTIAL  // DXGI_SWAP_EFFECT_DISCARD; // 

	//On DirectX 11.1 or later systems you can use IDXGIFactory2::CreateSwapChainForHwnd.
	h_result = dxgi_factory1->CreateSwapChain( m_video_device.Get() , & m_swap_chain_description , m_swap_chain.ReleaseAndGetAddressOf() );

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; cannot create swap chain" );

	// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
	h_result = dxgi_factory1->MakeWindowAssociation( m_window , DXGI_MWA_NO_ALT_ENTER );
	// DXGI_MWA_NO_ALT_ENTER - Prevent DXGI from responding to an alt-enter sequence. 
	// ensures that DXGI will not interfere with application's handling of window mode changes or Alt+Enter.

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; MakeWindowAssociation()" );
}

void DX11::create_render_target_view()
{
	HRESULT h_result{ E_FAIL };

	//------------ GetBuffer swap_chain 0 ------------//

	// Obtain the backbuffer for this window which will be the final 3D render target.
	// Textures cannot be bound directly to the pipeline; instead, a view must be created and bound.

	// ComPtr< ID3D11Texture2D > swap_chain_texture;
	h_result = m_swap_chain->GetBuffer( 0 , IID_PPV_ARGS( m_render_target_texture.ReleaseAndGetAddressOf() ) );

	// IID_PPV_ARGS macro
	// Used to retrieve an interface pointer, 
	// supplying the IID value of the requested interface automatically 
	// based on the type of the interface pointer used. 
	// This avoids a common coding error by checking the type of the value passed at compile time.

	if( FAILED( h_result ) ) ErrorExit( L"Failed to get a swap chain buffer" );


	//------------ Create Render Target View ---------//
	// Create the render target view with the back buffer pointer.

	// resources must use a view to access resource data

	// A render-target is a resource that can be written by the output-merger stage at the end of a render pass. 
	// A render-target-view interface identifies the render-target subresources that can be accessed during rendering.
	// Each render-target should also have a corresponding depth-stencil view.

	h_result = m_video_device->CreateRenderTargetView( m_render_target_texture.Get(),	// ID3D11Resource * that represents a render target
														 nullptr,							// D3D11_RENDER_TARGET_VIEW_DESC *
														 m_render_target_view.ReleaseAndGetAddressOf() );	// ID3D11RenderTargetView &* 

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; CreateRenderTargetView()" );
}

void DX11::create_depth_stencil()
{
	HRESULT h_result { E_FAIL };
	//------------ Create depth/stencil texture ---------//

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.

	m_depth_texture_description.Width				= m_window_width;
	m_depth_texture_description.Height				= m_window_height;
	m_depth_texture_description.MipLevels			= 1;
	m_depth_texture_description.ArraySize			= 1;
	m_depth_texture_description.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_depth_texture_description.SampleDesc.Count	= 1;
	m_depth_texture_description.SampleDesc.Quality	= 0;
	m_depth_texture_description.Usage				= D3D11_USAGE_DEFAULT;
	m_depth_texture_description.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	m_depth_texture_description.CPUAccessFlags		= 0;
	m_depth_texture_description.MiscFlags			= 0;

	h_result = m_video_device->CreateTexture2D( & m_depth_texture_description ,
												  nullptr ,
												  m_depth_stencil_texture.ReleaseAndGetAddressOf() );

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; creating depth texture" );
}

void DX11::create_depth_stencil_view()
{
	HRESULT h_result { E_FAIL };
	
	//------------ create the depth stencil view ------------//
	m_depth_stencil_view_description.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;//depth_texture_description.Format;
	m_depth_stencil_view_description.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	m_depth_stencil_view_description.Texture2D.MipSlice	= 0;

	h_result = m_video_device->CreateDepthStencilView( m_depth_stencil_texture.Get() ,
													   & m_depth_stencil_view_description ,
													   m_depth_stencil_view.ReleaseAndGetAddressOf() );

	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() error; CreateDepthStencilView()" );

	//D3D11_DSV_DIMENSION_TEXTURE2DMS		The resource will be accessed as a 2D texture with multisampling.	
}

void DX11::window_size_update() // Allocate all memory resources that change on a window SizeChanged event.
{
	HRESULT h_result { E_FAIL };

	// Clear the previous window size specific context.
	ID3D11RenderTargetView * null_target_views[ ] { nullptr };
	m_video_device_context->OMSetRenderTargets( _countof( null_target_views ) , null_target_views , nullptr );

	m_render_target_view.Reset();	// Release all pointer references to the associated interface
	m_depth_stencil_view.Reset();	// Release all pointer references to the associated interface

	m_video_device_context->Flush(); // send buffered commands GPU

	// If the swap chain already exists, resize it, otherwise create one.
	if( m_swap_chain ) // .get()?
	{
		h_result = m_swap_chain->ResizeBuffers( m_swap_chain_count , m_window_width , m_window_height , m_swap_chain_format , 0 );

		if( h_result == DXGI_ERROR_DEVICE_REMOVED || h_result == DXGI_ERROR_DEVICE_RESET )
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			gpu_device_lost();

			// Everything is set up now. Do not continue execution of this method. 
			// gpu_device_lost() will re-enter this method ( function ? ) and correctly set up the new device.
			return;
		}
		else ErrorExit( L"window_size_update() error when resizing back buffer" ); 
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
	m_video_device_context->OMSetRenderTargets( 1 ,										// number of render targets to bind 
												m_render_target_view.GetAddressOf() ,	// array of ID3D11RenderTargetView pointer , RTV[8]
												m_depth_stencil_view.Get() );			// ID3D11DepthStencilView pointer

	m_rasteriser_description.FillMode				= D3D11_FILL_SOLID;	// D3D11_FILL_WIREFRAME
	m_rasteriser_description.CullMode				= D3D11_CULL_NONE; //D3D11_CULL_BACK; //  D3D11_CULL_FRONT
	m_rasteriser_description.FrontCounterClockwise	= false;
	m_rasteriser_description.DepthBias				= 0;
	m_rasteriser_description.DepthBiasClamp			= 0.0f;
	m_rasteriser_description.SlopeScaledDepthBias	= 0.0f;
	m_rasteriser_description.DepthClipEnable		= true; //false; //
	m_rasteriser_description.ScissorEnable			= false; //true;//
	m_rasteriser_description.MultisampleEnable		= false; //true;
	m_rasteriser_description.AntialiasedLineEnable	= false; //true;	// Only applies if doing line drawing and MultisampleEnable is FALSE. 

	m_video_device->CreateRasterizerState( & m_rasteriser_description , m_rasteriser_state.ReleaseAndGetAddressOf() );

	m_video_device_context->RSSetState( m_rasteriser_state.Get() );

	
	// A viewport is a way of translating pixel coordinates to normalized coordinates.
	// pixel coordinates start at 0, 0 in the upper-left corner, and increase one pixel at a time. 
	// Normalized coordinates start at -1, -1 and increase to 1, 1, no matter the size of the back buffer. 
	// The word normalized means that a value is adjusted until it equals 1.
	
	//------------ create a viewport same size as backbuffer ------------
	D3D11_VIEWPORT		  viewport { };
	viewport.TopLeftX	= 0.0f;
	viewport.TopLeftY	= 0.0f;
	viewport.Width		= static_cast< float >( m_window_width );
	viewport.Height		= static_cast< float >( m_window_height );
	viewport.MinDepth	= 0.0f; // Range between 0..1
	viewport.MaxDepth	= 1.0f; // Range between 0..1

	// Bind an array of viewports to the rasterizer stage of the pipeline.
	m_video_device_context->RSSetViewports( 1 ,           // number of viewports to bind
											& viewport );	// D3D11_VIEWPORT structures array to bind to device
	
	// * Initialise windows-size dependent objects here. * 

	//------------ create VS ------------//
	
	ID3DBlob * d3dBlob;
	h_result = D3DReadFileToBlob( L"shaders/VS_colour.cso" , & d3dBlob );
	
	m_video_device->CreateVertexShader( d3dBlob->GetBufferPointer(),
										d3dBlob->GetBufferSize(),
										nullptr,
										m_vertex_shader.ReleaseAndGetAddressOf() );

	/*
	File vertex_shader( L"shaders/VS_diffuse_map.hlsl" );

	m_p_video_device->CreateVertexShader( vertex_shader.content()->data() ,
	vertex_shader.content()->size() ,
	nullptr ,
	m_p_vertex_shader.ReleaseAndGetAddressOf() );

	debug_out( "\n vector size : %d \n" , vertex_shader.content()->size() );
	debug_out( "\n data size : %d \n" , sizeof( vertex_shader.content()->data() ) );
	*/

	
	// ****  MOVE TO MESH ***** -> need VS shader compiled cso blob
	//------------ create input layout ------------
	// * * * * * * * * * * * * * * * * * * * * *
	unsigned int total_layout_elements = ARRAYSIZE( input_layout_xyz_rgba_uv );
	// * * * * * * * * * * * * * * * * * * * * *

	h_result = m_video_device->CreateInputLayout( input_layout_xyz_rgba_uv ,		// input-assembler stage input data types array
												  total_layout_elements ,			// Total input-data types in array of input-elements
												  d3dBlob->GetBufferPointer() ,	// compiled shader pointer
												  d3dBlob->GetBufferSize() ,		// size of compiled shader
												  m_input_layout.ReleaseAndGetAddressOf() ); // output pointer to created input-layout object

	if( FAILED( h_result ) ) ErrorExit( L"CreateInputLayout error" );

	
	// *** MOVE TO MESH::Render()
	// Set the input layout
	// Bind an input-layout object to the input-assembler stage
	m_video_device_context->IASetInputLayout( m_input_layout.Get() );

	//-------------------------------------------------------------------------//
	
	//------------ create PS  ------------//
	h_result = D3DReadFileToBlob( L"shaders/PS_colour.cso" , & d3dBlob );

	h_result = m_video_device->CreatePixelShader( d3dBlob->GetBufferPointer() ,
												  d3dBlob->GetBufferSize() ,
												  nullptr ,
												  m_pixel_shader.ReleaseAndGetAddressOf() );

	if( FAILED( h_result ) ) ErrorExit( L"CreatePixelShader error" );

	/*
	File pixel_shader( L"shaders/PS_diffuse_map.hlsl" );

	m_p_video_device->CreatePixelShader( pixel_shader.content()->data() ,
										 pixel_shader.content()->size() ,
										 nullptr ,
										 m_p_pixel_shader.ReleaseAndGetAddressOf() );
	*/
		
	m_video_device_context->VSSetShader( m_vertex_shader.Get() , nullptr , 0 );
	m_video_device_context->PSSetShader( m_pixel_shader.Get() , nullptr , 0 );

	// move to Texture
	//------------ create the sample state ------------
	m_struct_sampler_description.Filter				= D3D11_FILTER_MIN_MAG_MIP_LINEAR;//_ANISOTROPIC; //_MIN_MAG_MIP_POINT;// ;//
	m_struct_sampler_description.AddressU			= D3D11_TEXTURE_ADDRESS_CLAMP;// _WRAP;// ;// _BORDER;
	m_struct_sampler_description.AddressV			= D3D11_TEXTURE_ADDRESS_BORDER;//_CLAMP;//;// _WRAP;
	m_struct_sampler_description.AddressW			= D3D11_TEXTURE_ADDRESS_BORDER;// _CLAMP;//;// _WRAP;//
	m_struct_sampler_description.MipLODBias			= 0.0f;
	m_struct_sampler_description.MaxAnisotropy		= 1u;
	m_struct_sampler_description.ComparisonFunc		= D3D11_COMPARISON_LESS;//pixels closest to the camera will overwrite the pixels behind them // _NEVER;
	m_struct_sampler_description.BorderColor[ 0 ]	= 1.0f;
	m_struct_sampler_description.BorderColor[ 1 ]	= 0.0f;
	m_struct_sampler_description.BorderColor[ 2 ]	= 0.0f;
	m_struct_sampler_description.BorderColor[ 3 ]	= 1.0f;
	m_struct_sampler_description.MinLOD				= 0.0f;
	m_struct_sampler_description.MaxLOD				= D3D11_FLOAT32_MAX;// 0.0f;
	
	h_result = m_video_device->CreateSamplerState( & m_struct_sampler_description , m_sampler_state.ReleaseAndGetAddressOf() );
	if( FAILED( h_result ) ) ErrorExit( L"window_size_update() CreateSamplerState error" );
		
	//------------ set pixel shader sampler/s ------------
	m_video_device_context->PSSetSamplers( 0,			// start sampler/s slot // enum class { SAMPLER_SLOT0 , ... }
										   1,			// count of smaplers
										   m_sampler_state.GetAddressOf() );	// sampler state

	/*
		If building an array of Direct3D interface pointers ,
		you should build one as a local variable as there's no direct way to convert an array of ComPtr<T> to an array of T*.

		ID3D11SamplerState* samplers[] = { sampler1.Get(), sampler2.Get() };
		context->PSSetSamplers( 0 , _countof( samplers ) , samplers );
	*/

	//------------ render target 0 blend state ------------
	m_blend_descripton.AlphaToCoverageEnable	= false;
	m_blend_descripton.IndependentBlendEnable	= false; //FALSE, only the RenderTarget[0] members are used; RenderTarget[1..7] are ignored. 

	m_blend_descripton.RenderTarget[ 0 ].BlendEnable			= true;
	m_blend_descripton.RenderTarget[ 0 ].SrcBlend				= D3D11_BLEND_SRC_ALPHA; // The blend factor is (As, As, As, As) , that is alpha data (A) from a pixel shader
	m_blend_descripton.RenderTarget[ 0 ].DestBlend				= D3D11_BLEND_DEST_ALPHA; //****The blend factor is (Ad, Ad, Ad, Ad), that is alpha data from a render target.
	m_blend_descripton.RenderTarget[ 0 ].BlendOp				= D3D11_BLEND_OP_ADD; //Add source 1 and source 2.
	m_blend_descripton.RenderTarget[ 0 ].SrcBlendAlpha			= D3D11_BLEND_ZERO; //The blend factor is (0, 0, 0, 0)
	m_blend_descripton.RenderTarget[ 0 ].DestBlendAlpha			= D3D11_BLEND_ZERO;
	m_blend_descripton.RenderTarget[ 0 ].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	m_blend_descripton.RenderTarget[ 0 ].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	m_video_device->CreateBlendState( & m_blend_descripton , m_blend_state.ReleaseAndGetAddressOf() );
	m_video_device_context->OMSetBlendState( m_blend_state.Get() , m_blend_factor , m_sample_mask );

}

//void DX11::update( ){	//float delta_time}
//void DX11::render( ){clear();present();}

void DX11::clear()
{
	// Clear the views.
	float clear_colour[ 4 ] { 0.0f, 0.0f, 0.8f, 0.0f };

	m_video_device_context->ClearRenderTargetView( m_render_target_view.Get() , 
												   clear_colour );  // 4-component colour array color to fill // dx9 D3DCOLOR_RGBA() );

	m_video_device_context->ClearDepthStencilView( m_depth_stencil_view.Get() ,
												   D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL ,
												   1.0f ,	// depth
												   0 );		// stencil
}

void DX11::clear(const XMFLOAT4 in_colour )
{
	// Clear the views.
	float clear_colour[ 4 ];

	clear_colour[ 0 ] = in_colour.x;
	clear_colour[ 1 ] = in_colour.y;
	clear_colour[ 2 ] = in_colour.z;
	clear_colour[ 3 ] = in_colour.w;

	m_video_device_context->ClearRenderTargetView( m_render_target_view.Get() ,
													 clear_colour );  // 4-component colour array color to fill // dx9 D3DCOLOR_RGBA() );

	m_video_device_context->ClearDepthStencilView( m_depth_stencil_view.Get() ,
													 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL ,
													 1.0f ,		// depth
													 0 );		// stencil
}

void DX11::present()
{ 
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.

	HRESULT h_result = m_swap_chain->Present( 1u,	// sync interval
											  0u ); // flags

	// If the device was reset we must completely reinitialise the renderer.
	if( h_result == DXGI_ERROR_DEVICE_REMOVED || h_result == DXGI_ERROR_DEVICE_RESET )
	{
		window_size_update();
	}
	else
	{
		if( FAILED( h_result ) ) 
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
//    long h_result;
//
//    h_result = D3DCompileFromFile( file_path,     // The name of the file that contains the shader code
//                                   nullptr,       // Optional. Pointer to an array of macro definitions 
//                                   nullptr,       // Optional. Pointer to an interface for handling include files
//                                   entry,         // Name of the shader-entry point function where shader execution begins.
//                                   shader_model,  // string that specifies the shader model 
//                                   shader_flags,  // Shader compile flags
//                                   0,             // Effect compile flags
//                                   buffer,        // pointer to memory which contains the compiled shader
//                                   &error_buffer ); // pointer to memory which contains a listing of errors and warnings that occurred during compilation
//
//    if( FAILED( h_result ) )
//    {
//        if( error_buffer ) // != 0
//        {
//			ErrorExit( static_cast< LPCTSTR >( error_buffer->GetBufferPointer( ) ) );
//        }
//        return h_result;
//    }
//    
//    if( error_buffer != 0 ) error_buffer->Release( );
//
//    return h_result;
//}

void DX11::gpu_device_lost( )
{
	// ComPtr::Reset = Release all pointer references to the associated interface

	//m_map_cameras.clear();

	m_sampler_state.Reset();

	m_vertex_buffer.Reset();
	m_input_layout.Reset();

	m_pixel_shader.Reset();
	m_vertex_shader.Reset();
	
	//m_p_constant_buffer_projection.Reset();
	//m_p_constant_buffer_view.Reset();	
	//m_p_constant_buffer_world.Reset();
	
	m_depth_stencil_view.Reset();
	m_depth_stencil_texture.Reset();

	m_render_target_view.Reset();
	m_render_target_texture.Reset();
	
	m_swap_chain.Reset();

	m_DX_debug.Reset();

	m_video_device_context.Reset();
	m_video_device.Reset();


	create_dx11_device();

	window_size_update();	
}

//------------ Compile VS ------------

//ID3DBlob * vertex_shader_buffer = nullptr;
//h_result = compile_shader( L"shaders/texture_map.fx" , "VS_Main" , "vs_4_0" , & vertex_shader_buffer );
//if( FAILED( h_result ) ) { ErrorExit( L"compile_shader-vertex_shader error\n" ); }

///*
//ID3DBlob * vertex_shader_cso = nullptr;
//D3DReadFileToBlob( file_path , & vertex_shader_cso );
//*/

////------------ Create VS ------------
//h_result = m_p_video_device->CreateVertexShader( vertex_shader_buffer->GetBufferPointer() ,	// A pointer to the compiled shader
//												 vertex_shader_buffer->GetBufferSize() ,		// Size of the compiled vertex shader
//												 nullptr ,					 // A pointer to a class linkage interface, the value can be NULL
//												 m_p_vertex_shader_solid_colour.GetAddressOf() );	// Address of a pointer to a ID3D11VertexShader interface
//	

//if ( FAILED( h_result ) ) { ErrorExit( L"CreateVertexShader error" ); }
////if ( vertex_shader_buffer ) vertex_shader_buffer->Release( ); return false;

////------------ Compile PS ------------
//ID3DBlob * pixel_shader_buffer = nullptr;

//// LPCSTR profile = ( device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) ? "cs_5_0" : "cs_4_0";
//// Prefer higher CS shader profile when possible as CS 5.0 provides better performance on 11-class hardware.

//h_result = compile_shader( L"shaders/texture_map.fx" , "PS_Main" , "ps_4_0" , & pixel_shader_buffer );
//if ( FAILED( h_result ) ) { ErrorExit( L"compile_shader-pixel_shader error\n" ); }

////------------ Create PS  ------------
//h_result = m_p_video_device->CreatePixelShader( pixel_shader_buffer->GetBufferPointer( ) ,	// compiled shader pointer
//												pixel_shader_buffer->GetBufferSize( ) ,		// size of compiled pixel shader
//												nullptr ,									// class linkage interface pointer, value can be NULL
//												m_p_pixel_shader_solid_colour.GetAddressOf() );	// ID3D11PixelShader interface pointer address

////pixel_shader_buffer->Release( );

//if ( FAILED( h_result ) ) {	ErrorExit( L"CreatePixelShader error\n" ); }

////------------ Create input layout ------------
//// * * * * * * * * * * * * * * * * * * * * *
//unsigned int total_layout_elements = ARRAYSIZE( input_layout_pos_texture );
//// * * * * * * * * * * * * * * * * * * * * *

//h_result = m_p_video_device->CreateInputLayout( input_layout_pos_texture ,	// input-assembler stage input data types array
//												total_layout_elements ,		// Total input-data types in array of input-elements
//												vertex_shader_buffer->GetBufferPointer() ,	// compiled shader pointer
//												vertex_shader_buffer->GetBufferSize() ,		// size of compiled shader
//												m_p_input_layout.GetAddressOf() );		// output pointer to created input-layout object

//if( FAILED( h_result ) )
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