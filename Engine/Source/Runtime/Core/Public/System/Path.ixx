// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.System:Path;

export import Core.Std;
export import :String;

export class CORE_API Path : public StaticClass
{
private:
	static String EngineDirectory;

public:
	static String GetFileName(const String& path);
	static String GetFileNameWithoutExtension(const String& path);
	static bool HasExtension(const String& path);
	static String ChangeExtension(const String& path, const String& extension);
	static String GetRelativePath(const String& relativeTo, const String& path);
	static String Combine(std::span<const String> paths);
	static String GetFullPath(String InPath);
	static String GetDirectoryName(String InPath);
	static String GetExtension(String InPath);

	static void SetEngineDirectory(String InDir);
	static String GetEngineDirectory();
	static String GetEngineContentDirectory();

public:
	template<class... T>
	static String Combine(T&&... paths) requires requires { { std::initializer_list<String>{ paths... } }; }
	{
		std::vector<String> v{ std::forward<T>(paths)... };
		return Combine(v);
	}
};