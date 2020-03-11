#pragma once

namespace SC::Game::UI::Shapes
{
	/// <summary> ����� ǥ���ϴ� UI ��� ���� ���� ������ ��Ʈ�Դϴ�. </summary>
	class Shape : public Element
	{
		RefPtr<Brush> brush;

	protected:
		/// <summary> <see cref="Shape"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ��ü �̸��� �����մϴ�. </param>
		Shape( String name );

	public:
		/// <summary> (Visual Studio ����) ����� ä��� �귯�ø� �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<Brush>, Fill );

		/// <summary> ����� ä��� �귯�ø� �����ɴϴ�. </summary>
		RefPtr<Brush> Fill_get();

		/// <summary> ����� ä��� �귯�ø� �����մϴ�. </summary>
		void Fill_set( RefPtr<Brush> value );
	};
}