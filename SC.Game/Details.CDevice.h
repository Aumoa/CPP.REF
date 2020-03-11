#pragma once

namespace SC::Game::Details
{
	class CDevice : virtual public Object
	{
	public:
		struct tag_FontFamily
		{
			String Name;
			float FontSize;
			bool IsBold;

			bool operator<( const tag_FontFamily& right ) const
			{
				if ( Name < right.Name ) return true;
				else if ( Name > right.Name ) return false;
				else if ( FontSize < right.FontSize ) return true;
				else if ( FontSize > right.FontSize ) return false;
				else if ( IsBold < right.IsBold ) return true;
				else return false;
			}
		};

	private:
		std::mutex viewStorageLocker;
		std::queue<RefPtr<ViewStorage>> viewStorageChain;

		RefPtr<HeapAllocator> heapAllocator128;  // 1MB
		RefPtr<HeapAllocator> heapAllocator256;  // 1MB
		RefPtr<HeapAllocator> heapAllocator512;  // 1MB

	public:
		CDevice( IDXGIAdapter1* pAdapter );

		void FreeStorage( ViewStorage* pStorage );

		ComPtr<ID3D12RootSignature> CreateRootSignature( const D3D12_ROOT_SIGNATURE_DESC& desc, D3D_ROOT_SIGNATURE_VERSION version = D3D_ROOT_SIGNATURE_VERSION_1_0 );
		RefPtr<CShaderResourceView> CreateShaderResourceView( ID3D12Resource* pResource, const D3D12_SHADER_RESOURCE_VIEW_DESC* pSRVDesc );
		RefPtr<CUnorderedAccessView> CreateUnorderedAccessView( ID3D12Resource* pResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* pUAVDesc );
		RefPtr<CDynamicBuffer> CreateDynamicBuffer( uint64 sizeInBytes, int alignment = 0 );

		ComPtr<IDXGIAdapter1> pAdapter;
		ComPtr<ID3D12Device> pDevice;
		ComPtr<ID3D11Device> pDevice11;
		ComPtr<ID3D11On12Device> pDevice11On12;
		ComPtr<ID3D11DeviceContext> pDeviceContext11;
		ComPtr<ID2D1Device> pDevice2D;
		ComPtr<ID2D1DeviceContext> pDeviceContext2D;

		RefPtr<CCommandQueue> DirectQueue[4];  // 0 is CORE queue, 3 is UI queue
		RefPtr<CCommandQueue> CopyQueue;
		RefPtr<CCommandQueue> ComputeQueue;

	private:
		void InitializeInterop();
	};
}