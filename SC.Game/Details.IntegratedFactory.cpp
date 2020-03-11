using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

IntegratedFactory::IntegratedFactory()
{
	// COM ����� ����ϴ� ���� ��Ҹ� ����ϱ� ���� CoInitialize �Լ��� ȣ���մϴ�.
	HR( CoInitializeEx( nullptr, COINIT_MULTITHREADED ) );

	// DXGI ���丮 ��ü�� �����մϴ�.
	HR( CreateDXGIFactory( IID_PPV_ARGS( &pDXGIFactory ) ) );

	// DirectWrite ���丮 ��ü�� �����մϴ�.
	HR( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory ), ( IUnknown** )&pDWriteFactory ) );

	// WIC ���丮 ��ü�� �����մϴ�.
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