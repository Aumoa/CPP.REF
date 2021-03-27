// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <DirectXMath.h>
#include "Numerics/Vector3.h"
#include "Numerics/Quaternion.h"
#include "Numerics/Matrix4x4.h"

using namespace DirectX;

#define XMLoadVector3(value) XMLoadFloat3((const XMFLOAT3*)(value))
#define XMLoadQuaternion(value) XMLoadFloat4((const XMFLOAT4*)(value))
#define XMLoadMatrix4x4(value) XMLoadFloat4x4((const XMFLOAT4X4*)(value))
#define XMLoadVector4(value) XMLoadFloat4((const XMFLOAT4*)(value))

#define XMStoreVector3(v, value) XMStoreFloat3((XMFLOAT3*)(v), (value))
#define XMStoreQuaternion(q, value) XMStoreFloat4((XMFLOAT4*)(q), (value))
#define XMStoreMatrix4x4(m, value) XMStoreFloat4x4((XMFLOAT4X4*)(m), (value))
#define XMStoreVector4(v, value) XMStoreFloat4((XMFLOAT4*)(v), (value));