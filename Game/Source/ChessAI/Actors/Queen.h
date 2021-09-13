// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameMinimal.h"
#include "Piece.h"

class AQueen : public APiece
{
public:
	using Super = APiece;

private:
	static constexpr wchar_t AssetPath[] = L"Content/Chess/Arts/Queen/Mesh/queen.fbx";

public:
	AQueen();

	virtual bool QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const override;

protected:
	virtual SStaticMesh* GetStaticMesh() const override;
};