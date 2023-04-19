// Copyright 2020-2022 Aumoa.lib. All right reserved.

namespace AE.Misc;

public static class DirectoryExtensions
{
    public static void Cleanup(string DirectoryPath)
    {
        if (Directory.Exists(DirectoryPath))
        {
            Directory.Delete(DirectoryPath, true);
            Directory.CreateDirectory(DirectoryPath);
        }
    }
}
