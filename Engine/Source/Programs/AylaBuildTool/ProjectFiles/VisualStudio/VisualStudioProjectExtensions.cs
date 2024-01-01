// Copyright 2020-2023 Aumoa.lib. All right reserved.

namespace AE.ProjectFiles.VisualStudio;

public static class VisualStudioProjectExtensions
{
    public static bool TryGetFilterPaths(this VisualStudioProject VSProject, out string OutName, out string OutParentPath)
    {
        return TryGetFilterPaths(VSProject.filter, out OutName, out OutParentPath);
    }

    public static bool TryGetFilterPaths(string FilterPath, out string OutName, out string OutParentPath)
    {
        var Paths = FilterPath.Split(Path.DirectorySeparatorChar);
        OutName = Paths[^1];
        if (Paths.Length > 1)
        {
            OutParentPath = Paths[^2];
            return true;
        }

        OutParentPath = string.Empty;
        return false;
    }
}
