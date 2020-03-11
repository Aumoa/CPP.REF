#pragma once

namespace SC::Game
{
	/// <summary> 카메라 구성 요소에 대한 함수를 제공합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class Camera final : public Component
	{
		friend class Details::GameLogic;

#pragma pack( push, 4 )
		struct Constants
		{
			sc_game_export_float( XMFLOAT4X4, 16 ) ViewProj;
			sc_game_export_float( XMFLOAT4X4, 16 ) ViewInv;
			sc_game_export_float( XMFLOAT4X4, 16 ) ProjInv;
			sc_game_export_float( XMFLOAT3, 3 ) Pos;
		};
#pragma pack( pop )

		std::array<sc_game_export_object( RefPtr<Details::CDynamicBuffer> ), 2> dynamicBuffer;
		RefPtr<CameraClearMode> clearMode = new CameraClearModeColor();

		double aspectRatio = 0;

	private:
		void SetGraphicsRootConstantBufferView( sc_game_export_object( RefPtr<Details::CDeviceContext> )& deviceContext );

	public:
		/// <summary> <see cref="Camera"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Camera();
		~Camera() override;

		/// <summary> (<see cref="ICloneable"/>) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> 클래스에서 상속 됨.) 컴포넌트에 대한 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		/// <param name="input"> 현재 프레임에서 입력 상태를 전달합니다. </param>
		void LateUpdate( Time& time, Input& input ) override;

		/// <summary> 화면 공간 위치로부터 세계 공간으로 투영되는 광선 정보를 얻습니다. </summary>
		/// <param name="screenSpace"> 화면 공간 위치를 전달합니다. [-1, 1]로 정규화된 위치입니다. </param>
		Ray ScreenSpaceToRay( Drawing::Point<double> screenSpace );

		/// <summary> (Visual Studio 전용) 카메라가 렌더링하는 장면의 배경 칠 모드를 설정하거나 가져옵니다. nullptr일 경우 배경을 칠하지 않습니다. </summary>
		vs_property( RefPtr<CameraClearMode>, ClearMode );

		/// <summary> 카메라가 렌더링하는 장면의 배경 칠 모드를 가져옵니다. nullptr일 경우 배경을 칠하지 않습니다. </summary>
		RefPtr<CameraClearMode> ClearMode_get();

		/// <summary> 카메라가 렌더링하는 장면의 배경 칠 모드를 설정합니다. nullptr일 경우 배경을 칠하지 않습니다. </summary>
		void ClearMode_set( RefPtr<CameraClearMode> value );

		/// <summary> (Visual Studio 전용) 카메라의 종횡비를 가져오거나 설정합니다. 0일 경우 렌더링 대상 크기에 따라 자동 설정됩니다. </summary>
		vs_property( double, AspectRatio );

		/// <summary> 카메라의 종횡비를 가져옵니다. 0일 경우 렌더링 대상 크기에 따라 자동 설정됩니다. </summary>
		double AspectRatio_get();

		/// <summary> 카메라의 종횡비를 설정합니다. 0일 경우 렌더링 대상 크기에 따라 자동 설정됩니다. </summary>
		void AspectRatio_set( double value );

		/// <summary> (Visual Studio 전용) 카메라의 렌더링 대상을 설정하거나 가져옵니다. nullptr일 경우 주 화면을 대상으로 렌더링을 진행합니다. </summary>
		//vs_property( RefPtr<RenderTexture2D>, RenderTarget );

		/// <summary> 카메라의 렌더링 대상을 가져옵니다. nullptr일 경우 주 화면을 대상으로 렌더링을 진행합니다. </summary>
		//RefPtr<RenderTexture2D> RenderTarget_get();

		/// <summary> 카메라의 렌더링 대상을 설정합니다. nullptr일 경우 주 화면을 대상으로 렌더링을 진행합니다. </summary>
		//void RenderTarget_set( RefPtr<RenderTexture2D> value );
	};
}