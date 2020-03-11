#pragma once

namespace SC::Game::Details
{
	class CDynamicBuffer : virtual public Object
	{
		int indexOf = -1;
		RefPtr<HeapAllocator> pAllocator;

	public:
		CDynamicBuffer( RefPtr<HeapAllocator> pAllocator, int indexOf, D3D12_GPU_VIRTUAL_ADDRESS virtualAddress, void* pBlock );
		CDynamicBuffer( ComPtr<ID3D12Resource>&& pResource );
		~CDynamicBuffer() override;

		ComPtr<ID3D12Resource> pResource;
		D3D12_GPU_VIRTUAL_ADDRESS VirtualAddress;
		void* pBlock;
	};
}