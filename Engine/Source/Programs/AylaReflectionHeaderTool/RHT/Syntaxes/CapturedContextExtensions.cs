namespace AylaEngine.RHT;

internal static class CapturedContextExtensions
{
    public static Exception ParsingError(this ICapturedContext context, ReadOnlySpan<char> message)
    {
        return new ParsingErrorException(context.FilePath, context.LineNumber, context.ColumnNumber, message);
    }
}
