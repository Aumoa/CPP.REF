#pragma once

namespace SC::Game
{
	/// <summary> ������ ������� ����� �� �ִ� 2���� �ؽ�ó �����͸� ǥ���մϴ�. </summary>
	class RenderTexture2D : virtual public Object
	{
	protected:
		RenderTexture2D();

	public:
		/// <summary> (Visual Studio ����) �ؽ�ó ������ �ȼ� �ʺ� �����ɴϴ�. </summary>
		vs_property_get( uint32, Width );

		/// <summary> �ؽ�ó ������ �ȼ� �ʺ� �����ɴϴ�. </summary>
		virtual uint32 Width_get() = 0;

		/// <summary> (Visual Studio ����) �ؽ�ó ������ �ȼ� ���̸� �����ɴϴ�. </summary>
		vs_property_get( uint32, Height );

		/// <summary> �ؽ�ó ������ �ȼ� ���̸� �����ɴϴ�. </summary>c
		virtual uint32 Height_get() = 0;
	};
}