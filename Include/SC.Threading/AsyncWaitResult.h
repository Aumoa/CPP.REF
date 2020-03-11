#pragma once

namespace SC::Threading
{
	/// <summary> 비동기 개체 대기 결과를 표현합니다. </summary>
	enum class AsyncWaitResult
	{
		/// <summary> 해당 개체는 소유 스레드가 종료되었지만 명시적으로 해제되지 않았습니다. </summary>
		Abandoned,

		/// <summary> 해당 개체의 작업이 완료되어 대기 명령이 반환되었습니다. </summary>
		Completed,

		/// <summary> 해당 개체의 작업 대기 시간이 만료되어 대기 명령이 반환되었습니다. </summary>
		Timeout,

		/// <summary> 해당 개체에 대기 명령이 실패하였습니다. 지원하지 않는 개체이거나 권한이 부족할 수 있습니다. </summary>
		Failed,
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Threading::AsyncWaitResult value )
{
	static constexpr const wchar_t* context[] =
	{
		L"Abandoned",
		L"Completed",
		L"Timeout",
		L"Failed",
	};

	if ( ( int )value >= 0 && ( int )value <= 3 )
	{
		return wout << context[( int )value];
	}
	else
	{
		return wout << L"SC.Threading.AsyncStatus(" << ( int )value << L")";
	}
}