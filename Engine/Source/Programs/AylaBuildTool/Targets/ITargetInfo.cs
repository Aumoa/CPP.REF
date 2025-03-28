namespace AylaEngine;

public interface ITargetInfo
{
    PlatformInfo Platform { get; }

    Configuration Config { get; }
}
