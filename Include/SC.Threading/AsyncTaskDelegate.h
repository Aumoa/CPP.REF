#pragma once

namespace SC::Threading
{
	/// <summary> 스레드 풀에 제출하는 비동기 작업 콜백 함수를 나타냅니다. </summary>
	template< class T >
	using AsyncTaskDelegate = std::function<T( object args )>;

	/// <summary> 체인으로 연결 된 작업에 대한 매개 변수를 사용하는 비동기 작업 콜백 함수를 나타냅니다. </summary>
	template< class T >
	using AsyncTaskChainDelegate = std::function<T( T results, object args )>;
}