#include "..\Header files\File.h"

//File::File() { }

File::File( const wstring filename ) //: m_p_vector_file_data( std::make_unique< std::vector< char > >() )
{
	load( filename );
}

File::~File()
{
	//m_p_vector_file_data->clear();
	//delete[ ] m_ucp_file_data;
}

void File::load( const wstring filename )
{
	// open binary, with; ptr position at eof
	m_in_file_stream.open( filename, std::ios::binary | std::ios::beg );
	//  m_ifs_file.seekg( 0 , std::ios_base::end );


	//std::istreambuf_iterator< char > file_start( m_ifs_file ) , file_end;
	//m_p_vector_file_data->assign( file_start , file_end );

	if( ! m_in_file_stream.eof() && ! m_in_file_stream.fail() )
	{		
		//m_ifs_file.seekg( 0 , std::ios_base::end );
		std::streampos file_size = m_in_file_stream.tellg();

		//debug_out( "\n file size : %d \n" , static_cast<int>(file_size) );

		m_content->resize( file_size );

		m_in_file_stream.seekg( 0 , std::ios_base::beg );
		//m_ifs_file.read( m_p_vector_file_data.get()->data() , file_size );
		m_in_file_stream.read( m_content->data() , file_size );

		//debug_out( "\n vector size : %d \n" , m_p_vector_file_data->size() );

		m_in_file_stream.seekg( 0 , std::ios_base::beg );

		/*struct format
		{

			vector<char> identifier;
			long offset;
		};*/

		//format bitmap {"bmp" , 0};

		//for( auto format &format_loaders :  )


		//m_ifs_file
			// search n bytes of file for matching format bytes

		
	}
	else
	{
		debug_out( "\n error loading file - filename : %ls \n" , filename );
		ErrorExit( L"file.load error; unable to load file" );
	}

	//m_ifs_file.close();
	//istreambuf_iterator(); Constructs an end-of-stream iterator.

	// std::istream_iterator<T> is for formatted input , 
	// i.e. , it is going go skip leading whitespace before trying to read an object of type T
	// std::istreambuf_iterator<char> which is used to iterate over the characters in a file, not doing any skipping.
		
	/*
	if( ( m_h_file_handle = CreateFile( filename ,
		GENERIC_READ ,				// desired access
		FILE_SHARE_READ ,			// share mode
		nullptr ,					// security attributes
		OPEN_EXISTING ,				// creation disposition
		FILE_ATTRIBUTE_NORMAL ,		// flags and attributes
		0 ) )						// handle template file

		== INVALID_HANDLE_VALUE )
	{
		// debug_out( L" filename : %s \n " , filename );
		// OutputDebugString( L"\n****** file load ******\n" );
		// OutputDebugString( filename );
		// OutputDebugString( L"\n" );	
		ErrorExit( L"file.load : could not open file." );
		CloseHandle( m_h_file_handle );
	}
	
	m_ul_file_size = GetFileSize( m_h_file_handle , nullptr );
	m_ucp_file_data = new unsigned char[ m_ul_file_size ];  // unsafe C syle = static_cast< UCHAR * >( malloc( m_ul_file_size ) );
	
	if( ! ReadFile( m_h_file_handle , m_ucp_file_data , m_ul_file_size , & m_ul_bytes_read , nullptr ) )
	{
		ErrorExit( L"Image.load : could not read file." );
		CloseHandle( m_h_file_handle );
	}
	*/
}