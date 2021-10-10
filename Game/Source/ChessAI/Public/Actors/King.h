// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"

class CHESSAI_API AKing : public APiece
{
	GENERATED_BODY(AKing)

private:
	static constexpr wchar_t AssetPath[] = L"Content/Chess/Arts/King/Mesh/king.fbx";

public:
	AKing();

	virtual bool QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const override;

protected:
	virtual SStaticMesh* GetStaticMesh() const override;
};