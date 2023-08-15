// Copyright 2020-2023 Aumoa.lib. All right reserved.

import Core;

Task<String> File::ReadAllTextAsync(String InPath, std::stop_token InCancellationToken)
{
	FileStream Stream(InPath, EFileMode::Open, EFileAccessMode::Read);
	StreamReader Reader(&Stream);
	String Content = co_await Reader.ReadToEndAsync(InCancellationToken);
	Stream.Close();
	co_return Content;
}

bool File::Exists(String InPath)
{
	return std::filesystem::is_regular_file(InPath.path());
}