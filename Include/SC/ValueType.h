#pragma once

namespace SC
{
	/// <summary> SC �� ������ �⺻ Ŭ������ �����մϴ�. </summary>
	class ValueType : virtual public Object
	{
	protected:
		/// <summary> <see cref="ValueType"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		ValueType();

		/// <summary> <see cref="ValueType"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		ValueType( const ValueType& ) = default;
	};
}