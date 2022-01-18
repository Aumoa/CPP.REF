// Copyright 2020-2021 Aumoa.lib. All right reserved.

namespace BuildTool;

class RuleClassRuntimeErrorException : BuildToolException
{
    public RuleClassRuntimeErrorException(string Message) : base(4, Message)
    {
    }
}