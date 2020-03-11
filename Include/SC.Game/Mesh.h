#pragma once

namespace SC::Game
{
	/// <summary> ���� �� ���� ���ۿ� ���� �⺻ �Լ��� �����մϴ�. </summary>
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
		/// <summary> <see cref="Mesh"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ������ �ڻ� �̸��� �����մϴ�. </param>
		/// <param name="vertexBuffer"> ���� ���۸� �����մϴ�. </param>
		/// <param name="indexBuffer"> �ε��� ���۸� �����մϴ�. </param>
		Mesh( String name, const std::vector<Vertex>& vertexBuffer, const std::vector<uint32>& indexBuffer );

		/// <summary> <see cref="Mesh"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="name"> ������ �ڻ� �̸��� �����մϴ�. </param>
		/// <param name="vertexBuffer"> ���� ���۸� �����մϴ�. </param>
		/// <param name="indexBuffer"> �ε��� ���۸� �����մϴ�. </param>
		Mesh( String name, const std::vector<SkinnedVertex>& vertexBuffer, const std::vector<uint32>& indexBuffer );

		/// <summary> (Visual Studio ����) ���� ��ü�� ��Ű�� �����͸� ����ϴ��� ��Ÿ���� ���� �����ɴϴ�. </summary>
		vs_property_get( bool, IsSkinned );

		/// <summary> ���� ��ü�� ��Ű�� �����͸� ����ϴ��� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsSkinned_get();

		/// <summary> ��� �޽� ��ü�� �����մϴ�. </summary>
		/// <param name="name"> ������ ��ü �̸��� �����մϴ�. </param>
		/// <param name="texScaleX"> �ؽ�ó ��ǥ X�� ��ʸ� �����մϴ�. </param>
		/// <param name="texScaleX"> �ؽ�ó ��ǥ Y�� ��ʸ� �����մϴ�. </param>
		/// <param name="gridCountX"> ����� ���� X�� ������ �����մϴ�. </param>
		/// <param name="gridCountY"> ����� ���� Y�� ������ �����մϴ�. </param>
		static RefPtr<Mesh> CreatePlane( String name, float texScaleX = 1.0f, float texScaleY = 1.0f, int gridCountX = 1, int gridCountY = 1 );

		/// <summary> ť�� �޽� ��ü�� �����մϴ�. </summary>
		/// <param name="name"> ������ ��ü �̸��� �����մϴ�. </param>
		static RefPtr<Mesh> CreateCube( String name );
	};
}