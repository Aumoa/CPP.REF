// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	struct GUIEvent
	{
		enum class Types
		{
			Repaint,
			MouseMove,
			MouseDown,
			MouseUp,
			Used
		};

		Types Type;
		Vector2F MousePosition;
		int32 Button;

		void Use() const
		{
			// Use const_cast to represent event consumption.
			const_cast<GUIEvent*>(this)->Type = Types::Used;
		}

	public:
		static constexpr GUIEvent Repaint()
		{
			return GUIEvent
			{
				.Type = Types::Repaint,
				.MousePosition = Vector2N::Zero(),
				.Button = 0
			};
		}

		static constexpr GUIEvent MouseMove(const Vector2N& mousePosition)
		{
			return GUIEvent
			{
				.Type = Types::MouseMove,
				.MousePosition = Vector<>::Cast<Vector2F>(mousePosition),
				.Button = 0
			};
		}

		static constexpr GUIEvent MouseDown(const Vector2N& mousePosition, int32 button)
		{
			return GUIEvent
			{
				.Type = Types::MouseDown,
				.MousePosition = Vector<>::Cast<Vector2F>(mousePosition),
				.Button = button
			};
		}

		static constexpr GUIEvent MouseUp(const Vector2N& mousePosition, int32 button)
		{
			return GUIEvent
			{
				.Type = Types::MouseUp,
				.MousePosition = Vector<>::Cast<Vector2F>(mousePosition),
				.Button = button
			};
		}
	};
}