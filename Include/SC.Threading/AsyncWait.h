#pragma once

namespace SC::Threading
{
	/// <summary> 비동기 개체에서 시스템에서 대기 명령을 수행할 수 있는 개체에 대한 공통 함수를 제공합니다. </summary>
	class AsyncWait : virtual public Object
	{
		void* native_handle = nullptr;

	protected:
		AsyncWait( void* handle );

		virtual bool CustomRelease( void* native_handle );

	public:
		~AsyncWait() override;

		/// <summary> 현재 개체에 대해 대기 명령을 수행합니다. </summary>
		/// <param name="milliseconds"> 대기 명령의 Timeout 시간을 전달합니다. 0xFFFFFFFF 값이 전달될 경우 무한 대기합니다. </param>
		AsyncWaitResult WaitForSingleObject( uint32 milliseconds = 0xFFFFFFFF ) const;

		/// <summary> (Visual Studio 전용) 현재 대기 개체의 비관리 핸들을 가져옵니다. </summary>
		vs_property_get( void*, Handle );

		/// <summary> 현재 대기 개체의 비관리 핸들을 가져옵니다. </summary>
		void* Handle_get() const;

		/// <summary> 지원하는 모든 개체에 대해 대기 명령을 수행합니다. </summary>
		/// <param name="numAsyncWaitObjects"> 대기할 개체 개수를 전달합니다. </param>
		/// <param name="asyncWaitObjects"> 대기할 개체 목록을 전달합니다. </param>
		/// <param name="milliseconds"> 대기 명령의 Timeout 시간을 전달합니다. 0xFFFFFFFF 값이 전달될 경우 무한 대기합니다. </param>
		/// <remarks> 모든 개체가 동기 상태가 되면 <see cref="AsyncWaitResult::Completed"/> 상태를 반환합니다. </remarks>
		//static AsyncWaitResult WaitForAllObjects( int numAsyncWaitObjects, const AsyncWait asyncWaitObjects[], uint32 milliseconds );
	};
}