#pragma once

namespace SC::Game
{
	/// <summary> ī�޶��� ��� ĥ ��忡 ���� �⺻���� �����մϴ�. </summary>
	class CameraClearMode abstract : virtual public Object
	{
		friend class CameraClearModeColor;
		friend class CameraClearModeSkybox;

	private:
		CameraClearMode()
		{

		}

	public:
	};
}