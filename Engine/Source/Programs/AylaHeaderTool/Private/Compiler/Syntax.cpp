// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Compiler/Syntax.h"

Syntax::Syntax(String InWord)
	: Word(InWord)
{
}

String Syntax::GetWord() const
{
	return Word;
}