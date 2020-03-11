#pragma once

namespace SC::Game
{
	/// <summary> 렌더링 재질에 대한 버퍼의 기본 함수를 제공합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class Material : public Assets
	{
		friend class MeshRenderer;
		friend class SkinnedMeshRenderer;
		friend class Details::GameLogic;

#pragma pack( push, 4 )
		struct Constants
		{
			int Index;
			int DiffuseMap;
			int NormalMap;
			int AlphaClip;
		};

		struct Reflection
		{
			float Ambient;
			float Diffuse;
			float Specular;
			float SpecExp;
		};
#pragma pack( pop )

		static sc_game_export_object( ComPtr<ID3D12Resource> ) pReflectionBuffer;
		static Reflection* reflectionBufferPtr;
		static sc_game_export_object( RefPtr<Details::CShaderResourceView> ) pShaderResourceView;
		static int capacity;
		static int reference_count;
		static std::vector<bool> locked;
		static sc_game_export_object( RefPtr<Details::CShaderResourceView> ) pNullSRV;

		sc_game_export_object( RefPtr<Details::CDynamicBuffer> ) constantBuffer;
		int lockIndex = 0;

		Reflection frameResourceReflection;
		Constants frameResourceConstants;

		RefPtr<Texture2D> diffuseMap;
		RefPtr<Texture2D> diffuseLayerMap;
		RefPtr<Texture2D> normalMap;
		RenderQueueLayer layer = RenderQueueLayer::Default;

		void SetGraphicsRootConstantBuffers( sc_game_export_object( RefPtr<Details::CDeviceContext> )& deviceContext );
		static void SetGraphicsRootShaderResources( sc_game_export_object( RefPtr<Details::CDeviceContext> )& deviceContext );

	public:
		/// <summary> <see cref="Material"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 데이터 자산의 식별자 이름을 전달합니다. </param>
		Material( String name );
		~Material() override;

		/// <summary> (Visual Studio 전용) 표면의 주변광 반사 정도를 설정하거나 가져옵니다. </summary>
		vs_property( double, Ambient );

		/// <summary> 표면의 주변광 반사 정도를 가져옵니다. </summary>
		double Ambient_get();

		/// <summary> 표면의 주변광 반사 정도를 설정합니다. </summary>
		void Ambient_set( double value );

		/// <summary> (Visual Studio 전용) 표면의 분산광 반사 정도를 설정하거나 가져옵니다. </summary>
		vs_property( double, Diffuse );

		/// <summary> 표면의 분산광 반사 정도를 가져옵니다. </summary>
		double Diffuse_get();

		/// <summary> 표면의 분산광 반사 정도를 설정합니다. </summary>
		void Diffuse_set( double value );

		/// <summary> (Visual Studio 전용) 표면의 반영광 반사 정도를 설정하거나 가져옵니다. </summary>
		vs_property( double, Specular );

		/// <summary> 표면의 반영광 반사 정도를 가져옵니다. </summary>
		double Specular_get();

		/// <summary> 표면의 반영광 반사 정도를 설정합니다. </summary>
		void Specular_set( double value );

		/// <summary> (Visual Studio 전용) 표면의 반영광 반사 세기를 설정하거나 가져옵니다. </summary>
		vs_property( double, SpecExp );

		/// <summary> 표면의 반영광 반사 세기를 가져옵니다. </summary>
		double SpecExp_get();

		/// <summary> 표면의 반영광 반사 세기를 설정합니다. </summary>
		void SpecExp_set( double value );

		/// <summary> (Visual Studio 전용) 표면의 분산광 반사 정도를 나타내는 텍스처 개체를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Texture2D>, DiffuseMap );

		/// <summary> 표면의 분산광 반사 정도를 나타내는 텍스처 개체를 가져옵니다. </summary>
		RefPtr<Texture2D> DiffuseMap_get();

		/// <summary> 표면의 분산광 반사 정도를 나타내는 텍스처 개체를 설정합니다. </summary>
		void DiffuseMap_set( RefPtr<Texture2D> value );

		/// <summary> (Visual Studio 전용) 표면의 분산광 반사 정도의 레이어를 나타내는 텍스처 개체를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Texture2D>, DiffuseLayerMap );

		/// <summary> 표면의 분산광 반사 정도의 레이어를 나타내는 텍스처 개체를 가져옵니다. </summary>
		RefPtr<Texture2D> DiffuseLayerMap_get();

		/// <summary> 표면의 분산광 반사 정도의 레이어를 나타내는 텍스처 개체를 설정합니다. </summary>
		void DiffuseLayerMap_set( RefPtr<Texture2D> value );

		/// <summary> (Visual Studio 전용) 표면의 방향의 디테일을 나타내는 텍스처 개체를 설정하거나 가져옵니다. </summary>
		vs_property( RefPtr<Texture2D>, NormalMap );

		/// <summary> 표면의 방향의 디테일을 나타내는 텍스처 개체를 가져옵니다. </summary>
		RefPtr<Texture2D> NormalMap_get();

		/// <summary> 표면의 방향의 디테일을 나타내는 텍스처 개체를 설정합니다. </summary>
		void NormalMap_set( RefPtr<Texture2D> value );

		/// <summary> (Visual Studio 전용) 재질의 렌더 큐 계층을 설정하거나 가져옵니다. </summary>
		vs_property( RenderQueueLayer, Layer );

		/// <summary> 재질의 렌더 큐 계층을 가져옵니다. </summary>
		RenderQueueLayer Layer_get();

		/// <summary> 재질의 렌더 큐 계층을 설정합니다. </summary>
		void Layer_set( RenderQueueLayer value );

	private:
		static int Lock( Material* ptr );
		static int Realloc( int capacity );
	};
}