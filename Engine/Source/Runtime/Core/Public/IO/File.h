// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/String.h"
#include "Threading/Tasks/Task.h"

class CORE_API File
{
public:
	static bool Exists(String path) noexcept;
	static inline bool TryMove(String src, String dst, bool overwrite = false) noexcept;
	static inline bool TryDelete(String src) noexcept;
	static Task<String> ReadAllTextAsync(String filename, std::stop_token cancellationToken = {});
	static Task<> WriteAllTextAsync(String InFilename, String Text, uint32 Encoding = 0, std::stop_token SToken = {});
};