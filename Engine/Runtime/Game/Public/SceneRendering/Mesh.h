// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectX/DirectXMinimal.h"

class Engine;
class DirectXDeviceBundle;
struct Vertex;

class GAME_API Mesh : virtual public Object
{
public:
	using Super = Object;

private:
	Engine* engine;
	DirectXDeviceBundle* deviceBundle;

public:
	Mesh(Engine* engine);
	~Mesh() override;

protected:
	TComPtr<ID3D12Resource> CreateVertexBuffer(std::span<Vertex const> inVertices);
	TComPtr<ID3D12Resource> CreateIndexBuffer(std::span<uint32 const> inVertices);
};