#pragma once

namespace SC::Game
{
	/// <summary> 카메라의 배경 칠 모드를 스카이박스 모드로 설정합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class CameraClearModeSkybox final : public CameraClearMode
	{
		RefPtr<Texture2D> skybox;

	public:
		/// <summary> <see cref="CameraClearModeSkybox"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="skyboxTexture"> 스카이박스 렌더링을 위한 텍스처 개체를 전달합니다. </param>
		CameraClearModeSkybox( RefPtr<Texture2D> skyboxTexture )
			: skybox( skyboxTexture )
		{

		}

		/// <summary> (Visual Studio 전용) 설정된 스카이박스 텍스처를 가져옵니다. </summary>
		vs_property_get( RefPtr<Texture2D>, SkyboxTexture );

		/// <summary> 설정된 스카이박스 텍스처를 가져옵니다. </summary>
		RefPtr<Texture2D> SkyboxTexture_get()
		{
			return skybox;
		}
	};
}