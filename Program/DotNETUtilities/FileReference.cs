// Copyright 2017 CLOVERGAMES Co., Ltd. All right reserved.

using System.Text;

namespace DotNETUtilities;

/// <summary>
/// .NET 환경에서 지원되는 파일 시스템에서 특정 경로에 대한 파일 레퍼런스를 지원합니다.
/// </summary>
public class FileReference : FileSystemReference
{
    /// <summary>
    /// 상대정 경로 및 전체 경로를 포함하여 프로그램에서 인식할 수 있는 경로로 파일 레퍼런스를 생성합니다.
    /// </summary>
    /// <param name="InPath"> 프로그램이 지원하는 경로 유형을 전달합니다. </param>
    public FileReference(string InPath) : base(InPath)
    {
        Extension = Path.GetExtension(FullPath);

        if (Extension is not null)
        {
            FileName = $"{Name}{Extension}";
        }
        else
        {
            FileName = Name;
        }
    }

    /// <summary>
    /// 확장자를 비교합니다.
    /// </summary>
    /// <param name="InExtension"> 확장자를 전달합니다. </param>
    /// <returns> 비교 결과가 반환됩니다. </returns>
    public bool CompareExtension(string InExtension)
    {
        if (InExtension[0] is not '.')
        {
            InExtension = $".{InExtension}";
        }

        return Extension?.Equals(InExtension, StringComparison.OrdinalIgnoreCase) == true;
    }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 파일이 존재하는지 검사합니다.
    /// </summary>
    public bool IsExist
    {
        get => File.Exists(FullPath);
    }

    /// <summary>
    /// 확장자가 포함된 파일 이름을 가져옵니다.
    /// </summary>
    public string FileName { get; }

    /// <summary>
    /// 지정된 경로에서 대상의 확장자를 가져옵니다.
    /// </summary>
    public string? Extension { get; }

    /// <summary>
    /// 파일이 존재할 경우 파일을 지웁니다.
    /// </summary>
    public void Delete()
    {
        if (IsExist)
        {
            File.Delete(FullPath);
        }
    }

    /// <summary>
    /// 파일을 대상 위치로 복사합니다.
    /// </summary>
    /// <param name="copyDst"> 대상 위치를 전달합니다. </param>
    /// <param name="overwrite"> 파일을 덮어쓸지 나타내는 값을 전달합니다. </param>
    public async Task CopyToAsync(FileReference copyDst, bool overwrite = false)
    {
        await Task.Run(() =>
        {
            if (copyDst.IsExist && overwrite)
            {
                copyDst.Delete();
            }
            File.Copy(FullPath, copyDst.FullPath);
        });
    }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 파일의 텍스트 데이터를 모두 읽습니다.
    /// </summary>
    /// <returns> 읽은 텍스트 데이터가 반환됩니다. </returns>
    public string ReadAllText()
    {
        return File.ReadAllText(FullPath);
    }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 파일의 텍스트 데이터를 모두 읽습니다.
    /// </summary>
    /// <param name="encoding"> 인코딩 정보를 전달합니다. </param>
    /// <returns> 읽은 텍스트 데이터가 반환됩니다. </returns>
    public string ReadAllText(Encoding encoding)
    {
        return File.ReadAllText(FullPath, encoding);
    }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 파일에 모든 텍스트 데이터를 씁니다.
    /// </summary>
    /// <param name="text"> 텍스트 데이터를 전달합니다. </param>
    /// <param name="encoding"> 인코딩 정보를 전달합니다. </param>
    public void WriteAllText(string text, Encoding? encoding = null)
    {
        if (encoding != null)
        {
            File.WriteAllText(FullPath, text, encoding);
        }
        else
        {
            File.WriteAllText(FullPath, text);
        }
    }

    /// <summary>
    /// 해당 레퍼런스에 해당하는 파일에 모든 텍스트 데이터를 씁니다. 파일이 변경되었을 때에만 쓰기 작업을 실행합니다.
    /// </summary>
    /// <param name="text"> 텍스트 데이터를 전달합니다. </param>
    /// <param name="encoding"> 인코딩 정보를 전달합니다. </param>
    /// <returns> 쓰기 작업이 실행되었으면 <see langword="true"/>가, 그 이외의 경우 <see langword="false"/>가 반환됩니다. </returns>
    public bool WriteAllTextIfChanged(string text, Encoding? encoding = null)
    {
        if (IsExist && ReadAllText() == text)
        {
            return false;
        }

        WriteAllText(text, encoding);
        return true;
    }

    /// <summary>
    /// 레퍼런스에 해당하는 파일의 스트림을 엽니다.
    /// </summary>
    /// <param name="fileMode"> 파일 열기 모드를 전달합니다. </param>
    /// <returns> 열린 스트림이 반환됩니다. </returns>
    public FileStream OpenStream(FileMode fileMode = FileMode.Open)
    {
        GetParent()?.CreateIfNotExists(recursive: true);

        return new FileStream(FullPath, fileMode);
    }

    /// <summary>
    /// 임시 파일 레퍼런스를 획득합니다.
    /// </summary>
    /// <returns> 파일 레퍼런스가 반환됩니다. </returns>
    public static FileReference GetTemp()
    {
        return new FileReference(Guid.NewGuid().ToString());
    }

    /// <summary>
    /// 파일의 확장자 명을 변경하고 대상으로 새 참조를 생성합니다.
    /// </summary>
    /// <param name="newExtensions"> 변경할 확장명을 전달합니다. </param>
    /// <returns> 생성된 개체가 반환됩니다. </returns>
    public FileReference ChangeExtensions(string newExtensions)
    {
        return new FileReference(Path.ChangeExtension(FullPath, newExtensions));
    }
}
