// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/File.h"
#include "IO/FileStream.h"
#include "IO/StreamReader.h"
#include "IO/StreamWriter.h"

namespace Ayla
{
	Task<String> File::ReadAllTextAsync(String InPath, CancellationToken InCancellationToken)
	{
		FileStream Stream(InPath, FileMode::Open, FileAccessMode::Read, FileSharedMode::Read);
		StreamReader Reader(&Stream);
		String Content = co_await Reader.ReadToEndAsync(InCancellationToken);
		Stream.Close();
		co_return Content;
	}

	String File::ReadAllText(String InPath)
	{
		return ReadAllTextAsync(InPath).GetResult();
	}

	Task<> File::WriteAllTextAsync(String InPath, String InContent, CancellationToken InCancellationToken)
	{
		FileStream Stream(InPath, FileMode::Create, FileAccessMode::Write);
		StreamWriter Writer(&Stream);
		co_await Writer.WriteAsync(InContent);
		Stream.Close();
	}

	Task<bool> File::CompareAndWriteAllTextAsync(String InPath, String InContent, CancellationToken InCancellationToken)
	{
		if (Exists(InPath))
		{
			String PrevContent = co_await ReadAllTextAsync(InPath, InCancellationToken);
			if (PrevContent == InContent)
			{
				co_return false;
			}
		}

		co_await WriteAllTextAsync(InPath, InContent, InCancellationToken);
		co_return true;
	}

	bool File::Exists(String InPath)
	{
		return std::filesystem::is_regular_file(InPath.path());
	}

	void File::Delete(String InPath)
	{
		std::filesystem::remove(InPath.path());
	}
}