using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;
using namespace std::filesystem;

bool Texture2D::Lock( RefPtr<CDeviceContext>& deviceContext, bool sync )
{
	if ( !hasTransitionFlag )
	{
		// ���ҽ� ���¸� �ȼ� ���̴� �ڿ� ���·� �����մϴ�.
		deviceContext->TransitionBarrier( pTexture2D.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0 );
		hasTransitionFlag = true;
	}

	if ( !copySuccessFlag )
	{
		// ���簡 ���� �Ϸ���� �ʾ��� ���,
		if ( auto pFence = GlobalVar.device->CopyQueue->pFence.Get(); pFence->GetCompletedValue() < uploadFenceValue )
		{
			if ( syncFlag == false && sync )
			{
				// ��� ť���� ���� �۾��� �Ϸ�� ������ ����϶�� �����մϴ�.
				HR( deviceContext->pCommandQueue->pCommandQueue->Wait( pFence, uploadFenceValue ) );
				syncFlag = true;
			}

			return false;
		}
		else
		{
			copySuccessFlag = true;
			pUploadCommands = nullptr;
			pUploadHeap = nullptr;
		}
	}

	return true;
}

bool Texture2D::IsValid_get()
{
	if ( !copySuccessFlag )
	{
		if ( auto pFence = GlobalVar.device->CopyQueue->pFence.Get(); pFence->GetCompletedValue() < uploadFenceValue )
		{
			return false;
		}
	}
	return true;
}

Texture2D::~Texture2D()
{
	GC.Add( pTexture2D );
	GC.Add( pShaderResourceView );
	GC.Add( pUploadCommands );
	GC.Add( pUploadHeap );
}

Texture2D::Texture2D( String name, void* textureData, uint32 sizeInBytes ) : Assets( name )
{
	auto pImagingFactory = GlobalVar.factory->pWICFactory.Get();

	ComPtr<IWICStream> pStream;
	ComPtr<IWICBitmapDecoder> pDecoder;

	// ��Ʈ���� ���� �޸� �����͸� �ҷ��ɴϴ�.
	HR( pImagingFactory->CreateStream( &pStream ) );
	HR( pStream->InitializeFromMemory( ( WICInProcPointer )textureData, sizeInBytes ) );

	// ��Ʈ�� �����͸� ������� �̹��� ���ڴ��� �����մϴ�.
	HR( pImagingFactory->CreateDecoderFromStream( pStream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder ) );

	// ���ڴ� ��ü�� �̿��Ͽ� �̹��� ��ü�� �ʱ�ȭ�մϴ�.
	InitializeFrom( pDecoder.Get() );
}

Texture2D::Texture2D( String name, const path& filepath ) : Assets( name )
{
	auto pImagingFactory = GlobalVar.factory->pWICFactory.Get();
	ComPtr<IWICBitmapDecoder> pDecoder;

	// ���� �����͸� ������� �̹��� ���ڴ��� �����մϴ�.
	HR( pImagingFactory->CreateDecoderFromFilename( filepath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder ) );

	// ���ڴ� ��ü�� �̿��Ͽ� �̹��� ��ü�� �ʱ�ȭ�մϴ�.
	InitializeFrom( pDecoder.Get() );
}

uint32 Texture2D::Width_get()
{
	return width;
}

uint32 Texture2D::Height_get()
{
	return height;
}

void Texture2D::InitializeFrom( IWICBitmapDecoder* pDecoder )
{
	auto pWICImagingFactory = GlobalVar.factory->pWICFactory.Get();
	auto pDevice = GlobalVar.device->pDevice.Get();
	ComPtr<IWICBitmapFrameDecode> pFrame;
	ComPtr<IWICFormatConverter> pConverter;

	// �̹����� 0�� �������� ���ɴϴ�.
	HR( pDecoder->GetFrame( 0, &pFrame ) );

	// ������ ��ü�� ������ �̹����� ������ ��ȯ�մϴ�.
	HR( pWICImagingFactory->CreateFormatConverter( &pConverter ) );
	HR( pConverter->Initialize( pFrame.Get(), GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom ) );

	// �̹����� ũ�⸦ ��ȸ�մϴ�.
	UINT width, height;
	HR( pConverter->GetSize( &width, &height ) );

	// �ؽ�ó ������ �����մϴ�.
	D3D12_RESOURCE_DESC textureDesc{ };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc = { 1, 0 };

	// �ؽ�ó ������ ������� ������ ũ�⸦ ��ȸ�մϴ�.
	UINT64 totalSizeInBytes;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
	pDevice->GetCopyableFootprints( &textureDesc, 0, 1, 0, &layout, nullptr, nullptr, &totalSizeInBytes );

	// ���� ������ �����մϴ�.
	D3D12_RESOURCE_DESC bufferDesc{ };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = totalSizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���ε� �� �ڿ��� �����մϴ�.
	D3D12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_UPLOAD };
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS( &pUploadHeap ) ) );

	// ���� �ؽ�ó �ڿ��� �����մϴ�.
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS( &pTexture2D ) ) );

	// ���ε� ���� �ؽ�ó �����͸� �����մϴ�.
	void* pBlock = nullptr;
	HR( pUploadHeap->Map( 0, nullptr, &pBlock ) );
	HR( pConverter->CopyPixels( nullptr, ( UINT )layout.Footprint.RowPitch, ( UINT )totalSizeInBytes, ( BYTE* )pBlock ) );
	pUploadHeap->Unmap( 0, nullptr );

	// �ؽ�ó ���� ��� �Ҵ��� ���� ��� �Ҵ�⸦ �غ��մϴ�.
	HR( pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS( &pUploadCommands ) ) );
	var deviceContext = new CDeviceContext( GlobalVar.device, D3D12_COMMAND_LIST_TYPE_COPY, pUploadCommands.Get() );

	D3D12_TEXTURE_COPY_LOCATION dst{ };
	auto src = dst;

	// ���� ��� ���� ������ �����մϴ�.
	dst.pResource = pTexture2D.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;

	// ���� ���� ���� ������ �����մϴ�.
	src.pResource = pUploadHeap.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = layout;

	// ���� ����� �߰��մϴ�.
	deviceContext->pCommandList->CopyTextureRegion( &dst, 0, 0, 0, &src, nullptr );

	// ��� ����� �ݰ� ����� ī�� ť�� �����մϴ�.
	deviceContext->Close();
	GlobalVar.device->CopyQueue->Execute( deviceContext );
	uploadFenceValue = GlobalVar.device->CopyQueue->Signal();

	// ���̴� �ڿ� �����ڸ� �����մϴ�.
	pShaderResourceView = GlobalVar.device->CreateShaderResourceView( pTexture2D.Get(), nullptr );

	this->width = width;
	this->height = height;
}