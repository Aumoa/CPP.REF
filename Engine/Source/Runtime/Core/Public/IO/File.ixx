// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:File;

import Core.Std;
import Core.System;
import Core.Threading;

export class CORE_API File : public StaticClass
{
public:
	static Task<String> ReadAllTextAsync(String InPath, std::stop_token InCancellationToken = {});
	static bool Exists(String InPath);
};