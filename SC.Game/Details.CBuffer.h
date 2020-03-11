#pragma once

namespace SC::Game::Details
{
	class CBuffer : virtual public Object
	{
		const D3D12_RESOURCE_STATES initialState;

		RefPtr<CDevice>& deviceRef;
		ComPtr<ID3D12CommandAllocator> pUploadCommands;
		ComPtr<ID3D12Resource> pUploadHeap;
		uint64 uploadFenceValue = 0;

		bool copySuccessFlag = true;
		bool hasTransitionFlag = false;
		bool waitingFlag = false;

	public:
		CBuffer( RefPtr<CDevice>& device, uint64 sizeInBytes, D3D12_RESOURCE_STATES initialStates, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE, const void* pInitialData = nullptr, uint64 initialDataSize = 0 );
		~CBuffer() override;

		void Lock( RefPtr<CDeviceContext>& deviceContext, bool sync = true );

		ComPtr<ID3D12Resource> pResource;
	};
}