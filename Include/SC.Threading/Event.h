#pragma once

namespace SC::Threading
{
	/// <summary> 서로 다른 스레드 간 동기화 상태를 전달합니다. </summary>
	class Event : public AsyncWait
	{
		bool isManual = false;

	public:
		/// <summary> <see cref="Event"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="isManual"> 신호 상태를 수동으로 관리합니다. 이 경우 WaitFor... 함수를 사용하여 대기하여도 신호 상태가 변경되지 않습니다. </param>
		Event( bool isManual = false );

		/// <summary> 이벤트의 신호 상태를 활성화합니다. </summary>
		void Set();

		/// <summary> 이벤트의 신호 상태를 비활성화합니다. </summary>
		void Reset();

		/// <summary> (Visual Studio 전용) 이벤트 개체의 현재 신호 상태를 가져옵니다. </summary>
		vs_property_get( bool, IsSignaled );

		/// <summary> 이벤트 개체의 현재 신호 상태를 가져옵니다. </summary>
		[[nodiscard]] bool IsSignaled_get() const;
	};
}