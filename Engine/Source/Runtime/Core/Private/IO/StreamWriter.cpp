// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/StreamWriter.h"

StreamWriter::StreamWriter(std::shared_ptr<Stream> InStream)
	: CurrentStream_Capture(std::move(InStream))
	, CurrentStream(CurrentStream_Capture.get())
{
}

StreamWriter::StreamWriter(Stream* InStreamPtr)
	: CurrentStream(InStreamPtr)
{
}

void StreamWriter::Write(String Val)
{
	std::string MbChars = Val.AsCodepage();
	CurrentStream->Write({ (const uint8*)MbChars.c_str(), MbChars.length() });
}

Task<> StreamWriter::WriteAsync(String Val)
{
	std::string MbChars = Val.AsCodepage();
	co_await CurrentStream->WriteAsync({ (const uint8*)MbChars.c_str(), MbChars.length() });
}