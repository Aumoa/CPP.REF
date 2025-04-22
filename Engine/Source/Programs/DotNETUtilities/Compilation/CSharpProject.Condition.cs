// Copyright 2020-2025 AylaEngine. All Rights Reserved.

using System.Text.RegularExpressions;

namespace AylaEngine;

public partial class CSharpProject
{
    public record Condition
    {
        public required string? Configuration { get; init; }

        public required string? Platform { get; init; }

        public override string ToString()
        {
            List<string> left = [];
            List<string> right = [];

            if (Configuration != null)
            {
                left.Add("Configuration");
                right.Add(Configuration);
            }

            if (Platform != null)
            {
                left.Add("Platform");
                right.Add(Platform);
            }

            return $"'{string.Join('|', left.Select(p => $"$({p})"))}'=='{string.Join('|', right)}'";
        }

        public bool Contains(Condition condition)
        {
            if (Configuration != null && condition.Configuration != Configuration)
            {
                return false;
            }

            if (Platform != null && condition.Platform != Platform)
            {
                return false;
            }

            return true;
        }

        public static Condition Parse(string content)
        {
            var items = content.Split("==").Select(p => p.Trim('\'').Split('|')).ToArray();
            if (items.Length != 2)
            {
                throw new FormatException("Invalid condition format.");
            }

            var left = items[0];
            var right = items[1];
            if (left.Length != right.Length)
            {
                throw new FormatException("Invalid condition format.");
            }

            string? configuration = null;
            string? platform = null;

            for (int i = 0; i < left.Length; ++i)
            {
                switch (left[i])
                {
                    case "$(Configuration)":
                        configuration = right[i];
                        break;
                    case "$(Platform)":
                        platform = right[i];
                        break;
                    default:
                        throw new FormatException("Invalid condition format.");
                }
            }

            return new Condition
            {
                Configuration = configuration,
                Platform = platform
            };
        }
    }
}
