#pragma once

namespace SC::Game
{
	/// <summary> ī�޶��� ��� ĥ ��带 ���� ���� ���� �����մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class CameraClearModeColor final : public CameraClearMode
	{
		Drawing::Color clearColor;

	public:
		/// <summary> <see cref="CameraClearModeColor"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		CameraClearModeColor( Drawing::Color clearColor = Drawing::Color( 0, 0, 0, 0 ) )
			: clearColor( clearColor )
		{

		}

		/// <summary> (Visual Studio ����) ������ ���� ������ �����ɴϴ�. </summary>
		vs_property_get( Drawing::Color, ClearColor );

		/// <summary> ������ ���� ������ �����ɴϴ�. </summary>
		Drawing::Color ClearColor_get()
		{
			return clearColor;
		}
	};
}