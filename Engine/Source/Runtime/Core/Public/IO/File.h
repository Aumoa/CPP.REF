// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include "Threading/Tasks/Task.h"

class File
{
public:
	static bool Exists(String path) noexcept;
	static inline bool TryMove(String src, String dst, bool overwrite = false) noexcept;
	static inline bool TryDelete(String src) noexcept;
	static Task<String> ReadAllTextAsync(String filename, std::stop_token cancellationToken = {});
};