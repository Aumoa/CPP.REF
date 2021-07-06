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
	
	uint8 _bFirst : 1 = true;

public:
	AChessPawn();

	virtual ActionRecord SimulateMove(const GridIndex& index) override;
	virtual bool QueryMovable(MovablePointsQuery& query) const override;

protected:
	virtual StaticMesh* GetStaticMesh() const override;

private:
	inline int32 GetIncrementer() const;

	void QueryMove(MovablePointsQuery& query) const;
	void QueryAttack(MovablePointsQuery& query) const;
};