using namespace SC;
using namespace SC::Details;

template<>
StaticAllocator<16>& Details::GetStaticAlloc<16>()
{
	static StaticAllocator<16> instance;
	return instance;
}

template<>
StaticAllocator<32>& Details::GetStaticAlloc<32>()
{
	static StaticAllocator<32> instance;
	return instance;
}

template<>
StaticAllocator<64>& Details::GetStaticAlloc<64>()
{
	static StaticAllocator<64> instance;
	return instance;
}

template<>
StaticAllocator<128>& Details::GetStaticAlloc<128>()
{
	static StaticAllocator<128> instance;
	return instance;
}

template<>
StaticAllocator<256>& Details::GetStaticAlloc<256>()
{
	static StaticAllocator<256> instance;
	return instance;
}

template<>
StaticAllocator<512>& Details::GetStaticAlloc<512>()
{
	static StaticAllocator<512> instance;
	return instance;
}