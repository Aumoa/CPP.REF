// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SourceCode
{
private:
	FileReference file;
	Task<String> cachedTask;
	Spinlock locker;

protected:
	SourceCode(FileReference file) noexcept;

public:
	virtual ~SourceCode() noexcept = default;

	virtual Task<> CompileAsync(CancellationToken cancellationToken = {}) = 0;

	FileReference GetSourceFile() const;
	Task<String> ReadContentAsync(CancellationToken cancellationToken = {});
};