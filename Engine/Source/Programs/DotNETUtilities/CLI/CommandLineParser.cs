// Copyright 2020-2022 Aumoa.lib. All right reserved.

using AE.Misc;

using System.Reflection;
using System.Text.Json;

namespace AE.CLI;

public class CommandLineParser
{
    private readonly Dictionary<string, List<string>> Options = new();

    private CommandLineParser()
    {
    }

    public IReadOnlyList<string> GetNonCategoryArguments()
    {
        return Options[string.Empty];
    }

    public IReadOnlyList<string>? GetCategoryArguments(string CategoryName)
    {
        Options.TryGetValue(CategoryName.ToLower(), out var OutList);
        return OutList;
    }

    public string? GetCategoryFirstArgument(string CategoryName)
    {
        var CategoryList = GetCategoryArguments(CategoryName);
        if (CategoryList != null && CategoryList.Count > 0)
        {
            return CategoryList[0];
        }
        return null;
    }

    public override string ToString()
    {
        return JsonSerializer.Serialize(Options);
    }

    public void ApplyTo(object ApplyObject)
    {
        Type ApplyType = ApplyObject.GetType();
        foreach (var Property in ApplyType.GetProperties())
        {
            var ApplyAtt = Property.GetCustomAttribute<CommandLineApplyAttribute>();
            if (ApplyAtt == null)
            {
                continue;
            }

            string CategoryName = ApplyAtt.CategoryName;
            IReadOnlyList<string>? CategoryList = GetCategoryArguments(CategoryName);
            if (CategoryList == null)
            {
                if (ApplyAtt.IsRequired)
                {
                    throw new KeyNotFoundException($"CategoryName `{CategoryName}` not found in CommandLineParser instance.");
                }
                continue;
            }

            Type TargetType = Property.PropertyType;
            bool bIsList = false;
            if (IsAllowedApplyType(TargetType) == false)
            {
                bIsList = TargetType.IsGenericList(out TargetType!);
                if (bIsList == false || IsAllowedApplyType(TargetType) == false)
                {
                    throw new InvalidOperationException("Not supported member type.");
                }
            }

            if (bIsList == false && CategoryList.Count > 1)
            {
                throw new InvalidOperationException("Not enough member to fill arguments.");
            }

            if (bIsList)
            {
                object? List = Property.GetValue(ApplyObject);
                if (List == null)
                {
                    List = Property.PropertyType.InstantiateList();
                    Property.SetValue(ApplyObject, List);
                }

                if (TargetType.IsString())
                {
                    List.GenericAddRange(CategoryList);
                }
                else
                {
                    List.GenericAddRange(CategoryList.Select(Item => Convert.ChangeType(Item, TargetType)));
                }
            }
            else
            {
                string? Value = CategoryList.IsValidIndex(0) ? CategoryList[0] : null;
                if (Value != null)
                {
                    object? ConvertedValue = Convert.ChangeType(Value, TargetType);
                    Property.SetValue(ApplyObject, ConvertedValue);
                }
                else
                {
                    Property.SetValue(ApplyObject, true);
                }
            }
        }
    }

    private static bool IsAllowedApplyType(Type InType)
    {
        return Type.GetTypeCode(InType) switch
        {
            TypeCode.Byte or
            TypeCode.SByte or
            TypeCode.UInt16 or
            TypeCode.UInt32 or
            TypeCode.UInt64 or
            TypeCode.Int16 or
            TypeCode.Int32 or
            TypeCode.Int64 or
            TypeCode.Single or
            TypeCode.Double or
            TypeCode.Decimal or
            TypeCode.Boolean or
            TypeCode.String => true,
            _ => false
        };
    }

    public static CommandLineParser Parse(params string[] Args)
    {
        var Parser = new CommandLineParser();

        List<string>? CurrentList = null;
        List<string> NonCategoryList = new();
        Parser.Options.Add(string.Empty, NonCategoryList);

        for (int Index = 0; Index < Args.Length; ++Index)
        {
            string Current = Args[Index];
            if (Current.StartsWith("-"))
            {
                string Command = Current[1..];
                if (string.IsNullOrEmpty(Command) || Command.IsOnlyAlphabet() == false)
                {
                    throw new ArgumentException($"Args[{Index}]({Current}) is not valid parameter value.", nameof(Args));
                }

                Command = Command.ToLower();
                if (Parser.Options.TryGetValue(Command, out CurrentList) == false)
                {
                    CurrentList = new();
                    Parser.Options.Add(Command, CurrentList);
                }
            }
            else
            {
                CurrentList = CurrentList ?? NonCategoryList;
                CurrentList.Add(Current);
                CurrentList = null;
            }
        }

        return Parser;
    }
}
