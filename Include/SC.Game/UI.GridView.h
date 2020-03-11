#pragma once

namespace SC::Game::UI
{
	/// <summary> ���� �׸��� ���� ���·� �����ϱ� ���� View ����Դϴ�. </summary>
	class GridView : public ViewBase
	{
		double gridWidth = 0;
		double gridHeight = 0;

	protected:
		/// <summary> (<see cref="Panel"/> Ŭ�������� ��� ��.) ��Ҹ� �����մϴ�. </summary>
		/// <param name="clientRect"> �θ��� ������ �簢 ������ ���޵˴ϴ�. </param>
		Drawing::Rect<double> OnUpdate( Drawing::Rect<double> clientRect ) override;

	public:
		/// <summary> <see cref="GridView"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		GridView( String name );

		/// <summary> (Visual Studio ����) ���� �� ����� ���̸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, GridWidth );

		/// <summary> ���� �� ����� ���̸� �����ɴϴ�. </summary>
		double GridWidth_get();

		/// <summary> ���� �� ����� ���̸� �����մϴ�. </summary>
		void GridWidth_set( double value );

		/// <summary> (Visual Studio ����) ���� �� ����� ���̸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, GridHeight );

		/// <summary> ���� �� ����� ���̸� �����ɴϴ�. </summary>
		double GridHeight_get();

		/// <summary> ���� �� ����� ���̸� �����մϴ�. </summary>
		void GridHeight_set( double value );
	};
}