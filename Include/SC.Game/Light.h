#pragma once

namespace SC::Game
{
	/// <summary> ��鿡 ���� ȿ���� �߰��մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class Light : public Component
	{
		friend class Details::GameLogic;
		friend class DirectionalLight;

	protected:
#pragma pack( push, 4 )
		struct alignas( 256 ) LightConstants
		{
			uint32 Type;
			sc_game_export_float( XMFLOAT3, 3 ) Color;
			float Ambient;
			float Diffuse;
			float Specular;
			int ShadowCast;
			sc_game_export_float( XMFLOAT4X4, 16 ) ViewProj;
			union
			{
				struct DirectionalLightConstants
				{
					sc_game_export_float( XMFLOAT4, 4 ) Direction;
				} DirectionalLight;
			};
		};
#pragma pack( pop )

	private:
		Drawing::Color lightColor;

		std::array<sc_game_export_object( RefPtr<Details::CDynamicBuffer> ), 2> lightBuffer;

		bool shadowCast = false;
		sc_game_export_object( RefPtr<Details::CShaderResourceView> ) pShaderResourceView;

	protected:
		LightConstants frameResource;

	private:
		Light();

		void SetGraphicsRootShaderResources( RefPtr<Details::CDeviceContext>& deviceContext );

	protected:
		void SetDepthBuffer( void* pResource );
		virtual void ReadyBuffer() = 0;
		virtual void BeginDraw( RefPtr<Details::CDeviceContext>& deviceContext ) = 0;
		virtual void EndDraw( RefPtr<Details::CDeviceContext>& deviceContext ) = 0;

	public:
		~Light() override;

		/// <summary> (Visual Studio ����) ������ ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( Drawing::Color, LightColor );

		/// <summary> ������ ���� �����ɴϴ�. </summary>
		Drawing::Color LightColor_get();

		/// <summary> ������ ���� �����մϴ�. </summary>
		void LightColor_set( Drawing::Color value );

		/// <summary> (Visual Studio ����) ������ �ֺ��� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Ambient );

		/// <summary> ������ �ֺ��� ���� �����ɴϴ�. </summary>
		double Ambient_get();

		/// <summary> ������ �ֺ��� ���� �����մϴ�. </summary>
		void Ambient_set( double value );

		/// <summary> (Visual Studio ����) ������ �л걤 ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Diffuse );

		/// <summary> ������ �л걤 ���� �����ɴϴ�. </summary>
		double Diffuse_get();

		/// <summary> ������ �л걤 ���� �����մϴ�. </summary>
		void Diffuse_set( double value );

		/// <summary> (Visual Studio ����) ������ �ݿ��� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( double, Specular );

		/// <summary> ������ �ݿ��� ���� �����ɴϴ�. </summary>
		double Specular_get();

		/// <summary> ������ �ݿ��� ���� �����մϴ�. </summary>
		void Specular_set( double value );

		/// <summary> (Visual Studio ����) ������ �׸��ڸ� �����ϴ��� ��Ÿ���� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( bool, IsShadowCast );

		/// <summary> ������ �׸��ڸ� �����ϴ��� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsShadowCast_get();

		/// <summary> ������ �׸��ڸ� �����ϴ��� ��Ÿ���� ���� �����մϴ�. </summary>
		void IsShadowCast_set( bool value );
	};
}