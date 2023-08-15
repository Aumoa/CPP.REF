// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module AylaHeaderTool:CodeDiagnostic;

import Core;

export class CodeDiagnostic
{
	const String Path;
	const int32 Line;
	const int32 Column;
	const int32 Code;
	const String Message;

public:
	CodeDiagnostic(String InPath, int32 InLine, int32 InColumn, int32 InCode, String InMessage)
		: Path(InPath)
		, Line(InLine)
		, Column(InColumn)
		, Code(InCode)
		, Message(InMessage)
	{
	}
};