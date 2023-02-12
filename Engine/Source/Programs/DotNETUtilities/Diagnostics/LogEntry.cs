// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Diagnostics;

internal struct LogEntry
{
    public LogVerbosity Verbosity { get; set; }

    public string? Message { get; set; }

    public Action? LogAction { get; set; }

    public Thread LogThread { get; set; }
}
