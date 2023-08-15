// Copyright 2020-2023 Aumoa.lib. All right reserved.

import Core.Std;
import Core.System;
import Core.IO;

StreamReader::StreamReader(std::shared_ptr<Stream> InStream)
	: CurrentStream_Capture(std::move(InStream))
	, CurrentStream(CurrentStream_Capture.get())
{
}

StreamReader::StreamReader(Stream* InStreamRaw)
	: CurrentStream(InStreamRaw)
{
}

String StreamReader::ReadLine()
{
	return ReadLineAsync().GetResult();
}

Task<String> StreamReader::ReadLineAsync(std::stop_token InCancellationToken)
{
	size_t EndPos = static_cast<size_t>(-1);

	if (Buffer.size() == 0)
	{
		bool bExpanded = co_await ExpandBufferAsync(InCancellationToken);
		if (bExpanded == false)
		{
			co_return String::GetEmpty();
		}
	}

	size_t StartPos = BufferPos;
	while (bEOF == false)
	{
		for (size_t i = BufferPos; i < Buffer.size(); ++i)
		{
			if (Buffer[i] == '\n')
			{
				EndPos = Buffer[i];
				break;
			}
		}

		if (EndPos != -1)
		{
			break;
		}

		BufferPos = Buffer.size();
		if (co_await ExpandBufferAsync(InCancellationToken) == false)
		{
			EndPos = Buffer.size();
			break;
		}
	}

	size_t Length = EndPos - StartPos;
	if (Length == 0)
	{
		co_return String::GetEmpty();
	}

	const char* StringPtr = reinterpret_cast<const char*>(Buffer.data()) + StartPos;
	String Str = String::FromCodepage(std::string_view(StringPtr, Length));
	BufferPos = EndPos;
	co_await TryShrinkAndFillAsync(InCancellationToken);
	co_return Str;
}

Task<String> StreamReader::ReadToEndAsync(std::stop_token InCancellationToken)
{
	while (bEOF == false)
	{
		co_await ExpandBufferAsync(InCancellationToken);
	}

	size_t Length = Buffer.size() - BufferPos;
	if (Length == 0)
	{
		co_return String::GetEmpty();
	}

	const char* StringPtr = reinterpret_cast<const char*>(Buffer.data() + BufferPos);
	String Str = String::FromCodepage(std::string_view(StringPtr, Length));
	BufferPos = Buffer.size();
	co_await TryShrinkAndFillAsync(InCancellationToken);
	co_return Str;
}

bool StreamReader::IsEOF() const
{
	return bEOF;
}

Task<> StreamReader::TryShrinkAndFillAsync(std::stop_token InCancellationToken)
{
	if (BufferPos == 0 || BufferPos < InitialBufferSize)
	{
		co_return;
	}

	size_t CopySize = Buffer.size() - BufferPos;
	size_t ShrinkSize = std::exchange(BufferPos, 0);
	memmove(Buffer.data() + ShrinkSize, Buffer.data(), CopySize);
	if (bEOF)
	{
		Buffer.resize(Buffer.size() - ShrinkSize);
		co_return;
	}

	size_t ActualRead = co_await CurrentStream->ReadAsync(std::span(Buffer.data() + CopySize, ShrinkSize), InCancellationToken);
	if (ActualRead < ShrinkSize)
	{
		Buffer.resize(Buffer.size() - (ShrinkSize - ActualRead));
	}

	bEOF = ActualRead == 0;
}

Task<bool> StreamReader::ExpandBufferAsync(std::stop_token InCancellationToken)
{
	size_t SizeToRead;
	uint8* BufferPtr;

	if (bEOF)
	{
		co_return false;
	}

	if (Buffer.size() == 0)
	{
		SizeToRead = InitialBufferSize;
	}
	else
	{
		SizeToRead = Buffer.size();
	}

	size_t Offset = Buffer.size();
	Buffer.resize(Buffer.size() + SizeToRead);
	BufferPtr = Buffer.data() + Offset;

	size_t ActualRead = co_await CurrentStream->ReadAsync(std::span(BufferPtr, SizeToRead), InCancellationToken);
	if (ActualRead < SizeToRead)
	{
		size_t Diff = SizeToRead - ActualRead;
		Buffer.resize(Buffer.size() - Diff);
	}

	bEOF = ActualRead == 0;
	co_return !bEOF;
}