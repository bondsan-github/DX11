#include "WICImage.h"

WICImage::WICImage( const wstring in_filename ) // std::string
{
	load( in_filename );
}

void WICImage::load( const wstring filename )
{
	// HRESULT WICCreateImagingFactory_Proxy( _In_ UINT SDKVersion , _Out_ IWICImagingFactory **ppIImagingFactory );

	result = CoCreateInstance( CLSID_WICImagingFactory , nullptr , CLSCTX_INPROC_SERVER , IID_PPV_ARGS( & imaging_factory ) );
	if( FAILED( result ) ) ErrorExit( L"WICImage::load error; CoCreateInstance" );	
	

	result = imaging_factory->CreateDecoderFromFilename( filename.data(), // Image to be decoded
														 nullptr , // Do not prefer a particular vendor
														 GENERIC_READ , // Desired read access to the file
														 WICDecodeMetadataCacheOnDemand , // Cache metadata when needed // WICDecodeMetadataCacheOnLoad
														 & bitmap_decoder ); // Pointer to the decoder  

	if( FAILED( result ) ) ErrorExit( L"WICImage::load() error; CreateDecoderFromFilename" );

	result = bitmap_decoder->GetFrameCount( & frame_count );

	//for( uint i = 0; i < frame_count; i++ )	
	result = bitmap_decoder->GetFrame( 0 , & bitmap_frame_decode );

	// image information
	bitmap_frame_decode->GetSize( & width , & height );	

	uchar pitch_padding = 0;

	if( width % 4 != 0 )
	{
		pitch_padding = width % 4;
	}
	
	bitmap_frame_decode->GetPixelFormat( & wic_pixel_format_guid );
	
	imaging_factory->CreateComponentInfo( wic_pixel_format_guid , & component_info );
	component_info->QueryInterface( & pixel_format_info );

	pixel_format_info->GetFormatGUID( & format_guid );
	pixel_format_info->GetBitsPerPixel( & bits_per_pixel );
	pixel_format_info->GetChannelCount( & channel_count );
	pixel_format_info->GetFriendlyName( 128 , format_name , & chars_read );
		
	switch( bits_per_pixel ) 
	{
		case 24:
		{
		}
		case 32:
		{
		}
		default: {}
	}
	
	// _pixel_format.dxgi_format = pixel_format_wic;
	
	result = imaging_factory->CreateFormatConverter( & format_converter );
	
	result = imaging_factory.Reset();

	result = format_converter->CanConvert( wic_pixel_format_guid , GUID_WICPixelFormat32bppRGBA , & can_convert );

	result = format_converter->Initialize( bitmap_frame_decode.Get() ,
										   GUID_WICPixelFormat32bppRGBA , //wic_pixel_format_guid // = DXGI_FORMAT_R8G8B8A8_UINT
										   WICBitmapDitherTypeNone ,
										   nullptr ,
										   0.0f , // alphaThresholdPercent
										   WICBitmapPaletteTypeCustom );

	if( FAILED( result ) ) ErrorExit( L"WICImage::load() error; Initialize" );

	/*
	int bitsPerPixel = ((int)format & 0xff00) >> 8;
	int bytesPerPixel = (bitsPerPixel + 7) / 8;
	int stride = 4 * ((width * bytesPerPixel + 3) / 4);
	*/

	row_byte_pitch = width * 4; //(bits_per_pixel / 8);// (_width * bits_per_pixel + 7 )/ 8 ; // ? must be padded or multiple of 4 , row_pitch = ( width * bitspp + 7 ) / 8;
	size_bytes		= row_byte_pitch * height;

	pixels.reserve( size_bytes );	

	// copy pixels is where the image format conversion executes
	result = format_converter->CopyPixels(	nullptr, // rectangle to copy, null = entire bitmap
											row_byte_pitch, // bitmap stride
											size_bytes, // size buffer
										    pixels.data() ); // buffer pointer // BYTE == uchar

	if( FAILED( result ) ) ErrorExit( L"WICImage::load() error; CopyPixels" );

	bitmap_frame_decode.Reset();
	bitmap_decoder.Reset();
	format_converter.Reset();
}

//DXGI_FORMAT _WICToDXGI( const GUID& guid )
//{
//	// for all wic formats
//	for( size_t i = 0; i < _countof( g_WICFormats ); ++i )
//	{
//
		// if 
//		if( memcmp( &g_WICFormats[ i ].wic , &guid , sizeof( GUID ) ) == 0 )
//			return g_WICFormats[ i ].format;
//	}
//
//#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
//	if( g_WIC2 )
//	{
//		if( memcmp( &GUID_WICPixelFormat96bppRGBFloat , &guid , sizeof( GUID ) ) == 0 )
//			return DXGI_FORMAT_R32G32B32_FLOAT;
//	}
//#endif
//
//	return DXGI_FORMAT_UNKNOWN;
//}

	////if( FAILED( result ) ) ErrorExit( L"WICImage::load() error; CreateFormatConverter" );


	//size_t charsize = strlen( in_filename ) + 1;
	//const size_t newsize = 100;
	//size_t convertedChars = 0;
	//wchar_t wcstring[ newsize ];// receive wide? string

	//mbstowcs_s( &convertedChars , wcstring , charsize , in_filename , _TRUNCATE ); 
	//mh_result = mp_WICFactory->CreateDecoderFromFilename( wcstring , // Image to be decoded
