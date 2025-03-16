namespace AylaEngine;

public interface IAppLogger
{
    void WriteLine(string format, params ReadOnlySpan<object> args);
    void MarkupLine(string format, params ReadOnlySpan<object> args);
}
