#pragma once

namespace SC::Game::Details
{
	class CCommandQueue : virtual public Object
	{
		const D3D12_COMMAND_LIST_TYPE type;

	public:
		CCommandQueue( CDevice* device, D3D12_COMMAND_LIST_TYPE type );

		void Execute( RefPtr<CDeviceContext>& deviceContext );
		void Execute( int numContexts, CDeviceContext* const* ppDeviceContexts );

		uint64 Signal();
		void Signal( ID3D12Fence* pFence, uint64 fenceValue );

		void WaitFor( uint64 fenceValue, RefPtr<Threading::Event>& waitingHandle, uint32 limit = 1000 );

		ComPtr<ID3D12CommandQueue> pCommandQueue;
		ComPtr<ID3D12Fence> pFence;
		uint64 LastPending = 0;
	};
}