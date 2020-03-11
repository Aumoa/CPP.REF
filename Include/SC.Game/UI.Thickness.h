#pragma once

namespace SC::Game::UI
{
	/// <summary> �簢�� �������� �β��� ǥ���մϴ�. </summary>
	struct Thickness : public ValueType
	{
		/// <summary> ���� �β��� �����մϴ�. </summary>
		double Left = 0;

		/// <summary> ���� �β��� �����մϴ�. </summary>
		double Top = 0;

		/// <summary> ������ �β��� �����մϴ�. </summary>
		double Right = 0;

		/// <summary> �Ʒ��� �β��� �����մϴ�. </summary>
		double Bottom = 0;

		/// <summary> <see cref="Thickness"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Thickness() = default;

		/// <summary> <see cref="Thickness"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Thickness( const Thickness& copy )
			: Left( copy.Left )
			, Top( copy.Top )
			, Right( copy.Right )
			, Bottom( copy.Bottom )
		{

		}

		/// <summary> <see cref="Thickness"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Thickness( double value )
			: Left( value )
			, Top( value )
			, Right( value )
			, Bottom( value )
		{

		}

		/// <summary> <see cref="Thickness"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Thickness( double horizontal, double vertical )
			: Left( horizontal )
			, Top( vertical )
			, Right( horizontal )
			, Bottom( vertical )
		{

		}

		/// <summary> <see cref="Thickness"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Thickness( double left, double top, double right, double bottom )
			: Left( left )
			, Top( top )
			, Right( right )
			, Bottom( bottom )
		{

		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) �ش� ��ü�� �ؽ�Ʈ ǥ���� �����ɴϴ�. </summary>
		String ToString() override
		{
			return String::Format( "{0}, {1}, {2}, {3}", Left, Top, Right, Bottom );
		}

		/// <summary> (Visual Studio ����) ��ü�� ���� ��ȭ���� �����ɴϴ�. </summary>
		vs_property_get( double, WidthDelta );

		/// <summary> ��ü�� ���� ��ȭ���� �����ɴϴ�. </summary>
		double WidthDelta_get()
		{
			return Left + Right;
		}

		/// <summary> (Visual Studio ����) ��ü�� ���� ��ȭ���� �����ɴϴ�. </summary>
		vs_property_get( double, HeightDelta );

		/// <summary> ��ü�� ���� ��ȭ���� �����ɴϴ�. </summary>
		double HeightDelta_get()
		{
			return Top + Bottom;
		}

		Thickness& operator=( const Thickness& copy )
		{
			Left = copy.Left;
			Top = copy.Top;
			Right = copy.Right;
			Bottom = copy.Bottom;

			return *this;
		}
	};
}