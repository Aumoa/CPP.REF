// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Piece.h"

class AKing : public APiece
{
public:
	using Super = APiece;

private:
	static constexpr wchar_t AssetPath[] = L"Contents/Chess/Arts/King/Mesh/king.fbx";

public:
	AKing();

	virtual bool QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const override;

protected:
	virtual StaticMesh* GetStaticMesh() const override;
};