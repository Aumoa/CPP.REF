#pragma once

namespace SC::Game::UI
{
	/// <summary> ��ȣ ������ ��ġ�� ����Ͽ� ����� ��ġ�� �����ϴ� �г� �����Դϴ�. </summary>
	class RelativePanel : public Panel
	{
	protected:
		/// <summary> (<see cref="Element"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

	public:
		/// <summary> <see cref="RelativePanel"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		RelativePanel( String name );

		/// <summary> �� Ŭ������ �̸� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t ClassName;

		/// <summary> ���� �гο� �Ҽӵ� ����� ���ʿ� ��ġ�ϵ��� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t LeftOf;

		/// <summary> ���� �гο� �Ҽӵ� ����� ���ʿ� ��ġ�ϵ��� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t Above;

		/// <summary> ���� �гο� �Ҽӵ� ����� �����ʿ� ��ġ�ϵ��� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t RightOf;

		/// <summary> ���� �гο� �Ҽӵ� ����� �Ʒ��� ��ġ�ϵ��� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t Below;

		/// <summary> ���� �гο� �Ҽӵ� ����� ������ ��ġ�ϵ��� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t AlignLeftWith;

		/// <summary> ���� �гο� �Ҽӵ� ����� ������ ��ġ�ϵ��� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t AlignTopWith;

		/// <summary> ���� �гο� �Ҽӵ� ����� �������� ��ġ�ϵ��� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t AlignRightWith;

		/// <summary> ���� �гο� �Ҽӵ� ����� �Ʒ����� ��ġ�ϵ��� �����ϴ� �Ӽ��� �ؽ� ���� �����ɴϴ�. </summary>
		static const std::size_t AlignBottomWith;

	private:
		void ComputeRect( int index, std::vector<Drawing::Rect<double>>& computedRects, std::vector<bool>& computed );
	};
}