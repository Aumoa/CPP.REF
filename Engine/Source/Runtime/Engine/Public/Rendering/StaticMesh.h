// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Mesh.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Misc/PrimitiveVertex.h"
#include "Misc/VertexTypes.h"
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

		/// <summary>
		/// Sets vertices from a typed span. Any vertex struct is accepted.
		/// The Graphics backend will create a GPU-resident vertex buffer from this data.
		/// </summary>
		template<typename TVertex>
		void SetVertices(std::span<const TVertex> vertices)
		{
			SetVerticesRaw(
				std::span<const byte>(reinterpret_cast<const byte*>(vertices.data()), vertices.size() * sizeof(TVertex)),
				sizeof(TVertex)
			);
		}

		/// <summary>
		/// Sets 16-bit indices for this mesh.
		/// </summary>
		void SetIndices(std::span<const uint16> indices);

		/// <summary>
		/// Sets 32-bit indices for this mesh.
		/// </summary>
		void SetIndices(std::span<const uint32> indices);

		SharedPtr<Buffer> GetVertexBuffer() const noexcept;
		SharedPtr<Buffer> GetIndexBuffer() const noexcept;
		size_t GetVertexCount() const noexcept;
		size_t GetIndexCount() const noexcept;

	private:
		void SetVerticesRaw(std::span<const byte> data, size_t stride);

	public:
		static Task<SharedPtr<StaticMesh>> CreateBoxAsync(SharedPtr<Graphics> graphics, Vector3F size, bool rhcoords, bool invertNormal, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateSphereAsync(SharedPtr<Graphics> graphics, float diameter, size_t tessellation, bool rhcoords, bool invertn, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateGeoSphereAsync(SharedPtr<Graphics> graphics, float diameter, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateCylinderAsync(SharedPtr<Graphics> graphics, float height, float diameter, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateConeAsync(SharedPtr<Graphics> graphics, float diameter, float height, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateTorusAsync(SharedPtr<Graphics> graphics, float diameter, float thickness, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateTetrahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateOctahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateDodecahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateIcosahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken = {});
		static Task<SharedPtr<StaticMesh>> CreateTeapotAsync(SharedPtr<Graphics> graphics, float size, size_t tessellation, bool rhcoords, std::stop_token cancellationToken = {});

	private:
		static Task<SharedPtr<StaticMesh>> CreatePrimitiveMeshAsync(SharedPtr<Graphics> graphics, std::function<Task<>(std::vector<PrimitiveVertex>&, std::vector<uint16>&)> generator, std::stop_token cancellationToken)
		{
			auto mesh = BeforeCreatePrimitiveMesh(graphics);
			std::vector<PrimitiveVertex> vertices;
			std::vector<uint16> indices;
			co_await generator(vertices, indices).ConfigureAwait(false);
			co_await AfterCreatePrimitiveMeshAsync(mesh, vertices, indices, cancellationToken).ConfigureAwait(false);
			co_return mesh;
		}

		static SharedPtr<StaticMesh> BeforeCreatePrimitiveMesh(SharedPtr<Graphics> graphics);
		static Task<> AfterCreatePrimitiveMeshAsync(SharedPtr<StaticMesh> mesh, std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices, std::stop_token cancellationToken);
	};
}
