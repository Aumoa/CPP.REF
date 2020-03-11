#pragma once

namespace SC::Details
{
	template< size_t _BlockSz, size_t _Count >
	void* StaticAllocator<_BlockSz, _Count>::Alloc( size_t length )
	{
		if ( length > _BlockSz )
		{
			return malloc( length );
		}
		else if ( queCnt == 0 )
		{
			return malloc( length );
		}
		else
		{
			auto idx = Pop();
			return block + idx * _BlockSz;
		}
	}

	template< size_t _BlockSz, size_t _Count >
	void StaticAllocator<_BlockSz, _Count>::Free( void* block )
	{
		if ( ( char* )block >= this->block )
		{
			size_t ptrdif = ( size_t )( ( char* )block - this->block );
			if ( ptrdif < length )
			{
				Push( ptrdif / _BlockSz );
				return;
			}
		}

		free( block );
	}

	template< size_t _BlockSz, size_t _Count >
	bool StaticAllocator<_BlockSz, _Count>::CheckHeap( void* block )
	{
		if ( ( char* )block >= this->block )
		{
			size_t ptrdif = ( size_t )( ( char* )block - this->block );
			if ( ptrdif < length )
			{
				return true;
			}
		}

		return false;
	}

	template< size_t _BlockSz, size_t _Count >
	void StaticAllocator<_BlockSz, _Count>::Wakeup()
	{
		length = _BlockSz * _Count;
		block = ( char* )malloc( length );
		que = ( size_t* )malloc( _Count * sizeof( size_t ) );

		queCnt = _Count;
		for ( size_t i = 0; i < _Count; ++i )
		{
			que[i] = i;
		}
	}

	template< size_t _BlockSz, size_t _Count >
	void StaticAllocator<_BlockSz, _Count>::Cleanup()
	{
		if ( block )
		{
			free( block );
			block = nullptr;
		}

		if ( que )
		{
			free( que );
			que = nullptr;
		}
	}

	template< size_t _BlockSz, size_t _Count >
	size_t StaticAllocator<_BlockSz, _Count>::Pop()
	{
		std::lock_guard<std::mutex> lock( locker );

		auto ret = que[queIdx++];
		queCnt -= 1;
		Clamp( &queIdx );
		return ret;
	}

	template< size_t _BlockSz, size_t _Count >
	void StaticAllocator<_BlockSz, _Count>::Push( size_t idx )
	{
		std::lock_guard<std::mutex> lock( locker );

		size_t i = queIdx + queCnt++;
		Clamp( &i );
		que[i] = idx;
	}

	template< size_t _BlockSz, size_t _Count >
	void StaticAllocator<_BlockSz, _Count>::Clamp( size_t* idx )
	{
		if ( *idx >= _Count )
		{
			*idx -= _Count;
		}
	}

	template< size_t BlockSize>
	StaticAllocator<BlockSize>& GetStaticAlloc()
	{
		static StaticAllocator<BlockSize> instance;
		return instance;
	}
}