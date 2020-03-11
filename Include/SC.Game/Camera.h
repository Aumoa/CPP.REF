#pragma once

namespace SC::Game
{
	/// <summary> ī�޶� ���� ��ҿ� ���� �Լ��� �����մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
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
		/// <summary> <see cref="Camera"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Camera();
		~Camera() override;

		/// <summary> (<see cref="ICloneable"/>) ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> Ŭ�������� ��� ��.) ������Ʈ�� ���� �� ������ �����մϴ�. </summary>
		/// <param name="time"> ���� �ð��� �����մϴ�. </param>
		/// <param name="input"> ���� �����ӿ��� �Է� ���¸� �����մϴ�. </param>
		void LateUpdate( Time& time, Input& input ) override;

		/// <summary> ȭ�� ���� ��ġ�κ��� ���� �������� �����Ǵ� ���� ������ ����ϴ�. </summary>
		/// <param name="screenSpace"> ȭ�� ���� ��ġ�� �����մϴ�. [-1, 1]�� ����ȭ�� ��ġ�Դϴ�. </param>
		Ray ScreenSpaceToRay( Drawing::Point<double> screenSpace );

		/// <summary> (Visual Studio ����) ī�޶� �������ϴ� ����� ��� ĥ ��带 �����ϰų� �����ɴϴ�. nullptr�� ��� ����� ĥ���� �ʽ��ϴ�. </summary>
		vs_property( RefPtr<CameraClearMode>, ClearMode );

		/// <summary> ī�޶� �������ϴ� ����� ��� ĥ ��带 �����ɴϴ�. nullptr�� ��� ����� ĥ���� �ʽ��ϴ�. </summary>
		RefPtr<CameraClearMode> ClearMode_get();

		/// <summary> ī�޶� �������ϴ� ����� ��� ĥ ��带 �����մϴ�. nullptr�� ��� ����� ĥ���� �ʽ��ϴ�. </summary>
		void ClearMode_set( RefPtr<CameraClearMode> value );

		/// <summary> (Visual Studio ����) ī�޶��� ��Ⱦ�� �������ų� �����մϴ�. 0�� ��� ������ ��� ũ�⿡ ���� �ڵ� �����˴ϴ�. </summary>
		vs_property( double, AspectRatio );

		/// <summary> ī�޶��� ��Ⱦ�� �����ɴϴ�. 0�� ��� ������ ��� ũ�⿡ ���� �ڵ� �����˴ϴ�. </summary>
		double AspectRatio_get();

		/// <summary> ī�޶��� ��Ⱦ�� �����մϴ�. 0�� ��� ������ ��� ũ�⿡ ���� �ڵ� �����˴ϴ�. </summary>
		void AspectRatio_set( double value );

		/// <summary> (Visual Studio ����) ī�޶��� ������ ����� �����ϰų� �����ɴϴ�. nullptr�� ��� �� ȭ���� ������� �������� �����մϴ�. </summary>
		//vs_property( RefPtr<RenderTexture2D>, RenderTarget );

		/// <summary> ī�޶��� ������ ����� �����ɴϴ�. nullptr�� ��� �� ȭ���� ������� �������� �����մϴ�. </summary>
		//RefPtr<RenderTexture2D> RenderTarget_get();

		/// <summary> ī�޶��� ������ ����� �����մϴ�. nullptr�� ��� �� ȭ���� ������� �������� �����մϴ�. </summary>
		//void RenderTarget_set( RefPtr<RenderTexture2D> value );
	};
}