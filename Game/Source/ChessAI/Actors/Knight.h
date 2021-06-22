// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Piece.h"

class AKnight : public APiece
{
public:
	using Super = APiece;

private:
	static constexpr wchar_t AssetPath[] = L"Contents/Chess/Arts/Knight/Mesh/knight.fbx";

public:
	AKnight();

protected:
	virtual StaticMesh* GetStaticMesh() const override;
};