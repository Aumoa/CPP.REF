namespace AylaEngine;

internal class ParsingErrorException(string filePath, int lineNumber, int columnNumber, ReadOnlySpan<char> message)
    : Exception($"{filePath}({lineNumber},{columnNumber}): {message}")
{
}
