#pragma once

namespace SC::Game
{
	/// <summary> 정점 및 색인 버퍼에 대한 기본 함수를 제공합니다. </summary>
	class Mesh : public Assets
	{
		friend class MeshRenderer;
		friend class SkinnedMeshRenderer;
		friend class Details::GameLogic;

		sc_game_export_object( RefPtr<Details::CBuffer> ) vertexBuffer;
		sc_game_export_object( RefPtr<Details::CBuffer> ) indexBuffer;
		int numVertex = 0;
		int numIndex = 0;
		bool isSkinned = false;

	private:
		Mesh( String name );

		void DrawIndexed( RefPtr<Details::CDeviceContext>& deviceContext );

	public:
		/// <summary> <see cref="Mesh"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 데이터 자산 이름을 전달합니다. </param>
		/// <param name="vertexBuffer"> 정점 버퍼를 전달합니다. </param>
		/// <param name="indexBuffer"> 인덱스 버퍼를 전달합니다. </param>
		Mesh( String name, const std::vector<Vertex>& vertexBuffer, const std::vector<uint32>& indexBuffer );

		/// <summary> <see cref="Mesh"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="name"> 데이터 자산 이름을 전달합니다. </param>
		/// <param name="vertexBuffer"> 정점 버퍼를 전달합니다. </param>
		/// <param name="indexBuffer"> 인덱스 버퍼를 전달합니다. </param>
		Mesh( String name, const std::vector<SkinnedVertex>& vertexBuffer, const std::vector<uint32>& indexBuffer );

		/// <summary> (Visual Studio 전용) 현재 개체가 스키닝 데이터를 사용하는지 나타내는 값을 가져옵니다. </summary>
		vs_property_get( bool, IsSkinned );

		/// <summary> 현재 개체가 스키닝 데이터를 사용하는지 나타내는 값을 가져옵니다. </summary>
		bool IsSkinned_get();

		/// <summary> 평면 메쉬 개체를 생성합니다. </summary>
		/// <param name="name"> 데이터 개체 이름을 제공합니다. </param>
		/// <param name="texScaleX"> 텍스처 좌표 X축 비례를 전달합니다. </param>
		/// <param name="texScaleX"> 텍스처 좌표 Y축 비례를 전달합니다. </param>
		/// <param name="gridCountX"> 평면의 격자 X축 개수를 전달합니다. </param>
		/// <param name="gridCountY"> 평면의 격자 Y축 개수를 전달합니다. </param>
		static RefPtr<Mesh> CreatePlane( String name, float texScaleX = 1.0f, float texScaleY = 1.0f, int gridCountX = 1, int gridCountY = 1 );

		/// <summary> 큐브 메쉬 개체를 생성합니다. </summary>
		/// <param name="name"> 데이터 개체 이름을 제공합니다. </param>
		static RefPtr<Mesh> CreateCube( String name );
	};
}