// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:DynamicLibrary;

export import :String;
export import :Action;
export import :Func;

export class CORE_API DynamicLibrary
{
	class Implementation
	{
		void* handle;

	public:
		Implementation(String InLibraryName);
		~Implementation() noexcept;

		bool IsValid() const noexcept;
		void (*LoadFunction(String Signature))();
	};

private:
	String LibraryName;
	Implementation* Impl;

public:
	DynamicLibrary();
	DynamicLibrary(String InLibraryName);
	~DynamicLibrary() noexcept;

	bool IsValid() const;

	template<class... TArgs>
	Action<TArgs...> LoadAction(String Signature)
	{
		void (*Ptr)() = InternalLoadFunction(Signature);
		if (Ptr == nullptr)
		{
			return {};
		}

		return Action<TArgs...>(reinterpret_cast<void(*)(TArgs...)>(Ptr));
	}

	template<class... TArgs>
	Func<TArgs...> LoadFunction(String Signature)
	{
		void (*Ptr)() = InternalLoadFunction(Signature);
		if (Ptr == nullptr)
		{
			return {};
		}

		return Func<TArgs...>::FromAnonymous(Ptr);
	}

private:
	void (*InternalLoadFunction(String Signature))();
};