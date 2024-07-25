// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:Directory;

export import :Std;
export import :StaticClass;
export import :String;
export import :SearchOption;

export class CORE_API Directory : public StaticClass
{
public:
	static std::vector<String> GetFiles(String InPath, ESearchOption InSearchOption = ESearchOption::TopDirectoryOnly);
	static std::vector<String> GetDirectories(String InPath, ESearchOption InSearchOption = ESearchOption::TopDirectoryOnly);
	static bool Exists(String InPath);
	static bool CreateDirectory(String InPath);
	static void Delete(String InPath, bool bRecursive);
};