#pragma once

namespace SC::Game::Details
{
	class CSwapChain : virtual public Object
	{
		ComPtr<ID3D12DescriptorHeap> pDescriptorHeapRTV;
		uint32 rtvStride = 0;

	public:
		CSwapChain();

		void ResizeBuffers( uint32 width, uint32 height );
		void Present( int syncInterval = 0 );
		
		vs_property_get( int, Index );
		int Index_get();

		static constexpr const int BufferCount = 3;
		ComPtr<IDXGISwapChain4> pSwapChain;

		ComPtr<ID3D12Resource> ppBackBuffers[BufferCount];
		D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle[BufferCount];
	};
}