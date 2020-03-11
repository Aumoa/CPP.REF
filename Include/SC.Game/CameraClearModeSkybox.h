#pragma once

namespace SC::Game
{
	/// <summary> ī�޶��� ��� ĥ ��带 ��ī�̹ڽ� ���� �����մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class CameraClearModeSkybox final : public CameraClearMode
	{
		RefPtr<Texture2D> skybox;

	public:
		/// <summary> <see cref="CameraClearModeSkybox"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="skyboxTexture"> ��ī�̹ڽ� �������� ���� �ؽ�ó ��ü�� �����մϴ�. </param>
		CameraClearModeSkybox( RefPtr<Texture2D> skyboxTexture )
			: skybox( skyboxTexture )
		{

		}

		/// <summary> (Visual Studio ����) ������ ��ī�̹ڽ� �ؽ�ó�� �����ɴϴ�. </summary>
		vs_property_get( RefPtr<Texture2D>, SkyboxTexture );

		/// <summary> ������ ��ī�̹ڽ� �ؽ�ó�� �����ɴϴ�. </summary>
		RefPtr<Texture2D> SkyboxTexture_get()
		{
			return skybox;
		}
	};
}