#pragma once

namespace SC::Game
{
	/// <summary> ���� ������ �ڻ��� ǥ���մϴ�. </summary>
	class Assets : virtual public Object
	{
		String name;

	protected:
		/// <summary> <see cref="Assets"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> �ڻ� �������� �ĺ��� �̸��� �����մϴ�. </param>
		Assets( String name );

	public:
		/// <summary> (Visual Studio ����) �ڻ� �������� �ĺ��� �̸��� �����ɴϴ�. </summary>
		vs_property( String, Name );

		/// <summary> �ڻ� �������� �ĺ��� �̸��� �����ɴϴ�. </summary>
		String Name_get();
	};
}