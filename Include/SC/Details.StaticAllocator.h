#pragma once

namespace SC::Details
{
	static constexpr const size_t _4MB = 1024 * 1024 * 4;

	template< size_t _BlockSz, size_t _Count = _4MB / _BlockSz >
	class StaticAllocator
	{
		static constexpr size_t BlockSz = _BlockSz;
		static constexpr size_t Count = _Count;

		size_t length = 0;
		char* block;
		size_t* que;
		size_t queIdx = 0;
		size_t queCnt = 0;

		std::mutex locker;

	public:
		void* Alloc( size_t length );
		void Free( void* block );
		bool CheckHeap( void* block );

		void Wakeup();
		void Cleanup();

	private:
		size_t Pop();
		void Push( size_t idx );
		void Clamp( size_t* idx );
	};

	template< size_t BlockSize >
	StaticAllocator<BlockSize>& GetStaticAlloc();

	template<> StaticAllocator<16>& GetStaticAlloc<16>();
	template<> StaticAllocator<32>& GetStaticAlloc<32>();
	template<> StaticAllocator<64>& GetStaticAlloc<64>();
	template<> StaticAllocator<128>& GetStaticAlloc<128>();
	template<> StaticAllocator<256>& GetStaticAlloc<256>();
	template<> StaticAllocator<512>& GetStaticAlloc<512>();
}