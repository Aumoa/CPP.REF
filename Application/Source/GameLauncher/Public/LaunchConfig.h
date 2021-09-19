// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class GAMELAUNCHER_API SLaunchConfig : virtual public SObject
{
	GENERATED_BODY(SLaunchConfig)

public:
	struct Element
	{
		std::wstring Name;
		std::filesystem::path Icon;
		std::filesystem::path Redirect;
	};

private:
	std::vector<Element> _elements;

public:
	SLaunchConfig();

	void Init();
	const std::vector<Element>& GetElements() const { return _elements; }
};