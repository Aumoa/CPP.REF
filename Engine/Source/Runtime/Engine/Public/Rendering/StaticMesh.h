// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Mesh.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Misc/PrimitiveVertex.h"
#include "Graphics.h"
#include "StaticMesh.gen.h"

namespace Ayla
{
	class Buffer;

	ACLASS()
	class ENGINE_API StaticMesh : public Mesh
	{
		GENERATED_BODY()

	private:
		SharedPtr<Buffer> m_VertexBuffer;
		SharedPtr<Buffer> m_IndexBuffer;

	public:
		ACONSTRUCTOR()
		StaticMesh(SharedPtr<Graphics> graphics);
		virtual ~StaticMesh() noexcept override;

		void SetVertices(std::span<const Vector3F> vertices);
		void SetIndices(std::span<const uint16> indices);

	public:
		static Task<SharedPtr<StaticMesh>> CreateBoxAsync(SharedPtr<Graphics> graphics, Vector3F size, bool rhcoords, bool invertNormal, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateSphereAsync(SharedPtr<Graphics> graphics, float diameter, size_t tessellation, bool rhcoords, bool invertn, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateGeoSphereAsync(SharedPtr<Graphics> graphics, float diameter, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateCylinderAsync(SharedPtr<Graphics> graphics, float height, float diameter, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateConeAsync(SharedPtr<Graphics> graphics, float diameter, float height, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateTorusAsync(SharedPtr<Graphics> graphics, float diameter, float thickness, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateTetrahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateOctahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToDodecahedronken = {});
		static Task<SharedPtr<StaticMesh>> CreateDodecahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateIcosahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateTeapotAsync(SharedPtr<Graphics> graphics, float size, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});

	private:
		static Task<SharedPtr<StaticMesh>> CreatePrimitiveMeshAsync(SharedPtr<Graphics> graphics, std::function<Task<>(std::vector<PrimitiveVertex>&, std::vector<uint16>&)> generator, std::stop_token cancellationToken)
		{
			auto mesh = BeforeCreatePrimitiveMesh(graphics);
			std::vector<PrimitiveVertex> vertices;
			std::vector<uint16> indices;
			std::vector<Vector3F> positions;
			co_await generator(vertices, indices).ConfigureAwait(false);
			co_await AfterCreatePrimitiveMeshAsync(mesh, vertices, indices, cancellationToken).ConfigureAwait(false);
			mesh->SetVertices(positions);
			mesh->SetIndices(indices);
			co_return mesh;
		}

		static SharedPtr<StaticMesh> BeforeCreatePrimitiveMesh(SharedPtr<Graphics> graphics);
		static Task<> AfterCreatePrimitiveMeshAsync(SharedPtr<StaticMesh> mesh, std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices, std::stop_token cancellationToken);
	};
}