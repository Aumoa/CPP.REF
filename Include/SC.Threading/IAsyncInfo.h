#pragma once

namespace SC::Threading
{
	/// <summary> 비동기 작업에 대한 기초 정보를 표현합니다. </summary>
	struct IAsyncInfo : virtual public Object
	{
		/// <summary> 현재 비동기 작업을 취소합니다. </summary>
		virtual void Cancel() = 0;

		/// <summary> 현재 비동기 작업을 종료합니다. </summary>
		virtual void Close() = 0;

		/// <summary> (Visual Studio 전용) 비동기 작업에 대한 ID를 가져옵니다. </summary>
		vs_property_get( int64, Id );

		/// <summary> 비동기 작업에 대한 ID를 가져옵니다. </summary>
		virtual int64 Id_get() = 0;

		/// <summary> (Visual Studio 전용) 비동기 작업에서 발생한 예외 정보를 가져옵니다. </summary>
		vs_property_get( RefPtr<Exception>, Error );

		/// <summary> 비동기 작업에서 발생한 예외 정보를 가져옵니다. </summary>
		virtual RefPtr<Exception> Error_get() = 0;

		/// <summary> (Visual Studio 전용) 현재 비동기 작업의 상태를 나타내는 값을 가져옵니다. </summary>
		vs_property_get( AsyncStatus, Status );

		/// <summary> 현재 비동기 작업의 상태를 나타내는 값을 가져옵니다. </summary>
		virtual AsyncStatus Status_get() = 0;
	};
}