using BuildTool;

namespace DotNETUtilites;

class CommandLineArgumentException : BuildToolException
{
    public CommandLineArgumentException(string Message) : base(-1, Message)
    {
    }
}