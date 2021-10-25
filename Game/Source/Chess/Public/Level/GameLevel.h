// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Level/Level.h"

class AChessBoard;
class AGridIndicator;

class CHESS_API GameLevel : public SLevel
{
	GENERATED_BODY(GameLevel)

private:
	AChessBoard* _psBoard = nullptr;

public:
	GameLevel();

	inline AChessBoard* GetPersistentChessBoard() const { return _psBoard; }

protected:
	virtual void OnLoadLevel() override;
};