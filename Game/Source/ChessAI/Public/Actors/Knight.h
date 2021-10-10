// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"

class CHESSAI_API AKnight : public APiece
{
	GENERATED_BODY(AKnight)

private:
	static constexpr wchar_t AssetPath[] = L"Content/Chess/Arts/Knight/Mesh/knight.fbx";

public:
	AKnight();

	virtual bool QueryMovable(MovablePointsQuery& query, const ChessBoardBuilt& built) const override;

protected:
	virtual SStaticMesh* GetStaticMesh() const override;
};