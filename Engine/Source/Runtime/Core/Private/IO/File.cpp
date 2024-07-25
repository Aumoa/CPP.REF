// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:File;

export import :StaticClass;
export import :Task;

export class CORE_API File : public StaticClass
{
public:
	static Task<String> ReadAllTextAsync(String InPath, CancellationToken InCancellationToken = {});
	static String ReadAllText(String InPath);
	static Task<> WriteAllTextAsync(String InPath, String InContent, CancellationToken InCancellationToken = {});
	static Task<bool> CompareAndWriteAllTextAsync(String InPath, String InContent, CancellationToken InCancellationToken = {});
	static bool Exists(String InPath);
	static void Delete(String InPath);
};