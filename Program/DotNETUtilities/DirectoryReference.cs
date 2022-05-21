// Copyright 2017 CLOVERGAMES Co., Ltd. All right reserved.

namespace DotNETUtilities;

/// <summary>
/// .NET 환경에서 지원되는 파일 시스템에서 특정 경로에 대한 디렉토리 레퍼런스를 지원합니다.
/// </summary>
public class DirectoryReference : FileSystemReference
{
    readonly DirectoryInfo info;

    /// <summary>
    /// 상대정 경로 및 전체 경로를 포함하여 프로그램에서 인식할 수 있는 경로로 디렉토리 레퍼런스를 생성합니다.
    /// </summary>
    /// <param name="InPath"> 프로그램이 지원하는 경로 유형을 전달합니다. </param>
    public DirectoryReference(string InPath) : base(InPath)
    {
        info = new DirectoryInfo(FullPath);
    }

    /// <summary>
    /// 이 레퍼런스에 해당하는 디렉토리가 존재하지 않는다면 새로 생성합니다.
    /// </summary>
    /// <param name="recursive"> 재귀적으로 경로상의 모든 디렉토리를 생성합니다. </param>
    public void CreateIfNotExists(bool recursive = true)
    {
        if (!IsExist)
        {
            if (recursive)
            {
                GetParent()?.CreateIfNotExists();
            }
            info.Create();
        }
    }

    /// <summary>
    /// 이 레퍼런스에 해당하는 디렉토리에서 특정 디렉토리로 이동합니다.
    /// </summary>
    /// <param name="ChangeDirectoryPath"> 이동하기 위한 상대 경로를 전달합니다. </param>
    /// <returns> 이동된 디렉토리 레퍼런스가 반환됩니다. </returns>
    public DirectoryReference Move(string ChangeDirectoryPath)
    {
        return new(Path.Combine(FullPath, ChangeDirectoryPath));
    }

    /// <summary>
    /// 현재 디렉토리에 있는 파일 레퍼런스를 가져옵니다.
    /// </summary>
    /// <returns> 파일 레퍼런스 목록을 가져옵니다. </returns>
    public FileReference[] GetCurrentFiles()
    {
        return ConvertFilePaths(Directory.GetFiles(FullPath, "*.*"));
    }

    /// <summary>
    /// 현재 디렉토리 및 선택적으로 하위 디렉토리를 포함하여 디렉토리에 있는 파일 레퍼런스를 가져옵니다.
    /// </summary>
    /// <param name="bRecursive"> 재귀적으로 모든 하위 디렉토리를 검사합니다. </param>
    /// <returns> 파일 레퍼런스 목록을 가져옵니다. </returns>
    public FileReference[] GetAllFiles(bool bRecursive = true)
    {
        return ConvertFilePaths(Directory.GetFiles(FullPath, "*.*", bRecursive ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly));
    }

    /// <summary>
    /// 현재 디렉토리에 있는 하위 디렉토리 레퍼런스를 가져옵니다.
    /// </summary>
    /// <returns> 디렉토리 레퍼런스 목록을 가져옵니다. </returns>
    public DirectoryReference[] GetCurrentDirectories()
    {
        return ConvertDirectoryPaths(Directory.GetDirectories(FullPath, "*", SearchOption.TopDirectoryOnly));
    }

    /// <summary>
    /// 이 디렉토리를 지웁니다.
    /// </summary>
    /// <param name="bRecursiveAll"> 재귀적으로 전부 지웁니다. </param>
    public void Remove(bool bRecursiveAll = false)
    {
        Directory.Delete(FullPath, bRecursiveAll);
    }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 디렉토리이 존재하는지 검사합니다.
    /// </summary>
    public bool IsExist
    {
        get => info.Exists;
    }

    /// <summary>
    /// 파일 경로를 파일 레퍼런스 형식으로 변환합니다.
    /// </summary>
    /// <param name="InPaths"> 경로 목록을 전달합니다. </param>
    /// <returns> 레퍼런스 목록을 반환합니다. </returns>
    private static FileReference[] ConvertFilePaths(string[] InPaths)
    {
        if (InPaths is null || InPaths.Length == 0)
        {
            return Array.Empty<FileReference>();
        }
        var References = new FileReference[InPaths.Length];
        for (int i = 0; i < InPaths.Length; ++i)
        {
            References[i] = new(InPaths[i]);
        }

        return References;
    }

    /// <summary>
    /// 디렉토리 경로를 디렉토리 레퍼런스 형식으로 변환합니다.
    /// </summary>
    /// <param name="InPaths"> 경로 목록을 전달합니다. </param>
    /// <returns> 레퍼런스 목록을 반환합니다. </returns>
    private static DirectoryReference[] ConvertDirectoryPaths(string[] InPaths)
    {
        if (InPaths is null || InPaths.Length == 0)
        {
            return Array.Empty<DirectoryReference>();
        }
        var References = new DirectoryReference[InPaths.Length];
        for (int i = 0; i < InPaths.Length; ++i)
        {
            References[i] = new(InPaths[i]);
        }

        return References;
    }

    /// <summary>
    /// 해당 레퍼런스의 디렉토리에 있는 모든 정보를 제거합니다.
    /// </summary>
    /// <param name="bCreateDirectory"> 디렉토리가 존재하지 않을 경우 새로 생성합니다. </param>
    /// <param name="recursive"> 재귀적으로 하위 디렉터리도 제거합니다. </param>
    public void CleanupAllItems(bool bCreateDirectory = true, bool recursive = false)
    {
        // 디렉토리가 없으면 새로 만들고 즉시 리턴합니다.
        if (!info.Exists)
        {
            if (bCreateDirectory)
            {
                info.Create();
            }
            return;
        }

        // 디렉토리 내부 모든 파일을 지웁니다.
        foreach (FileInfo file in info.GetFiles())
        {
            file.Delete();
        }

        if (recursive)
        {
            foreach (DirectoryInfo dir in info.GetDirectories())
            {
                dir.Delete(true);
            }
        }
    }

    /// <summary>
    /// 해당 디렉토리에 하위 파일에 대한 레퍼런스를 가져옵니다.
    /// </summary>
    /// <param name="fileName"> 파일 이름을 전달합니다. </param>
    /// <returns> 생성된 레퍼런스가 반환됩니다. </returns>
    public FileReference GetFile(string fileName)
    {
        return new(Path.Combine(FullPath, fileName));
    }

    /// <summary>
    /// 유저의 임시 디렉토리를 가져옵니다.
    /// </summary>
    /// <param name="additionalRecursePath"> 임시 디렉토리 내부 추가 디렉토리 상태를 전달합니다. </param>
    /// <returns> 임시 디렉토리가 반환됩니다. </returns>
    public static DirectoryReference GetUserTemp(string? additionalRecursePath = null)
    {
        string path = Path.GetTempPath();
        if (!string.IsNullOrEmpty(additionalRecursePath))
        {
            path = Path.Combine(path, additionalRecursePath);
            Directory.CreateDirectory(path);
        }

        return new DirectoryReference(path);
    }

    /// <summary>
    /// 디렉토리가 심볼릭 링크로 생성된 개체인지 나타내는 값을 가져옵니다.
    /// </summary>
    public bool IsSymbolicLink
    {
        get
        {
            if (!IsExist)
            {
                return false;
            }

            FileInfo fi = new(FullPath);
            return fi.Attributes.HasFlag(FileAttributes.ReparsePoint);
        }
    }
}
