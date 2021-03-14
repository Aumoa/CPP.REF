// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class DirectXRaytracingShader;
struct DirectXShaderRecordInfo;

class GAME_API DirectXShaderRecord : virtual public Object
{
public:
	using Super = Object;

public:
	DirectXShaderRecord(const DirectXShaderRecordInfo& sInfo);
	~DirectXShaderRecord() override;

	size_t GetSizeInBytes() const;
	void SetParameter(uint32 index, uint64 handle);

	void Update(char* destination);

private:
	size_t CalcSizeInBytes(size_t paramCount) const;
	static size_t RoundUp(size_t actual, size_t powerOf2Alignment);

private:
	const void* shaderIdentifier;
	size_t sizeInBytes;
	std::vector<uint64> params;
};

class GAME_API DirectXShaderBindingTable : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	DirectXDeviceBundle* deviceBundle;

	DirectXRaytracingShader* shader;
	TRefPtr<DirectXShaderRecord> rayGen;
	std::vector<TRefPtr<DirectXShaderRecord>> closestHit;
	std::vector<TRefPtr<DirectXShaderRecord>> miss;

	TComPtr<ID3D12Resource> rayGenBuf;
	uint64 rayGenLen;
	char* rayGenBufPtr;
	TComPtr<ID3D12Resource> closestHitBuf;
	uint64 closestHitStride;
	uint64 closestHitLen;
	char* closestHitBufPtr;
	TComPtr<ID3D12Resource> missBuf;
	uint64 missStride;
	uint64 missLen;
	char* missBufPtr;

public:
	DirectXShaderBindingTable(DirectXDeviceBundle* deviceBundle);
	~DirectXShaderBindingTable() override;

	void Init(DirectXRaytracingShader* initShader);
	void Update();
	void FillDispatchRaysDesc(D3D12_DISPATCH_RAYS_DESC& outDispatchRays) const;

private:
	void ReadyBuffers();
	void ReadyBufferSingle(const std::vector<TRefPtr<DirectXShaderRecord>>& records, TComPtr<ID3D12Resource>& buf, uint64& length, uint64& stride, char*& bufPtr);
};