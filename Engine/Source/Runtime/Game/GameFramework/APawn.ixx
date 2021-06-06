// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:APawn;

import std.core;
import :AActor;

using namespace std;

export class AController;

/// <summary>
/// Represents pawn that controller can be possessed.
/// </summary>
export class APawn : public AActor
{
public:
	using Super = AActor;

private:
	AController* _controller = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="APawn"/> instance.
	/// </summary>
	/// <param name="name"> The pawn name. </param>
	APawn(wstring_view name);

	/// <summary>
	/// The function called when possessed by controller.
	/// </summary>
	virtual void PossessedBy(AController* controller);

	/// <summary>
	/// The function called when unpossessed.
	/// </summary>
	virtual void UnPossessed();

	/// <summary>
	/// Get current possessed controller.
	/// </summary>
	/// <returns></returns>
	template<derived_from<AController> T>
	T* GetController() const
	{
		return dynamic_cast<T>(_controller);
	}
};