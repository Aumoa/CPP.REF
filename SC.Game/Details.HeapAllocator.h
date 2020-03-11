#pragma once

namespace SC::Game::Details
{
	class HeapAllocator : virtual public Object
	{
		const int alignOf;
		const uint32 heapCount;

		ComPtr<ID3D12Device> pDevice;
		ComPtr<ID3D12Resource> pDynamicBuffer;

		D3D12_GPU_VIRTUAL_ADDRESS virtualAddress = 0;
		char* pBlock = nullptr;

		std::mutex locker;
		std::queue<int> allocQueue;

	public:
		HeapAllocator( CDevice* device, int alignOf = 512, uint32 heapCount = 2048 );  // 1MB

		int Alloc( D3D12_GPU_VIRTUAL_ADDRESS& virtualAddress, void*& pBlock );
		void Free( int index );
	};
}