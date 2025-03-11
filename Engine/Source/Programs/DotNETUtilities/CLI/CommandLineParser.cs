// Copyright 2020-2025 Aumoa.lib. All right reserved.

using System.Reflection;
using System.Text.Json;

namespace AylaEngine;

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

    public IReadOnlyList<string>? GetCategoryArguments(string categoryName)
    {
        Options.TryGetValue(categoryName.ToLower(), out var list);
        return list;
    }

    public string? GetCategoryFirstArgument(string categoryName)
    {
        var categoryList = GetCategoryArguments(categoryName);
        if (categoryList != null && categoryList.Count > 0)
        {
            return categoryList[0];
        }
        return null;
    }

    public override string ToString()
    {
        return JsonSerializer.Serialize(Options);
    }

    public void ApplyTo(object applyObject)
    {
        Type applyType = applyObject.GetType();
        foreach (var property in applyType.GetProperties())
        {
            var applyAtt = property.GetCustomAttribute<CommandLineApplyAttribute>();
            if (applyAtt == null)
            {
                continue;
            }

            string categoryName = applyAtt.CategoryName;
            IReadOnlyList<string>? categoryList = GetCategoryArguments(categoryName);
            if (categoryList == null)
            {
                if (applyAtt.IsRequired)
                {
                    throw new KeyNotFoundException($"CategoryName `{categoryName}` not found in CommandLineParser instance.");
                }
                continue;
            }

            Type targetType = property.PropertyType;
            bool isList = false;
            if (IsAllowedApplyType(targetType) == false)
            {
                isList = targetType.IsGenericList(out targetType!);
                if (isList == false || IsAllowedApplyType(targetType) == false)
                {
                    throw new InvalidOperationException("Not supported member type.");
                }
            }

            if (isList == false && categoryList.Count > 1)
            {
                throw new InvalidOperationException("Not enough member to fill arguments.");
            }

            if (isList)
            {
                object? list = property.GetValue(applyObject);
                if (list == null)
                {
                    list = property.PropertyType.InstantiateList();
                    property.SetValue(applyObject, list);
                }

                if (targetType.IsString())
                {
                    GenericList.AddRange(list, categoryList);
                }
                else
                {
                    GenericList.AddRange(list, categoryList.Select(Item => Convert.ChangeType(Item, targetType)));
                }
            }
            else
            {
                string? value = categoryList.IsValidIndex(0) ? categoryList[0] : null;
                if (value != null)
                {
                    object? convertedValue = Convert.ChangeType(value, targetType);
                    property.SetValue(applyObject, convertedValue);
                }
                else
                {
                    property.SetValue(applyObject, true);
                }
            }
        }
    }

    private static bool IsAllowedApplyType(Type type)
    {
        return Type.GetTypeCode(type) switch
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

    public static CommandLineParser Parse(params string[] args)
    {
        var parser = new CommandLineParser();

        List<string>? currentList = null;
        List<string> nonCategoryList = new();
        parser.Options.Add(string.Empty, nonCategoryList);

        for (int index = 0; index < args.Length; ++index)
        {
            string current = args[index];
            if (current.StartsWith("-"))
            {
                string command = current[1..];
                if (string.IsNullOrEmpty(command) || command.IsOnlyAlphabet() == false)
                {
                    throw new ArgumentException($"Args[{index}]({current}) is not valid parameter value.", nameof(args));
                }

                command = command.ToLower();
                if (parser.Options.TryGetValue(command, out currentList) == false)
                {
                    currentList = new();
                    parser.Options.Add(command, currentList);
                }
            }
            else
            {
                currentList = currentList ?? nonCategoryList;
                currentList.Add(current);
                currentList = null;
            }
        }

        return parser;
    }
}
