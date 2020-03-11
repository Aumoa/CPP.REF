#pragma once

namespace SC::Game
{
	/// <summary> ��鿡 ���� ���� ȿ���� �߰��մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
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
		/// <summary> <see cref="DirectionalLight"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		DirectionalLight();
		~DirectionalLight();

		/// <summary> (<see cref="ICloneable"/>) ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> Ŭ�������� ��� ��.) ������Ʈ�� ���� �� ������ �����մϴ�. </summary>
		/// <param name="time"> ���� �ð��� �����մϴ�. </param>
		/// <param name="input"> ���� �����ӿ��� �Է� ���¸� �����մϴ�. </param>
		void Update( Time& time, Input& input ) override;

		/// <summary> (Visual Studio ����) ���� ���� �׸��ڿ��� �ִ� ������ ������ ���̸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, MaxDepth );

		/// <summary> ���� ���� �׸��ڿ��� �ִ� ������ ������ ���̸� �����ɴϴ�. </summary>
		double MaxDepth_get();

		/// <summary> ���� ���� �׸��ڿ��� �ִ� ������ ������ ���̸� �����մϴ�. </summary>
		void MaxDepth_set( double value );

		/// <summary> (Visual Studio ����) ���� ���� �׸��ڿ��� �׸��� �������� ���� ī�޶��� ���̸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Width );

		/// <summary> ���� ���� �׸��ڿ��� �׸��� �������� ���� ī�޶��� ���̸� �����ϰų� �����ɴϴ�. </summary>
		double Width_get();

		/// <summary> ���� ���� �׸��ڿ��� �׸��� �������� ���� ī�޶��� ���̸� �����ϰų� �����ɴϴ�. </summary>
		void Width_set( double value );

		/// <summary> (Visual Studio ����) ���� ���� �׸��ڿ��� �׸��� �������� ���� ī�޶��� ���̸� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Height );

		/// <summary> ���� ���� �׸��ڿ��� �׸��� �������� ���� ī�޶��� ���̸� �����ϰų� �����ɴϴ�. </summary>
		double Height_get();

		/// <summary> ���� ���� �׸��ڿ��� �׸��� �������� ���� ī�޶��� ���̸� �����ϰų� �����ɴϴ�. </summary>
		void Height_set( double value );
	};
}