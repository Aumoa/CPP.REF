namespace AylaEngine;

internal class TerminalExecutionException : Exception
{
    public TerminalExecutionException(Terminal.Output output)
    {
        Output = output;
    }

    public readonly Terminal.Output Output;

    public static void ThrowIfFailure(Terminal.Output output)
    {
        if (output.IsFailure)
        {
            throw new TerminalExecutionException(output);
        }
    }
}
