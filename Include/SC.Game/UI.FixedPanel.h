#pragma once

namespace SC::Game::UI
{
	/// <summary> ���� ��ġ�� ����Ͽ� ����� ��ġ�� �����ϴ� �г� �����Դϴ�. </summary>
	class FixedPanel : public Panel
	{
	protected:
		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

	public:
		/// <summary> <see cref="FixedPanel"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		FixedPanel( String name );

		/// <summary> �� Ŭ������ �̸� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t ClassName;

		/// <summary> ���� �г��� ���� ��ġ�� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t Left;

		/// <summary> ���� �г��� ���� ��ġ�� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t Top;

		/// <summary> ���� �г��� ������ ��ġ�� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t Right;

		/// <summary> ���� �г��� �Ʒ��� ��ġ�� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t Bottom;
	};
}