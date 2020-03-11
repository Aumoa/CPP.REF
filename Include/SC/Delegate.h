#pragma once

namespace SC
{
	/// <summary> ���� �޼��� �Ǵ� Ŭ���� �ν��Ͻ� �� �ش� Ŭ������ �ν��Ͻ� �޼��带 �����ϴ� ������ �����Դϴ�. </summary>
	struct Delegate : public ValueType
	{
	protected:
		/// <summary> <see cref="Delegate"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Delegate() = default;

	public:
		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		String ToString() override;
	};
}