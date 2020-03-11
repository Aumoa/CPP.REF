#pragma once

namespace SC::Game
{
	/// <summary> 장면에 평행 조명 효과를 추가합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class DirectionalLight final : public Light
	{
		sc_game_export_object( ComPtr<ID3D12Resource> ) pShadowDepthMap;
		sc_game_export_object( ComPtr<ID3D12DescriptorHeap> ) pDSVHeap;

		double width = 100;
		double height = 100;
		double maxDepth = 1000.0;

	protected:
		void ReadyBuffer() override;
		void BeginDraw( RefPtr<Details::CDeviceContext>& deviceContext ) override;
		void EndDraw( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="DirectionalLight"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		DirectionalLight();
		~DirectionalLight();

		/// <summary> (<see cref="ICloneable"/>) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> 클래스에서 상속 됨.) 컴포넌트에 대한 논리 갱신을 수행합니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		/// <param name="input"> 현재 프레임에서 입력 상태를 전달합니다. </param>
		void Update( Time& time, Input& input ) override;

		/// <summary> (Visual Studio 전용) 평행 조명 그림자에서 최대 렌더링 가능한 깊이를 설정하거나 가져옵니다. </summary>
		vs_property( double, MaxDepth );

		/// <summary> 평행 조명 그림자에서 최대 렌더링 가능한 깊이를 가져옵니다. </summary>
		double MaxDepth_get();

		/// <summary> 평행 조명 그림자에서 최대 렌더링 가능한 깊이를 설정합니다. </summary>
		void MaxDepth_set( double value );

		/// <summary> (Visual Studio 전용) 평행 조명 그림자에서 그림자 렌더링을 위한 카메라의 넓이를 설정하거나 가져옵니다. </summary>
		vs_property( double, Width );

		/// <summary> 평행 조명 그림자에서 그림자 렌더링을 위한 카메라의 넓이를 설정하거나 가져옵니다. </summary>
		double Width_get();

		/// <summary> 평행 조명 그림자에서 그림자 렌더링을 위한 카메라의 넓이를 설정하거나 가져옵니다. </summary>
		void Width_set( double value );

		/// <summary> (Visual Studio 전용) 평행 조명 그림자에서 그림자 렌더링을 위한 카메라의 높이를 설정하거나 가져옵니다. </summary>
		vs_property( double, Height );

		/// <summary> 평행 조명 그림자에서 그림자 렌더링을 위한 카메라의 높이를 설정하거나 가져옵니다. </summary>
		double Height_get();

		/// <summary> 평행 조명 그림자에서 그림자 렌더링을 위한 카메라의 높이를 설정하거나 가져옵니다. </summary>
		void Height_set( double value );
	};
}