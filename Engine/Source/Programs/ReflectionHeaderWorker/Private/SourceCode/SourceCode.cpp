// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "SourceCode/SourceCode.h"

SourceCode::SourceCode(FileReference file) noexcept
	: file(file)
{
}

FileReference SourceCode::GetSourceFile() const
{
	return file;
}

Task<String> SourceCode::ReadContentAsync(CancellationToken cancellationToken)
{
	if (cachedTask.IsCompleted())
	{
		return cachedTask;
	}

	std::unique_lock lock(locker);
	if (cachedTask.IsCompleted())
	{
		return cachedTask;
	}
	else if (cachedTask.IsValid())
	{
		return cachedTask.WaitAsync(cancellationToken);
	}

	cachedTask = file.ReadAllTextAsync(cancellationToken);
	return cachedTask;
}