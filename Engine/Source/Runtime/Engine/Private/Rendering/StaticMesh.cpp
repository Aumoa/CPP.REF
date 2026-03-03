// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/StaticMesh.h"
#include "Misc/Geometry.h"
#include "Misc/PrimitiveVertex.h"
#include "Graphics.h"
#include "Buffer.h"

namespace Ayla
{
	StaticMesh::StaticMesh(SharedPtr<Graphics> graphics)
		: Super(std::move(graphics))
	{
	}

	StaticMesh::~StaticMesh() noexcept
	{
	}

	void StaticMesh::SetVertices(std::span<const Vector3F> vertices)
	{
		// TODO: Convert to proper vertex format
		//m_VertexBuffer->UpdateData(std::span<const byte>(reinterpret_cast<const byte*>(vertices.data()), vertices.size() * sizeof(Vector3F)));
	}

	void StaticMesh::SetIndices(std::span<const uint16> indices)
	{
		// TODO: Support 32-bit indices
		//m_IndexBuffer->UpdateData(std::span<const byte>(reinterpret_cast<const byte*>(indices.data()), indices.size() * sizeof(uint16)));
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateBoxAsync(SharedPtr<Graphics> graphics, Vector3F size, bool rhcoords, bool invertNormal, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeBox(vertices, indices, size, rhcoords, invertNormal);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateSphereAsync(SharedPtr<Graphics> graphics, float diameter, size_t tessellation, bool rhcoords, bool invertn, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeSphere(vertices, indices, diameter, tessellation, rhcoords, invertn);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateGeoSphereAsync(SharedPtr<Graphics> graphics, float diameter, size_t tessellation, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeGeoSphere(vertices, indices, diameter, tessellation, rhcoords);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateCylinderAsync(SharedPtr<Graphics> graphics, float height, float diameter, size_t tessellation, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeCylinder(vertices, indices, height, diameter, tessellation, rhcoords);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateConeAsync(SharedPtr<Graphics> graphics, float diameter, float height, size_t tessellation, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeCone(vertices, indices, diameter, height, tessellation, rhcoords);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateTorusAsync(SharedPtr<Graphics> graphics, float diameter, float thickness, size_t tessellation, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeTorus(vertices, indices, diameter, thickness, tessellation, rhcoords);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateTetrahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeTetrahedron(vertices, indices, size, rhcoords);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateOctahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeOctahedron(vertices, indices, size, rhcoords);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateDodecahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeDodecahedron(vertices, indices, size, rhcoords);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateIcosahedronAsync(SharedPtr<Graphics> graphics, float size, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeIcosahedron(vertices, indices, size, rhcoords);
			});
		}, cancellationToken);
	}

	Task<SharedPtr<StaticMesh>> StaticMesh::CreateTeapotAsync(SharedPtr<Graphics> graphics, float size, size_t tessellation, bool rhcoords, std::stop_token cancellationToken)
	{
		co_return co_await CreatePrimitiveMeshAsync(graphics, [&](std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices) -> Task<>
		{
			return Task<>::Run([&]()
			{
				Geometry::ComputeTeapot(vertices, indices, size, tessellation, rhcoords);
			});
		}, cancellationToken);
	}

	SharedPtr<StaticMesh> StaticMesh::BeforeCreatePrimitiveMesh(SharedPtr<Graphics> graphics)
	{
		return New<StaticMesh>(graphics);
	}

	Task<> StaticMesh::AfterCreatePrimitiveMeshAsync(SharedPtr<StaticMesh> mesh, std::vector<PrimitiveVertex>& vertices, std::vector<uint16>& indices, std::stop_token cancellationToken)
	{
		std::vector<Vector3F> positions;
		co_await Task<>::Run([&]()
		{
			positions.reserve(vertices.size());
			for (const auto& v : vertices)
			{
				positions.emplace_back(v.Position);
			}
		});
		mesh->SetVertices(positions);
		mesh->SetIndices(indices);
		co_return;
	}
}