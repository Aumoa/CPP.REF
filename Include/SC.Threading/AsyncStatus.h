#pragma once

namespace SC::Threading
{
	/// <summary> 비동기 작업의 상태를 표현합니다. </summary>
	enum class AsyncStatus
	{
		/// <summary> 작업 상태가 게시되지 않았습니다. 일반적으로 이 상황은 오류로 간주합니다. </summary>
		Unspecified = -1,

		/// <summary> 작업이 시작되었습니다. </summary>
		Started = 0,

		/// <summary> 작업이 완료되었습니다. </summary>
		Completed = 1,

		/// <summary> 작업이 취소되었습니다. </summary>
		Canceled = 2,
		
		/// <summary> 작업을 진행하는 중 오류가 발생하였습니다. </summary>
		Error = 3,

		/// <summary> 작업이 대기중입니다. </summary>
		Pending = 4,
	};
}

inline std::wostream& operator<<( std::wostream& wout, SC::Threading::AsyncStatus value )
{
	static constexpr const wchar_t* context[] =
	{
		L"Unspecified",
		L"Started",
		L"Completed",
		L"Canceled",
		L"Error",
		L"Pending",
	};

	if ( ( int )value >= -1 && ( int )value <= 4 )
	{
		return wout << context[( int )value + 1];
	}
	else
	{
		return wout << L"SC.Threading.AsyncStatus(" << ( int )value << L")";
	}
}