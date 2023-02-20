// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.CLI;

[AttributeUsage(AttributeTargets.Property)]
public class CommandLineApplyAttribute : Attribute
{
    public CommandLineApplyAttribute()
    {
    }

    public string CategoryName { get; set; } = string.Empty;

    public bool IsRequired { get; set; } = false;
}
