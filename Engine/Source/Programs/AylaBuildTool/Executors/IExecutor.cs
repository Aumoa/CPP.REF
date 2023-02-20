// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Executors;

public interface IExecutor
{
    Task<int> RunAsync(CancellationToken CToken = default);
}
