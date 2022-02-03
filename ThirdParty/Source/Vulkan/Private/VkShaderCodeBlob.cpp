// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkShaderCodeBlob.h"

GENERATE_BODY(SVkShaderCodeBlob);

SVkShaderCodeBlob::SVkShaderCodeBlob(SVkDevice* Owner, std::vector<uint8> Binary)
	: Super(Owner)
	, Binary(std::move(Binary))
{
}

const void* SVkShaderCodeBlob::GetBufferPointer()
{
	return Binary.data();
}

size_t SVkShaderCodeBlob::GetBufferSize()
{
	return Binary.size();
}

void SVkShaderCodeBlob::Dispose(bool bDisposing)
{
	Binary.clear();
}