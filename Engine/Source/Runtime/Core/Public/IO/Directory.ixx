// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:Directory;

import Core.System;
import :SearchOption;

export class CORE_API Directory : public StaticClass
{
public:
	static std::vector<String> GetFiles(String InPath, ESearchOption InSearchOption = ESearchOption::TopDirectoryOnly);
	static bool Exists(String InPath);
};