#include "WICImage.h"

using std::wstring;

WICImage::WICImage( const wstring in_filename ) // std::string
{
	load( in_filename );
}

void WICImage::load( const wstring in_filename )
{
	/*
	HRESULT WICCreateImagingFactory_Proxy(
		_In_  UINT               SDKVersion ,
		_Out_ IWICImagingFactory **ppIImagingFactory
	);
	*/

	m_result = CoCreateInstance( CLSID_WICImagingFactory ,
								  nullptr ,
								  CLSCTX_INPROC_SERVER ,
								  IID_PPV_ARGS( & m_imaging_factory ) );

	if( FAILED( m_result ) ) ErrorExit( L"Quad_textured() error; CoCreateInstance" );
	
	
	//size_t charsize = strlen( in_filename ) + 1;
	//const size_t newsize = 100;
	//size_t convertedChars = 0;
	//wchar_t wcstring[ newsize ];// receive wide? string

	//mbstowcs_s( &convertedChars , wcstring , charsize , in_filename , _TRUNCATE );
	
	
	//mh_result = mp_WICFactory->CreateDecoderFromFilename( wcstring ,							// Image to be decoded

	m_result = m_imaging_factory->CreateDecoderFromFilename( in_filename.data(),			// Image to be decoded
															 nullptr ,						// Do not prefer a particular vendor
															 GENERIC_READ ,					// Desired read access to the file
															 WICDecodeMetadataCacheOnDemand ,// Cache metadata when needed // WICDecodeMetadataCacheOnLoad
															 m_bitmap_decoder.ReleaseAndGetAddressOf() );	// Pointer to the decoder

	if( FAILED( m_result ) ) ErrorExit( L"WICImage::load() error; CreateDecoderFromFilename" );

	m_bitmap_decoder->GetFrameCount( & m_frame_count );


	// Retrieve the first frame of the image from the decoder
	m_result = m_bitmap_decoder->GetFrame( 0 , // frame number
										   m_bitmap_frame_decode.ReleaseAndGetAddressOf() );
	
	if( FAILED( m_result ) ) ErrorExit( L"WICImage::load() error; GetFrame" );

	m_bitmap_decoder.Reset();

	// get frame image dimensions
	m_result = m_bitmap_frame_decode->GetSize( & m_width , & m_height );

	if( FAILED( m_result ) ) ErrorExit( L"WICImage::load() error; GetSize" );
		
	m_result = m_bitmap_frame_decode->GetPixelFormat( & m_pixel_format );

	if( FAILED( m_result ) ) ErrorExit( L"Quad_textured() error; GetPixelFormat" );

	IWICComponentInfo* pComponentInfo = NULL;
	m_result = m_imaging_factory->CreateComponentInfo( m_pixel_format , &pComponentInfo );

	IWICPixelFormatInfo* pPixelFormatInfo = NULL;
	m_result = pComponentInfo->QueryInterface( &pPixelFormatInfo );

	unsigned int bits_per_pixel = 0;
	m_result = pPixelFormatInfo->GetBitsPerPixel( &bits_per_pixel );

	unsigned int channelCount = 0;
	m_result = pPixelFormatInfo->GetChannelCount( &channelCount );

	wchar_t format_name[ 128 ]{};
	unsigned int chars_read = 0;
	pPixelFormatInfo->GetFriendlyName( 128 , format_name , &chars_read );

	m_result = m_imaging_factory->CreateFormatConverter( m_format_converter.ReleaseAndGetAddressOf() );

	if( FAILED( m_result ) ) ErrorExit( L"WICImage::load() error; CreateFormatConverter" );

	m_imaging_factory.Reset();

	m_result = m_format_converter->Initialize( m_bitmap_frame_decode.Get() ,
											   GUID_WICPixelFormat128bppRGBAFloat,//GUID_WICPixelFormat32bppRGBA ,	// = DXGI_FORMAT_R8G8B8A8_UINT
											   WICBitmapDitherTypeNone ,
											   nullptr ,
											   0.0f ,							// alphaThresholdPercent
											   WICBitmapPaletteTypeCustom );

	if( FAILED( m_result ) ) ErrorExit( L"WICImage::load() error; Initialize" );

	m_bitmap_frame_decode.Reset();

	m_row_byte_pitch	= m_width * 16u; // bytes_per_pixel // uint rowPitch = ( width * bitspp + 7 ) / 8;
	m_size_bytes		= m_row_byte_pitch * m_height;

	m_pixels.reserve( m_size_bytes );

	BOOL can_convert = false;

	m_result = m_format_converter->CanConvert( m_pixel_format , GUID_WICPixelFormat128bppRGBAFloat , & can_convert );

	// copy pixels is where the image format conversion executes
	m_result = m_format_converter->CopyPixels(	nullptr,					// The rectangle to copy. A NULL value specifies the entire bitmap
												m_row_byte_pitch,			// The stride of the bitmap
												m_size_bytes,				// The size of the buffer
												m_pixels.data() );			// A pointer to the buffer // BYTE == uchar

	if( FAILED( m_result ) ) ErrorExit( L"WICImage::load() error; CopyPixels" );

	m_format_converter.Reset();
}