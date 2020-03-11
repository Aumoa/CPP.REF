#pragma once

namespace SC
{
	/// <summary> SC 값 형식의 기본 클래스를 제공합니다. </summary>
	class ValueType : virtual public Object
	{
	protected:
		/// <summary> <see cref="ValueType"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		ValueType();

		/// <summary> <see cref="ValueType"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		ValueType( const ValueType& ) = default;
	};
}