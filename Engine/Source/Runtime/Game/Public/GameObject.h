// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Game
{
	class SLevel;
	class SWorld;

	/// <summary>
	/// Represents all classes in the smart component game framework hierarchy and provides low-level services to derived class.
	/// </summary>
	class GAME_API SGameObject : implements(SObject)
	{
		GENERATED_BODY(SGameObject);

	private:
		SPROPERTY(Outer)
		SGameObject* Outer = nullptr;
		String Name;

	public:
		SGameObject();

		virtual String ToString() override;

		SGameObject* GetOuter() const;
		void SetOuter(SGameObject* Value);
		String GetName() const;
		void SetName(String Value);

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