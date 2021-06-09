// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:AController;

import std.core;
import SC.Runtime.Core;
import :AActor;

using namespace std;

export class APawn;

/// <summary>
/// Represents controller that, possess to pawn and control the pawn.
/// </summary>
export class AController : public AActor
{
public:
	using Super = AActor;

private:
	APawn* _possessedPawn = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="AController"/> instance.
	/// </summary>
	/// <param name="name"> The controller name. </param>
	AController();

	/// <summary>
	/// Possess to target pawn.
	/// </summary>
	void Possess(APawn* pawn);

	/// <summary>
	/// Unpossess from current target.
	/// </summary>
	void UnPossess();

protected:
	virtual void OnPossess(APawn* pawn) {}
	virtual void OnUnPossess() {}
};