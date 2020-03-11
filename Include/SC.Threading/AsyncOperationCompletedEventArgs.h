#pragma once

namespace SC::Threading
{
	/// <summary> 비동기 작업이 완료되었을 경우 발생되는 이벤트에 대한 이벤트 매개변수를 제공합니다. </summary>
	template< class TResult >
	class AsyncOperationCompletedEventArgs : public EventArgs
	{
	public:
		/// <summary> (Visual Studio 전용) 비동기 작업 결과에 대한 인터페이스를 가져옵니다. </summary>
		vs_property_get( RefPtr<IAsyncOperation<TResult>>, AsyncInfo );

		/// <summary> (Visual Studio 전용) 비동기 작업 결과에 대한 인터페이스를 가져옵니다. </summary>
		virtual RefPtr<IAsyncOperation<TResult>> AsyncInfo_get() const = 0;
	};
}