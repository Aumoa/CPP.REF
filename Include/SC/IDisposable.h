#pragma once

namespace SC
{
	/// <summary> 명시적 메모리 관리 모듈이 필요한 개체에서 구현된 개체를 정리하는 함수의 인터페이스를 표현합니다. </summary>
	struct IDisposable : virtual public Object
	{
		/// <summary> 내부 자원을 해제합니다. </summary>
		virtual void Dispose() = 0;
	};
}