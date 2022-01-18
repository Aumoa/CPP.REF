namespace BuildTool;

class BuildToolException : Exception
{
    public BuildToolException(int ReturnErrorCode, string Message) : base(Message)
    {
        ReturnCode = ReturnErrorCode;
    }

    public readonly int ReturnCode;
}