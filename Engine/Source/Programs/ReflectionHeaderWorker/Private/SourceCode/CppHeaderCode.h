// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SourceCode/SourceCode.h"

class CppHeaderCode : public SourceCode
{
public:
	using Super = SourceCode;

public:
	CppHeaderCode(FileReference file) noexcept;

	virtual Task<> CompileAsync(CancellationToken cancellationToken = {}) override;

public:
	static bool TryGenerateCode(const FileReference& file, std::unique_ptr<SourceCode>* generatedCode);
};