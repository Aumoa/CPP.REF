#pragma once

namespace SC
{
	namespace Details
	{
		struct ReferenceCounter
		{
		private:
			static std::atomic<int> allocRef;

		public:
			uint64 refCount = 0;
			uint64 weakCount = 0;

			void AddRef();
			void Release();
			void AddWeakRef();
			void ReleaseWeak();

			void* operator new( size_t length );
			void operator delete( void* block );

		private:
			static void AddAllocRef();
			static void ReleaseAllocRef();
		};
	}
}