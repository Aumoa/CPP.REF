// Copyright 2020-2024 Aumoa.lib. All right reserved.

namespace AylaEngine;

public interface IExecutor
{
    Task<int> RunAsync(CancellationToken CToken = default);
}
