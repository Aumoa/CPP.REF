// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/StaticMesh.h"
#include "Graphics.h"
#include "Buffer.h"

namespace Ayla
{
	StaticMesh::StaticMesh(SharedPtr<Graphics> graphics)
		: Super(std::move(graphics))
		, m_VertexBuffer(GetGraphics()->CreateBuffer(BufferUsage::VertexBuffer))
		, m_IndexBuffer(GetGraphics()->CreateBuffer(BufferUsage::IndexBuffer))
	{
	}

	StaticMesh::~StaticMesh() noexcept
	{
	}

	void StaticMesh::SetVertices(std::span<const Vector3D> vertices)
	{
		// TODO: Convert to proper vertex format
		std::vector<Vector3F> cv_vertices;
		cv_vertices.reserve(vertices.size());
		for (const auto& v : vertices)
		{
			cv_vertices.emplace_back(Vector<>::Cast<float>(v));
		}

		m_VertexBuffer->UpdateData(std::span<const byte>(reinterpret_cast<const byte*>(cv_vertices.data()), cv_vertices.size() * sizeof(Vector3F)));
	}

	void StaticMesh::SetIndices(std::span<const int32> indices)
	{
		// TODO: Support 16-bit indices
		std::vector<uint16> cv_indices;
		cv_indices.reserve(indices.size());
		for (const auto& i : indices)
		{
			cv_indices.emplace_back(static_cast<uint16>(i));
		}

		m_IndexBuffer->UpdateData(std::span<const byte>(reinterpret_cast<const byte*>(cv_indices.data()), cv_indices.size() * sizeof(uint16)));
	}
}