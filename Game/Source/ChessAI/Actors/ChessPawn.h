// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Piece.h"

class AChessPawn : public APiece
{
public:
	using Super = APiece;

private:
	static constexpr wchar_t AssetPath[] = L"Contents/Chess/Arts/Pawn/Mesh/pawn.fbx";

public:
	AChessPawn();

protected:
	virtual StaticMesh* GetStaticMesh() const override;
};