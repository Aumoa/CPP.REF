// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Compiler/SourceCodeLocator.h"

SourceCodeLocator::SourceCodeLocator(String InFilenameHint, String InSourceCode)
	: FilenameHint(InFilenameHint)
	, SourceCode(InSourceCode)
	, Pos(0)
{
	for (auto& Ch : SourceCode)
	{
		if (Ch == '\n')
		{
			Lines.emplace_back(Pos + 1);
		}

		++Pos;
	}

	// Reset position.
	Pos = 0;
}

String SourceCodeLocator::ToString() const
{
	return GetCurrentView().ToString();
}

void SourceCodeLocator::SetEOF()
{
	Pos = SourceCode.length();
}

void SourceCodeLocator::IncrementPos(int32 Pos)
{
	check(IsEOF() == false);
	this->Pos += Pos;
}

void SourceCodeLocator::IncrementLine()
{
	check(IsEOF() == false);
	for (size_t& I : Lines)
	{
		if (Pos < I)
		{
			Pos = I;
			return;
		}
	}
	SetEOF();
}

void SourceCodeLocator::SkipWhitespace()
{
	check(IsEOF() == false);

	String View = GetCurrentView();
	for (size_t I = 0; I < View.length(); ++I)
	{
		if (Char::IsWhiteSpace(View[I]) == false)
		{
			IncrementPos(I);
			return;
		}
	}

	SetEOF();
}

String SourceCodeLocator::GetCurrentView() const
{
	check(IsEOF() == false);
	return SourceCode.SubstringView(Pos);
}

SourceCodeLocation SourceCodeLocator::GetPosition() const
{
	check(IsEOF() == false);

	SourceCodeLocation Location;
	Location.Filename = FilenameHint;
	Location.LinePos = 0;
	Location.CharPos = Pos;
	Location.SeekPos = Pos;

	for (const size_t& I : Lines)
	{
		++Location.LinePos;
		if (Pos < I)
		{
			return Location;
		}
		else
		{
			Location.CharPos = Pos - I;
		}
	}

	throw InvalidOperationException();
}

bool SourceCodeLocator::IsEOF() const
{
	return Pos >= SourceCode.length();
}

String SourceCodeLocator::GetWord(bool bStringView, bool bPeek)
{
	check(IsEOF() == false);

	constexpr std::array CodeSplits = { ' ', '{', '}', ':', '<', '>', '(', ')', '\r', '\n' };
	String View = GetCurrentView();
	size_t IndexOf = View.IndexOfAny(CodeSplits);
	if (IndexOf != -1)
	{
		if (IndexOf == 0 && View.length() > 0)
		{
			// Include current char.
			IndexOf = 1;
		}

		// Special scopes.
		if (View.length() >= 2 && View[0] == ':' && View[1] == ':')
		{
			IndexOf = 2;
		}

		View = View.SubstringView(0, IndexOf);
	}

	if (bPeek == false)
	{
		IncrementPos(View.length());
		SkipWhitespace();
	}

	if (bStringView == false)
	{
		View = View.Clone();
	}

	return View;
}