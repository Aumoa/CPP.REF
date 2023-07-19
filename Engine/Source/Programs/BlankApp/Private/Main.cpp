// Copyright 2020-2023 Aumoa.lib. All right reserved.

import Core;

int main()
{
	Log::Information(TEXT("Log system initialized."));
	Log::FlushAll();

	{
		FileStream FS(TEXT("Sample.txt"), EFileAccessMode::Write, EFileSharedMode::None, EFileMode::Create);
		std::string SampleText = TEXT("SampleText").AsCodepage();
		FS.Write(std::span(reinterpret_cast<const uint8*>(SampleText.c_str()), SampleText.length()));
		FS.Close();
	}
	{
		auto Stream = std::make_shared<FileStream>(TEXT("Sample.txt"), EFileAccessMode::Read, EFileSharedMode::None, EFileMode::Open);
		StreamReader Reader(Stream);
		String ReadStr = Reader.ReadToEndAsync().GetResult();
		Stream->Close();
	}
	return 0;
}