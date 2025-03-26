// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

namespace Ayla
{
	class Object;

	class BasePtr
	{
	protected:
		Object* m_Object = nullptr;

	public:
		inline Object* Get() const noexcept { return m_Object; }
	};
}