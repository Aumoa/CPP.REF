// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/TextWriter.h"

TextWriter::TextWriter()
	: CoreNewLine('\n'_c)
{
}

TextWriter::~TextWriter() noexcept
{
}

void TextWriter::Write(String Val)
{
	for (auto& Ch : Val)
	{
		Write(Ch);
	}
}

void TextWriter::Write(bool bVal)
{
	Write(bVal ? TEXT("True") : TEXT("False"));
}

void TextWriter::Write(int32 Val)
{
	Write(Int32(Val).ToString());
}

void TextWriter::Write(uint32 Val)
{
	Write(UInt32(Val).ToString());
}

void TextWriter::Write(int64 Val)
{
	Write(Int64(Val).ToString());
}

void TextWriter::Write(uint64 Val)
{
	Write(UInt64(Val).ToString());
}

void TextWriter::Write(float Val)
{
	Write(Single(Val).ToString());
}

void TextWriter::Write(double Val)
{
	Write(Double(Val).ToString());
}

void TextWriter::WriteLine()
{
	Write(CoreNewLine);
}

void TextWriter::WriteLine(char_t Val)
{
	Write(Val);
	WriteLine();
}

void TextWriter::WriteLine(String Val)
{
	Write(Val);
	WriteLine();
}

void TextWriter::WriteLine(bool bVal)
{
	Write(bVal);
	WriteLine();
}

void TextWriter::WriteLine(int32 Val)
{
	Write(Val);
	WriteLine();
}

void TextWriter::WriteLine(uint32 Val)
{
	Write(Val);
	WriteLine();
}

void TextWriter::WriteLine(int64 Val)
{
	Write(Val);
	WriteLine();
}

void TextWriter::WriteLine(uint64 Val)
{
	Write(Val);
	WriteLine();
}

void TextWriter::WriteLine(float Val)
{
	Write(Val);
	WriteLine();
}

void TextWriter::WriteLine(double Val)
{
	Write(Val);
	WriteLine();
}