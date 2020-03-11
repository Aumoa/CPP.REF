#pragma once

namespace SC::Game
{
	/// <summary> 장면에 조명 효과를 추가합니다. 이 클래스는 상속될 수 없습니다. </summary>
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

		/// <summary> (Visual Studio 전용) 조명의 색을 설정하거나 가져옵니다. </summary>
		vs_property( Drawing::Color, LightColor );

		/// <summary> 조명의 색을 가져옵니다. </summary>
		Drawing::Color LightColor_get();

		/// <summary> 조명의 색을 설정합니다. </summary>
		void LightColor_set( Drawing::Color value );

		/// <summary> (Visual Studio 전용) 조명의 주변광 값을 설정하거나 가져옵니다. </summary>
		vs_property( double, Ambient );

		/// <summary> 조명의 주변광 값을 가져옵니다. </summary>
		double Ambient_get();

		/// <summary> 조명의 주변광 값을 설정합니다. </summary>
		void Ambient_set( double value );

		/// <summary> (Visual Studio 전용) 조명의 분산광 값을 설정하거나 가져옵니다. </summary>
		vs_property( double, Diffuse );

		/// <summary> 조명의 분산광 값을 가져옵니다. </summary>
		double Diffuse_get();

		/// <summary> 조명의 분산광 값을 설정합니다. </summary>
		void Diffuse_set( double value );

		/// <summary> (Visual Studio 전용) 조명의 반영광 값을 설정하거나 가져옵니다. </summary>
		vs_property( double, Specular );

		/// <summary> 조명의 반영광 값을 가져옵니다. </summary>
		double Specular_get();

		/// <summary> 조명의 반영광 값을 설정합니다. </summary>
		void Specular_set( double value );

		/// <summary> (Visual Studio 전용) 조명이 그림자를 생성하는지 나타내는 값을 설정하거나 가져옵니다. </summary>
		vs_property( bool, IsShadowCast );

		/// <summary> 조명이 그림자를 생성하는지 나타내는 값을 가져옵니다. </summary>
		bool IsShadowCast_get();

		/// <summary> 조명이 그림자를 생성하는지 나타내는 값을 설정합니다. </summary>
		void IsShadowCast_set( bool value );
	};
}