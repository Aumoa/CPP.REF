using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

IntegratedFactory::IntegratedFactory()
{
	// COM 기능을 사용하는 구성 요소를 사용하기 위해 CoInitialize 함수를 호출합니다.
	HR( CoInitializeEx( nullptr, COINIT_MULTITHREADED ) );

	// DXGI 팩토리 개체를 생성합니다.
	HR( CreateDXGIFactory( IID_PPV_ARGS( &pDXGIFactory ) ) );

	// DirectWrite 팩토리 개체를 생성합니다.
	HR( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory ), ( IUnknown** )&pDWriteFactory ) );

	// WIC 팩토리 개체를 생성합니다.
	HR( CoCreateInstance( CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS( &pWICFactory ) ) );
}

ComPtr<IDXGIAdapter1> IntegratedFactory::SearchHardwareAdapter()
{
	for ( int i = 0; true; ++i )
	{
		ComPtr<IDXGIAdapter1> pAdapter;
		HRESULT hr = pDXGIFactory->EnumAdapters1( ( UINT )i, &pAdapter );
		if ( hr == DXGI_ERROR_NOT_FOUND )
		{
			return nullptr;
		}
		else if ( FAILED( hr ) ) HR( hr );
		else
		{
			DXGI_ADAPTER_DESC1 desc{ };
			HR( pAdapter->GetDesc1( &desc ) );
			if ( ( desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE ) == 0 )
			{
				return move( pAdapter );
			}
		}
	}
}