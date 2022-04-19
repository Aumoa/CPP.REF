// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Game
{
	class SLevel;
	class SWorld;

	/// <summary>
	/// Represents all classes in the smart component game framework hierarchy and provides low-level services to derived class.
	/// </summary>
	class GAME_API SGameObject : extends(SObject)
	{
		GENERATED_BODY(SGameObject);

	private:
		SPROPERTY(Outer)
		SGameObject* Outer = nullptr;
		std::wstring Name;

	public:
		SGameObject();

		virtual std::wstring ToString() override;

		SGameObject* GetOuter() const;
		void SetOuter(SGameObject* Value);
		std::wstring_view GetName() const;
		void SetName(std::wstring_view Value);

		virtual SLevel* GetLevel();
		virtual SWorld* GetWorld();

	public:
		static SObject* LoadObject(const std::filesystem::path& assetPath);

	public:
		template<std::derived_from<SObject> T>
		static T* LoadObject(const std::filesystem::path& assetPath)
		{
			return Cast<T>(LoadObject(assetPath));
		}

		template<std::derived_from<SGameObject> T>
		T* GetOuter()
		{
			return Cast<T>(GetOuter());
		}

	private:
		mutable SWorld* _WorldPrivate = nullptr;
	};
}