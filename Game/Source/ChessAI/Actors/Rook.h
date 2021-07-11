// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Piece.h"

class ARook : public APiece
{
public:
	using Super = APiece;

private:
	static constexpr wchar_t AssetPath[] = L"Contents/Chess/Arts/Rook/Mesh/rook.fbx";

public:
	ARook();

	virtual bool QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const override;

protected:
	virtual StaticMesh* GetStaticMesh() const override;
};