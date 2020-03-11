#pragma once

namespace SC::Game::Details
{
	class ViewStorage : virtual public Object
	{
		CDevice* deviceRef;
		ComPtr<ID3D12Device> pDevice;

		ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE handleBase{ };
		UINT stride = 0;

		int capacity;
		std::queue<int> index_queue;
		std::mutex locker;

	public:
		ViewStorage( CDevice* device, int capacity = 1024 );

		int Lock();
		void Free( int lockIndex );

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle( int lockIndex );
	};
}