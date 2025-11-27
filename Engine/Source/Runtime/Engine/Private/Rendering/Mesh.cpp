// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/Mesh.h"
#include "Graphics.h"

namespace Ayla
{
	Mesh::Mesh(SharedPtr<Graphics> graphics) : Super(std::move(graphics))
	{
	}

	Mesh::~Mesh() noexcept
	{
	}
}