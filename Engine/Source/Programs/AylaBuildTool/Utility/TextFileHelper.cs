// Copyright 2020-2025 AylaEngine. All Rights Reserved.

namespace AylaEngine;

internal static class TextFileHelper
{
    public static async Task WriteIfChangedAsync(string filePath, string content, CancellationToken cancellationToken)
    {
        var directory = Path.GetDirectoryName(filePath);
        bool directoryExist = Directory.Exists(directory);
        if (directoryExist && File.Exists(filePath))
        {
            var previousContent = await File.ReadAllTextAsync(filePath, cancellationToken);
            if (previousContent.Trim().Replace("\r\n", "\n") == content.Trim())
            {
                return;
            }
        }

        if (directoryExist == false && directory != null)
        {
            Directory.CreateDirectory(directory);
        }

        await File.WriteAllTextAsync(filePath, content.Replace("\n", Environment.NewLine), cancellationToken);
    }
}
